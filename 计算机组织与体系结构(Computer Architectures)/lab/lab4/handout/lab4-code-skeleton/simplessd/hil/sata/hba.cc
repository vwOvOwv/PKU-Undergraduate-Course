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

#include "hil/sata/hba.hh"

#include "hil/sata/device.hh"
#include "util/fifo.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace HIL {

namespace SATA {

Completion::Completion()
    : slotIndex(0), maskIS(0), func([](uint64_t, void *) {}), context(nullptr) {
  memset(fis.data, 0, 64);
}

Completion::Completion(DMAFunction &f, void *c)
    : slotIndex(0), maskIS(0), func(f), context(c) {
  memset(fis.data, 0, 64);
}

HBA::HBA(Interface *i, ConfigReader &c) : pInterface(i), conf(c) {
  ARM::AXI::BUS_WIDTH hostBusWidth;
  SimpleSSD::SATA::SATA_GEN sataMode;
  uint64_t hostBusClock;

  // Get AXI setting
  hostBusWidth =
      (ARM::AXI::BUS_WIDTH)conf.readInt(CONFIG_SATA, SATA_AXI_BUS_WIDTH);
  hostBusClock = conf.readUint(CONFIG_SATA, SATA_AXI_CLOCK);
  sataMode = (SimpleSSD::SATA::SATA_GEN)conf.readInt(CONFIG_SATA, SATA_MODE);

  memset(ghc.data, 0, 0x100);
  memset(port.data, 0, 0x80);

  ghc.capability = 0xC0341F00;  // NCQ with 32 entry, one port
  ghc.globalHBAControl = 0x80000000;
  ghc.portsImplemented = 0x00000001;  // Bit 0 (Port 0)
  ghc.AHCIVersion = 0x00010301;       // AHCI version 1.3.1
  ghc.commandCompletionCoalescingControl = 0x00010100;
  ghc.enclosureManagementLocation = 0x00280020;  // 0xA0 + 0x20
  ghc.enclosureManagementControl = 0x03000000;
  ghc.HBACapabilityExtended = 0x00000020;  // BOHC not implemented

  port.commandAndStatus = 0x00000006;
  port.taskFileData = 0x0000007F;
  port.signature = 0xFFFFFFFF;

  workEvent = allocate([this](uint64_t) { work(); });
  requestEvent = allocate([this](uint64_t) { handleRequest(); });

  maxRequest = conf.readUint(CONFIG_SATA, SATA_MAX_REQUEST_COUNT);
  workInterval = conf.readUint(CONFIG_SATA, SATA_WORK_INTERVAL);
  requestInterval = workInterval / maxRequest;
  requestCounter = 0;

  submitFISPending = false;

  // FIFO
  FIFOParam fifoParam;

  fifoParam.rqSize = 8192;
  fifoParam.wqSize = 8192;
  fifoParam.transferUnit = 2048;
  fifoParam.latency = [hostBusWidth, hostBusClock](uint64_t size) -> uint64_t {
    return ARM::AXI::Stream::calculateDelay(hostBusClock, hostBusWidth, size);
  };

  pHostDMA = new FIFO(pInterface, fifoParam);

  fifoParam.latency = [sataMode](uint64_t size) -> uint64_t {
    return SimpleSSD::SATA::calculateDelay(sataMode, size);
  };

  pPHY = new FIFO(pHostDMA, fifoParam);

  fifoParam.latency = [hostBusWidth, hostBusClock](uint64_t size) -> uint64_t {
    return ARM::AXI::Stream::calculateDelay(hostBusClock, hostBusWidth, size);
  };

  pDeviceDMA = new FIFO(pPHY, fifoParam);

  pDevice = new Device(this, pDeviceDMA, conf);
}

HBA::~HBA() {
  delete pDevice;
  delete pDeviceDMA;
  delete pPHY;
  delete pHostDMA;
}

void HBA::init() {
  ghc.globalHBAControl = HOST_AHCI_EN;
  ghc.interruptStatus = 0x00000000;

  // Reset is done
  ghc.globalHBAControl &= ~HOST_RESET;

  // Port
  deviceInited = false;
}

void HBA::readAHCIRegister(uint32_t offset, uint32_t size, uint8_t *buffer) {
  uint64_t temp = 0;

  for (uint32_t i = 0; i < size; i++) {
    if (offset + i < 0x100) {
      buffer[i] = ghc.data[offset + i];
    }
    else if (offset + i < 0x180) {
      buffer[i] = port.data[offset + i - 0x100];
    }
  }

  if (size <= 8) {
    memcpy(&temp, buffer, size);
  }
  else {
    panic("Invalid register access size");
  }

  // debugprint(LOG_HIL_SATA, "REG     | READ  | %02Xh + %d | %08" PRIX64,
  // offset, size, temp);
}

void HBA::writeAHCIRegister(uint32_t offset, uint32_t size, uint8_t *buffer) {
  uint32_t temp = 0;

  if (size <= 8) {
    memcpy(&temp, buffer, 4);
  }
  else {
    panic("Invalid register access size");
  }

  // debugprint(LOG_HIL_SATA, "REG     | WRITE | %02Xh + %d | %08X", offset,
  // size, temp);

  // Access to AHCI Generic Host Control Register
  if (offset < 0x100) {
    switch (offset) {
      case REG_GHC:
        ghc.globalHBAControl &= 0xFFFFFFFC;
        ghc.globalHBAControl |= temp & 0x00000003;

        if (ghc.globalHBAControl & HOST_RESET) {
          // HBA Reset
          init();
        }

        break;
      case REG_IS:
        ghc.interruptStatus &= ~temp;

        break;
      case REG_CCC_CTL:
        ghc.commandCompletionCoalescingControl &= 0x000000FE;
        ghc.commandCompletionCoalescingControl |= temp & 0xFFFFFF01;

        break;
      case REG_CCC_PORTS:
        ghc.commandCompletionCoalescingPorts = temp;

        break;
      case REG_EM_CTL:
        ghc.enclosureManagementControl &= 0xFFFFFCFE | ((~temp) & 0x00000001);
        ghc.enclosureManagementControl |= temp & 0x00000300;

        if (ghc.enclosureManagementControl & EM_CTL_RST) {
          // Reset EM message logic
          ghc.enclosureManagementControl &= ~EM_CTL_RST;
        }

        if (ghc.enclosureManagementControl & EM_CTL_TM) {
          // Transmit message
          ghc.enclosureManagementControl &= ~EM_CTL_TM;
        }

        break;
      case REG_BOHC:
        ghc.handoffControlAndStatus &= 0xFFFFFFE0 | ((~temp) & 0x00000008);
        ghc.handoffControlAndStatus |= temp & 0x00000017;

        break;
    }
  }
  // Access to Port 0 Register
  else if (offset < 0x180) {
    offset -= 0x100;

    switch (offset) {
      case REG_P0CLB:
        port.commandListBaseAddress &= 0xFFFFFFFF00000000;
        port.commandListBaseAddress |= temp & 0xFFFFFC00;

        break;
      case REG_P0CLBU:
        port.commandListBaseAddress &= 0xFFFFFFFF;
        port.commandListBaseAddress |= (uint64_t)temp << 32;

        break;
      case REG_P0FB:
        port.FISBaseAddress &= 0xFFFFFFFF00000000;
        port.FISBaseAddress |= temp & 0xFFFFFF00;

        break;
      case REG_P0FBU:
        port.FISBaseAddress &= 0xFFFFFFFF;
        port.FISBaseAddress |= (uint64_t)temp << 32;

        break;
      case REG_P0IS:
        port.interruptStatus &= (~temp) & 0x037FFF50;

        interruptCleared();

        break;
      case REG_P0IE:
        port.interruptEnable &= 0x823FFF80;
        port.interruptEnable |= temp & 0x7DC0007F;

        updateInterrupt();

        break;
      case REG_P0CMD:
        port.commandAndStatus &= 0x0CFFFFE6 | ((~temp) & 0x00000008);
        port.commandAndStatus |= temp & 0xF3000011;

        if (port.commandAndStatus & PORT_CMD_START) {
          if (!scheduled(workEvent) && !scheduled(requestEvent)) {
            // Enable controller
            schedule(workEvent, getTick() + workInterval);

            port.commandAndStatus |= PORT_CMD_LIST_ON;
          }
        }
        else {
          // Disable controller
          deschedule(workEvent);

          port.commandAndStatus &= ~PORT_CMD_LIST_ON;
        }

        // Set FR
        if (port.commandAndStatus & PORT_CMD_FIS_RX) {
          port.commandAndStatus |= PORT_CMD_FIS_ON;
        }
        else {
          port.commandAndStatus &= ~PORT_CMD_FIS_ON;
        }

        break;
      case REG_P0SCTL:
        port.control &= 0xFFFFF000;
        port.control |= temp & 0x00000FFF;

        if ((port.control & 0x0F) == 0x01 && !deviceInited) {
          deviceInited = true;

          pDevice->init();
        }

        break;
      case REG_P0SERR:
        port.error &= ~temp;

        break;
      case REG_P0SACT:
        port.active |= temp;

        break;
      case REG_P0CI: {
        uint32_t old = port.commandIssue;
        port.commandIssue |= temp;

        processCommand(old ^ port.commandIssue);

      } break;
      case REG_P0SNTF:
        port.notification &= 0xFFFF0000 | ((~temp) & 0x0000FFFF);

        break;
      case REG_P0FBS:
        port.switchingControl &= 0xFFFFF0FC | ((~temp) & 0x00000002);
        port.switchingControl |= temp & 0x00000F03;

        break;
      default:
        warn("Write to read only register 0x%X", offset);

        break;
    }
  }

  if (size > 4) {
    writeAHCIRegister(offset + 4, size - 4, buffer + 4);
  }
}

void HBA::updateInterrupt() {
  if (ghc.globalHBAControl & HOST_IRQ_EN) {
    if ((ghc.interruptStatus & 0x01) &&
        (port.interruptStatus & port.interruptEnable)) {
      pInterface->updateInterrupt(true);
    }
  }
}

void HBA::processCommand(uint32_t ci) {
  uint32_t bit = 0;

  for (int i = 0; i < 32; i++) {
    bit = 1 << i;

    if (ci & bit) {
      lRequestQueue.push(i);
    }
  }
}

void HBA::work() {
  lastWorkAt = getTick();

  handleRequest();
}

void HBA::handleRequest() {
  uint64_t tick = getTick();

  if (lRequestQueue.size() > 0) {
    auto pContext = new RequestContext();

    pContext->idx = lRequestQueue.front();
    lRequestQueue.pop();

    DMAFunction doRead = [this](uint64_t, void *context) {
      auto pContext = (RequestContext *)context;

      debugprint(LOG_HIL_SATA, "QUEUE   | Entry %d", pContext->idx);
      pDevice->submitCommand(pContext);

      delete pContext;
    };

    uint64_t addr =
        port.commandListBaseAddress + pContext->idx * sizeof(CommandHeader);

    CPUContext *pCPU = new CPUContext(doRead, pContext, CPU::SATA__DEVICE,
                                      CPU::SUBMIT_COMMAND);

    pHostDMA->dmaRead(addr, sizeof(CommandHeader), pContext->header.data,
                      cpuHandler, pCPU);
  }

  if (lRequestQueue.size() > 0 && requestCounter < maxRequest) {
    schedule(requestEvent, tick + requestInterval);
  }
  else {
    schedule(workEvent, MAX(tick + requestInterval, lastWorkAt + workInterval));
  }
}

void HBA::submitFIS(Completion &resp) {
  switch (resp.fis.data[0]) {
    case FIS_TYPE_DMA_SETUP:
      resp.maskIS |= PORT_IRQ_DMAS_FIS;
      break;
    case FIS_TYPE_PIO_SETUP:
      resp.maskIS |= PORT_IRQ_PIOS_FIS;
      break;
    case FIS_TYPE_REG_D2H:
      resp.maskIS |= PORT_IRQ_D2H_REG_FIS;
      break;
    case FIS_TYPE_DEV_BITS:
      resp.maskIS |= PORT_IRQ_SDB_FIS;
      break;
    default:
      resp.maskIS |= PORT_IRQ_UNK_FIS;
      break;
  }

  if (resp.fis.data[2] & ATA_ERR) {
    resp.maskIS |= PORT_IRQ_TF_ERR;
  }

  if (resp.fis.data[0] == FIS_TYPE_DEV_BITS) {
    // Reset PxSACT
    port.active &= ~resp.fis.sdb.payload;
  }

  lResponseQueue.push(resp);
  handleResponse();
}

void HBA::submitSignal(Completion &resp) {
  if (resp.fis.data[0] != FIS_TYPE_REG_D2H) {
    panic("Invalid FIS to submitSignal");
  }

  resp.maskIS = PORT_IRQ_TF_ERR | PORT_IRQ_D2H_REG_FIS;

  // Set PxSIG
  port.signature = 0x00000101;  // SATA drive

  // Set PxSSTS
  // TODO: set interface speed
  port.status = 0x00000133;

  lResponseQueue.push(resp);

  DMAFunction doCompletion = [this](uint64_t, void *context) {
    uint32_t mask = (uint64_t)context;

    // Set interrupt mask
    port.interruptStatus |= mask;
    ghc.interruptStatus |= 0x01;  // Port 0 has interrupt

    pInterface->updateInterrupt(true);
  };

  pHostDMA->dmaWrite(port.FISBaseAddress + 0x40, 0x14, resp.fis.data,
                     doCompletion, (void *)(uint64_t)resp.maskIS);
}

void HBA::handleResponse() {
  auto &iter = lResponseQueue.front();

  // There is ongoing response
  if (submitFISPending) {
    return;
  }

  // Write FIS entry
  uint64_t base = port.FISBaseAddress;
  uint64_t size;

  switch (iter.fis.data[0]) {
    case FIS_TYPE_DMA_SETUP:
      size = 0x1C;
      break;
    case FIS_TYPE_PIO_SETUP:
      base += 0x20;
      size = 0x14;
      break;
    case FIS_TYPE_REG_D2H:
      base += 0x40;
      size = 0x14;
      break;
    case FIS_TYPE_DEV_BITS:
      base += 0x58;
      size = 0x08;
      break;
    default:
      base += 0x60;
      size = 64;
      break;
  }

  DMAFunction doCompletion = [this](uint64_t, void *context) {
    uint32_t mask = (uint64_t)context;

    // Set interrupt mask
    port.interruptStatus |= mask;
    ghc.interruptStatus |= 0x01;  // Port 0 has interrupt

    updateInterrupt();
  };

  debugprint(LOG_HIL_SATA, "QUEUE   | submitting FIS for entry %d",
             iter.slotIndex);

  pHostDMA->dmaWrite(base, size, iter.fis.data, doCompletion,
                     (void *)(uint64_t)iter.maskIS);
}

void HBA::interruptCleared() {
  if (lResponseQueue.size() > 0) {
    auto &iter = lResponseQueue.front();

    // Interrupt cleared
    if (!(port.interruptStatus & iter.maskIS)) {
      debugprint(LOG_HIL_SATA, "QUEUE   | FIS completed for entry %d",
                 iter.slotIndex);

      pInterface->updateInterrupt(false);

      // Copy function
      auto func = iter.func;
      auto context = iter.context;

      // Clear PxCI
      port.commandIssue &= ~((uint32_t)1 << iter.slotIndex);

      // Pop
      submitFISPending = false;
      lResponseQueue.pop();

      if (lResponseQueue.size() > 0) {
        handleResponse();
      }

      func(getTick(), context);
    }
  }
}

}  // namespace SATA

}  // namespace HIL

}  // namespace SimpleSSD
