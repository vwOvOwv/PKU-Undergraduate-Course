/*
 * Copyright (C) 2017 CAMELab
 *
 * This file is part of SimpleSSD.
 *
 * SimpleSSD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSSD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "hil/ufs/host.hh"

#include "hil/ufs/device.hh"
#include "util/algorithm.hh"
#include "util/fifo.hh"
#include "util/interface.hh"

namespace SimpleSSD {

namespace HIL {

namespace UFS {

struct RequestContext {
  uint32_t bitMask;
  uint32_t index;
  UTPTransferReqDesc transferReqDesc;
};

Host::Host(Interface *i, ConfigReader &c)
    : pInterface(i),
      conf(c),
      requestCounter(0),
      lastWorkAt(0),
      pendingInterrupt(0) {
  ARM::AXI::BUS_WIDTH hostBusWidth;
  uint64_t hostBusClock;

  // Get AXI setting
  hostBusWidth =
      (ARM::AXI::BUS_WIDTH)conf.readInt(CONFIG_UFS, UFS_HOST_AXI_BUS_WIDTH);
  hostBusClock = conf.readUint(CONFIG_UFS, UFS_HOST_AXI_CLOCK);

  FIFOParam fifoParam;

  fifoParam.rqSize = 8192;
  fifoParam.wqSize = 8192;
  fifoParam.transferUnit = 2048;
  fifoParam.latency = [hostBusWidth, hostBusClock](uint64_t size) -> uint64_t {
    return ARM::AXI::Stream::calculateDelay(hostBusClock, hostBusWidth, size);
  };

  axiFIFO = new FIFO(pInterface, fifoParam);

  // Calculate AXI Stream bus width/speed for M-PHY layer
  auto mode = (MIPI::M_PHY::M_PHY_MODE)conf.readInt(CONFIG_UFS, UFS_MPHY_MODE);
  uint8_t lane = (uint8_t)conf.readUint(CONFIG_UFS, UFS_MPHY_LANE);

  // Assume maximum 2 lanes
  switch (mode) {
    case MIPI::M_PHY::HS_G1:  // 1.248 Gbps
    case MIPI::M_PHY::HS_G2:  // 2.496 Gbps
    case MIPI::M_PHY::HS_G3:  // 4.992 Gbps
      hostBusWidth = ARM::AXI::BUS_64BIT;
      hostBusClock = 200000000;  // 64Bit @ 200MHz -> 1.6GB/s

      break;
    case MIPI::M_PHY::HS_G4:  // 9.984 Gbps
      hostBusWidth = ARM::AXI::BUS_128BIT;
      hostBusClock = 200000000;  // 128Bit @ 200MHz -> 3.2GB/s

      break;
    default:  // Suppress warning
      break;
  }

  fifoParam.latency = [mode, lane](uint64_t size) -> uint64_t {
    return MIPI::M_PHY::calculateDelay(mode, lane, size);
  };

  mphyFIFO = new FIFO(axiFIFO, fifoParam);

  fifoParam.latency = [hostBusWidth, hostBusClock](uint64_t size) -> uint64_t {
    return ARM::AXI::Stream::calculateDelay(hostBusClock, hostBusWidth, size);
  };

  deviceFIFO = new FIFO(mphyFIFO, fifoParam);

  pDevice = new Device(deviceFIFO, c);

  // Initialize registers
  register_table.cap = 0x0707001F;
  register_table.version = 0x00010000;
  register_table.hcddid = 0xAA003C3C;
  register_table.hcpmid = 0x41524D48;
  register_table.hcs = 0x00000008;

  memset(&stat, 0, sizeof(stat));

  // Allocate events
  workEvent = allocate([this](uint64_t) { work(); });
  requestEvent = allocate([this](uint64_t) { handleRequest(); });
  completionEvent = allocate([this](uint64_t) { completion(); });

  maxRequest = conf.readUint(CONFIG_UFS, UFS_MAX_REQUEST_COUNT);
  workInterval = conf.readUint(CONFIG_UFS, UFS_WORK_INTERVAL);
  requestInterval = workInterval / maxRequest;
}

Host::~Host() {
  delete pDevice;
  delete deviceFIFO;
  delete mphyFIFO;
  delete axiFIFO;
}

void Host::processUIC() {
  uint8_t opcode = register_table.uiccmdr & 0xFF;

  register_table.ucmdarg2 = ERR_SUCCESS;

  debugprint(LOG_HIL_UFS,
             "COMMAND | UIC Command | CMD %08X | ARGS %08X, %08X, %08X",
             register_table.uiccmdr, register_table.ucmdarg1,
             register_table.ucmdarg2, register_table.ucmdarg3);

  switch (opcode) {
    case DME_LINKSTARTUP:
      register_table.hcs |= 0x0F;

      schedule(workEvent, getTick() + workInterval);

      break;
    case DME_GET:
      if (register_table.ucmdarg1 == MAKE_UICARG(MIB_VS_POWERSTATE, 0)) {
        register_table.ucmdarg3 = UFSHCD_LINK_IS_UP;
      }

      break;
    default:
      // Do Nothing
      break;
  }

  register_table.is |= UIC_COMMAND_COMPL;

  pInterface->generateInterrupt();

  register_table.uiccmdr = 0;

  stat.uicCommand++;
}

void Host::processUTPTask() {
  // Not Implemented
  panic("UTP Task is not implemented");
}

void Host::processUTPTransfer(uint64_t) {
  uint32_t bit = 0;

  for (int i = 0; i < 32; i++) {
    bit = 1 << i;

    if ((register_table.utrldbr & bit) && !(pendingInterrupt & bit)) {
      pendingInterrupt |= bit;

      lRequestQueue.push(i);
    }
  }
}

void Host::processUTPCommand(UTPTransferReqDesc &req, UTP_TRANSFER_CMD cmd,
                             DMAFunction &func, void *context) {
  struct UTPCommandContext {
    uint8_t *transferReq;
    uint8_t *transferResp;
    uint8_t *prdt;
    UPIU *transferReqUPIU;
    UPIU *transferRespUPIU;

    bool reqDone;
    bool respDone;
    bool prdtDone;

    uint64_t base;
    uint32_t offUPIUResp;
    uint32_t sizeUPIUResp;

    UTP_TRANSFER_CMD cmd;

    DMAFunction func;
    void *context;

    UTPCommandContext(DMAFunction &f, void *c)
        : transferReq(nullptr),
          transferResp(nullptr),
          prdt(nullptr),
          reqDone(false),
          respDone(false),
          prdtDone(false),
          func(f),
          context(c) {}
  };

  uint32_t offPRDT = req.prdtOffset * 4;
  uint32_t sizePRDT = req.prdtLength * sizeof(PRDT);

  UTPCommandContext *commandContext = new UTPCommandContext(func, context);

  DMAFunction doRequest = [this](uint64_t now, void *context) {
    DMAFunction done = [this](uint64_t, void *context) {
      DMAFunction doWrite = [](uint64_t now, void *context) {
        auto pContext = (UTPCommandContext *)context;
        auto pResult = (RequestContext *)pContext->context;

        // debugprint(LOG_HIL_UFS,
        //                    "COMMAND | UPIU Transfer Response | TT 0x%X",
        //                    transferRespUPIU->header.transactionType);

        // Write status to DW2
        pResult->transferReqDesc.dw2 = 0;

        // Cleanup
        free(pContext->transferReq);
        free(pContext->transferResp);
        if (pContext->prdt) {
          free(pContext->prdt);
        }

        delete pContext->transferReqUPIU;
        delete pContext->transferRespUPIU;

        pContext->func(now, pContext->context);

        delete pContext;
      };

      auto pContext = (UTPCommandContext *)context;

      // Transfer response
      pContext->transferRespUPIU->header.taskTag =
          pContext->transferReqUPIU->header.taskTag;
      pContext->transferRespUPIU->get(pContext->transferResp,
                                      pContext->sizeUPIUResp);
      axiFIFO->dmaWrite(pContext->base + pContext->offUPIUResp,
                        pContext->sizeUPIUResp, pContext->transferResp, doWrite,
                        context);
    };

    auto pContext = (UTPCommandContext *)context;
    auto pResult = (RequestContext *)pContext->context;

    // Create Request UPIU structure
    uint8_t reqType = pContext->transferReq[0] & 0x3F;
    pContext->transferReqUPIU = getUPIU((UPIU_OPCODE)reqType);
    pContext->transferReqUPIU->set(pContext->transferReq,
                                   pContext->offUPIUResp);

    // debugprint(LOG_HIL_UFS,
    //                    "COMMAND | UPIU Transfer Request | TT 0x%X", reqType);

    // Process
    switch (reqType) {
      case OPCODE_NOP_OUT:
        pContext->transferRespUPIU = getUPIU(OPCODE_NOP_IN);

        done(now, context);

        break;
      case OPCODE_QUERY_REQUEST:
        pContext->transferRespUPIU = getUPIU(OPCODE_QUERY_RESPONSE);
        pDevice->processQueryCommand(
            (UPIUQueryReq *)pContext->transferReqUPIU,
            (UPIUQueryResp *)pContext->transferRespUPIU);

        execute(CPU::UFS__DEVICE, CPU::PROCESS_QUERY_COMMAND, done, context);

        break;
      case OPCODE_COMMAND:
        pContext->transferRespUPIU = getUPIU(OPCODE_RESPONSE);

        {
          CPUContext *pCPU = new CPUContext(done, context, CPU::UFS__DEVICE,
                                            CPU::PROCESS_COMMAND);

          pDevice->processCommand(
              pContext->cmd, (UPIUCommand *)pContext->transferReqUPIU,
              (UPIUResponse *)pContext->transferRespUPIU, pContext->prdt,
              pResult->transferReqDesc.prdtLength, cpuHandler, pCPU);
        }

        break;
      default:
        panic("Unapplicable UPIU command type");

        break;
    }
  };
  DMAFunction doReqRead = [doRequest](uint64_t now, void *context) {
    auto pContext = (UTPCommandContext *)context;

    pContext->reqDone = true;

    if (pContext->prdtDone) {
      doRequest(now, context);
    }
  };

  commandContext->offUPIUResp = req.respUPIUOffset * 4;
  commandContext->sizeUPIUResp = req.respUPIULength * 4;
  commandContext->cmd = cmd;

  // Retrive UTP Command Descriptor
  commandContext->base = ((uint64_t)req.cmdAddressUpper << 32) | req.cmdAddress;
  commandContext->transferReq =
      (uint8_t *)calloc(commandContext->offUPIUResp, 1);
  commandContext->transferResp =
      (uint8_t *)calloc(commandContext->sizeUPIUResp, 1);

  axiFIFO->dmaRead(commandContext->base, commandContext->offUPIUResp,
                   commandContext->transferReq, doReqRead, commandContext);

  if (req.prdtLength > 0) {
    DMAFunction doPRDTRead = [doRequest](uint64_t now, void *context) {
      auto pContext = (UTPCommandContext *)context;

      pContext->prdtDone = true;

      if (pContext->reqDone) {
        doRequest(now, context);
      }
    };

    commandContext->prdt = (uint8_t *)calloc(sizePRDT, 1);

    axiFIFO->dmaRead(commandContext->base + offPRDT, sizePRDT,
                     commandContext->prdt, doPRDTRead, commandContext);
  }
  else {
    commandContext->prdtDone = true;
  }
}

UPIU *Host::getUPIU(UPIU_OPCODE code) {
  switch (code) {
    case OPCODE_COMMAND:
      return new UPIUCommand();
    case OPCODE_DATA_OUT:
      return new UPIUDataOut();
    case OPCODE_QUERY_REQUEST:
      return new UPIUQueryReq();
    case OPCODE_RESPONSE:
      return new UPIUResponse();
    case OPCODE_DATA_IN:
      return new UPIUDataIn();
    case OPCODE_READY_TO_TRANSFER:
      return new UPIUReadyToTransfer();
    case OPCODE_QUERY_RESPONSE:
      return new UPIUQueryResp();
    default:
      break;
  }

  UPIU *ret = new UPIU();
  ret->header.transactionType = code;

  return ret;
}

void Host::readRegister(uint32_t offset, uint32_t &data) {
  for (uint32_t i = 0; i < 4; i++) {
    if (offset + i < 0xB0) {
      data |= register_table.data[offset + i] << (i * 8);
    }
  }

  // debugprint(LOG_HIL_UFS,
  //                    "REG     | READ  | Offset 0x%02X | Data %08X", offset,
  //                    data);
}

void Host::writeRegister(uint32_t offset, uint32_t data, uint64_t tick) {
  // debugprint(LOG_HIL_UFS,
  //                    "REG     | WRITE | Offset 0x%02X | Data %08X", offset,
  //                    data);

  switch (offset) {
    case REG_IS:
      register_table.is &= (~data) & 0x00030FFF;

      pInterface->clearInterrupt();

      break;
    case REG_IE:
      register_table.ie &= 0x00000FFF;
      register_table.ie |= data & 0x00030FFF;  // Spec differs with Linux kernel

      break;
    case REG_HCS:
      register_table.hcs &= (~data) & 0x00000030;
      register_table.hcs |= data & 0x00000700;

      break;
    case REG_HCE:
      register_table.hce = data & 0x00000001;

      break;
    case REG_UTRIACR:
      register_table.utriacr = data & 0x81111FFF;

      break;
    case REG_UTRLBA:
      register_table.utrlba &= 0xFFFFFFFF00000000;
      register_table.utrlba |= data & 0xFFFFFC00;

      break;
    case REG_UTRLBAU:
      register_table.utrlba &= 0xFFFFFFFF;
      register_table.utrlba |= (uint64_t)data << 32;

      break;
    case REG_UTRLDBR:
      register_table.utrldbr |= data;

      processUTPTransfer(tick);

      break;
    case REG_UTRLCLR:
      register_table.utrlclr = data;
      // TODO: abort command

      break;
    case REG_UTRLRSR:
      register_table.utrlrsr = data & 0x00000001;

      break;
    case REG_UTMRLBA:
      register_table.utmrlba &= 0xFFFFFFFF00000000;
      register_table.utmrlba |= data & 0xFFFFFC00;

      break;
    case REG_UTMRLBAU:
      register_table.utmrlba &= 0xFFFFFFFF;
      register_table.utmrlba |= (uint64_t)data << 32;

      break;
    case REG_UTMRLDBR:
      register_table.utmrldbr |= data;

      processUTPTask();

      break;
    case REG_UTMRLCLR:
      register_table.utmrlclr = data;
      // TODO: abort command

      break;
    case REG_UTMRLRSR:
      register_table.utmrlrsr = data & 0x00000001;

      break;
    case REG_UICCMDR:
      register_table.uiccmdr = data & 0x000000FF;

      processUIC();

      break;
    case REG_UCMDARG1:
      register_table.ucmdarg1 = data;

      break;
    case REG_UCMDARG2:
      register_table.ucmdarg2 = data;

      break;
    case REG_UCMDARG3:
      register_table.ucmdarg3 = data;

      break;
    default:
      warn("Write to read only register 0x%X", offset);

      break;
  }
}

void Host::completion() {
  uint32_t count = 0;
  uint64_t tick = getTick();
  bool intr = false;

  while (lResponseQueue.size() > 0) {
    auto &iter = lResponseQueue.top();

    if (iter.finishedAt <= tick) {
      intr = true;

      // Clear doorbell
      register_table.utrldbr &= ~iter.bitmask;
      pendingInterrupt &= ~iter.bitmask;

      // Remove entry
      lResponseQueue.pop();
      count++;
    }
    else {
      break;
    }
  }

  if (intr) {
    debugprint(LOG_HIL_UFS, "INTR    | Completing %u requests", count);

    // Set Interrupt Status
    register_table.is |= UTP_TRANSFER_REQ_COMPL;

    // Post interrupt
    pInterface->generateInterrupt();
  }

  // Schedule for next interrupt
  if (lResponseQueue.size() > 0) {
    schedule(completionEvent, lResponseQueue.top().finishedAt);
  }
}

void Host::work() {
  lastWorkAt = getTick();

  // Queue is already collected. Just call handleRequest
  handleRequest();
}

void Host::handleRequest() {
  uint64_t tick = getTick();

  if (lRequestQueue.size() > 0) {
    RequestContext *reqContext = new RequestContext();
    reqContext->index = lRequestQueue.front();
    reqContext->bitMask = 1 << reqContext->index;

    lRequestQueue.pop();

    DMAFunction doRead = [this](uint64_t, void *context) {
      DMAFunction doRequest = [this](uint64_t, void *context) {
        DMAFunction doWrite = [this](uint64_t now, void *context) {
          auto *pContext = (RequestContext *)context;

          // Schedule interrupt
          lResponseQueue.push(
              Completion(now, pContext->bitMask,
                         pContext->transferReqDesc.dw0 & 0x01000000));
          schedule(completionEvent, lResponseQueue.top().finishedAt);

          stat.utpCommand++;

          delete pContext;
        };

        auto *pContext = (RequestContext *)context;

        // Write result to descriptor (DW2)
        uint64_t start = register_table.utrlba +
                         UTP_TRANSFER_REQ_DESC_SIZE * pContext->index;

        axiFIFO->dmaWrite(start, UTP_TRANSFER_REQ_DESC_SIZE,
                          pContext->transferReqDesc.data, doWrite, context);
      };

      auto *pContext = (RequestContext *)context;

      // Parse and process each commands
      uint8_t ct = pContext->transferReqDesc.dw0 >> 28;

      debugprint(LOG_HIL_UFS,
                 "COMMAND | UTP Transfer Request | Entry %d | CT %d",
                 pContext->index, ct);

      switch (ct) {
        case CMD_SCSI:
        case CMD_NATIVE_UFS_COMMAND:
        case CMD_DEVICE_MGMT_FUNCTION:
          processUTPCommand(pContext->transferReqDesc, (UTP_TRANSFER_CMD)ct,
                            doRequest, context);

          break;
        default:
          panic("Undefined UTP Transfer command type %#x", ct);

          break;
      }
    };

    // We have data at slot i
    uint64_t start =
        register_table.utrlba + UTP_TRANSFER_REQ_DESC_SIZE * reqContext->index;

    // Read description
    axiFIFO->dmaRead(start, UTP_TRANSFER_REQ_DESC_SIZE,
                     reqContext->transferReqDesc.data, doRead, reqContext);

    requestCounter++;
  }

  if (lRequestQueue.size() > 0 && requestCounter < maxRequest) {
    schedule(requestEvent, tick + requestInterval);
  }
  else {
    schedule(workEvent, MAX(tick + requestInterval, lastWorkAt + workInterval));
  }
}

UFSHCIRegister &Host::getRegisterTable() {
  return register_table;
}

void Host::getStatList(std::vector<Stats> &list, std::string prefix) {
  Stats temp;

  temp.name = prefix + "uic_count";
  temp.desc = "Total UIC Command handled";
  list.push_back(temp);

  temp.name = prefix + "utp_transfer_count";
  temp.desc = "Total UTP Transfer Command handled";
  list.push_back(temp);

  pDevice->getStatList(list, prefix);
}

void Host::getStatValues(std::vector<double> &values) {
  values.push_back(stat.uicCommand);
  values.push_back(stat.utpCommand);

  pDevice->getStatValues(values);
}

void Host::resetStatValues() {
  memset(&stat, 0, sizeof(stat));

  pDevice->resetStatValues();
}

}  // namespace UFS

}  // namespace HIL

}  // namespace SimpleSSD
