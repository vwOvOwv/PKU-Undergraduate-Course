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

#include "hil/sata/device.hh"

#include "util/algorithm.hh"

namespace SimpleSSD {

namespace HIL {

namespace SATA {

#define MAKE_LBA(req)                                                          \
  ((uint64_t)req.regH2D.lbaL[0] | ((uint64_t)req.regH2D.lbaL[1] << 8) |        \
   ((uint64_t)req.regH2D.lbaL[2] << 16))
#define MAKE_LBA_EXT(req)                                                      \
  (((uint64_t)req.regH2D.lbaH[0] << 24) |                                      \
   ((uint64_t)req.regH2D.lbaH[1] << 32) |                                      \
   ((uint64_t)req.regH2D.lbaH[2] << 40) | MAKE_LBA(req))

uint32_t makeCount(FIS &req) {
  uint32_t count = req.regH2D.countL;

  if (count == 0) {
    count = 0x100;
  }

  return count;
}

uint32_t makeCountExt(FIS &req) {
  uint32_t count = ((uint16_t)req.regH2D.countH << 8) | req.regH2D.countL;

  if (count == 0) {
    count = 0x10000;
  }

  return count;
}

Device::Device(HBA *p, DMAInterface *d, ConfigReader &c)
    : pParent(p), pDMA(d), pDisk(nullptr), conf(c) {
  pHIL = new HIL(c);
  pHIL->getLPNInfo(totalLogicalPages, logicalPageSize);

  lbaSize = conf.readUint(CONFIG_SATA, SATA_LBA_SIZE);

  // DMA Handler for PRDT
  dmaHandler = [](uint64_t now, void *context) {
    DMAContext *pContext = (DMAContext *)context;

    pContext->counter--;

    if (pContext->counter == 0) {
      pContext->function(now, pContext->context);
      delete pContext;
    }
  };
  writeDMASetup = [this](uint64_t tick, void *context) {
    _writeDMASetup(tick, context);
  };
  writeDMADone = [this](uint64_t tick, void *context) {
    _writeDMADone(tick, context);
  };
  readDMASetup = [this](uint64_t tick, void *context) {
    _readDMASetup(tick, context);
  };
  readDMADone = [this](uint64_t tick, void *context) {
    _readDMADone(tick, context);
  };

  if (conf.readBoolean(CONFIG_SATA, SATA_ENABLE_DISK_IMAGE)) {
    uint64_t diskSize;
    uint64_t expected;

    expected = totalLogicalPages * logicalPageSize;

    if (conf.readBoolean(CONFIG_SATA, SATA_USE_COW_DISK)) {
      pDisk = new CoWDisk();
    }
    else {
      pDisk = new Disk();
    }

    std::string filename = conf.readString(CONFIG_SATA, SATA_DISK_IMAGE_PATH);

    diskSize = pDisk->open(filename, expected,
                           conf.readUint(CONFIG_SATA, SATA_LBA_SIZE));

    if (diskSize == 0) {
      panic("Failed to open disk image");
    }
    else if (diskSize != expected) {
      if (conf.readBoolean(CONFIG_SATA, SATA_STRICT_DISK_SIZE)) {
        panic("Disk size not match");
      }
    }
  }
}

Device::~Device() {
  delete pHIL;
  delete pDisk;
}

void Device::init() {
  debugprint(LOG_HIL_SATA, "ATA     | COMRESET");

  // COMRESET received
  Completion resp;

  resp.fis.regD2H.type = FIS_TYPE_REG_D2H;
  resp.fis.regD2H.error = 0x01;
  resp.fis.regD2H.lbaL[0] = 0x01;
  resp.fis.regD2H.countL = 0x01;

  pParent->submitSignal(resp);
}

void Device::prdtRead(uint8_t *prdt, uint32_t prdtLength, uint32_t length,
                      uint8_t *buffer, DMAFunction &func, void *context) {
  DMAFunction doRead = [=](uint64_t, void *context) {
    DMAContext *pContext = (DMAContext *)context;

    PRDT *table = (PRDT *)prdt;
    uint64_t read = 0;

    for (uint32_t idx = 0; idx < prdtLength; idx++) {
      uint32_t size = (table[idx].dw3 & 0x3FFFF) + 1;

      size = MIN(size, length - read);

      pContext->counter++;
      pDMA->dmaRead(table[idx].dataBaseAddress, size, buffer + read, dmaHandler,
                    context);

      read += size;

      if (read >= length) {
        break;
      }
    }

    if (pContext->counter == 0) {
      pContext->counter = 1;

      dmaHandler(getTick(), context);
    }
  };

  DMAContext *pContext = new DMAContext(func, context);

  execute(CPU::SATA__DEVICE, CPU::PRDT_READ, doRead, pContext);
}

void Device::prdtWrite(uint8_t *prdt, uint32_t prdtLength, uint32_t length,
                       uint8_t *buffer, DMAFunction &func, void *context) {
  DMAFunction doWrite = [=](uint64_t, void *context) {
    DMAContext *pContext = (DMAContext *)context;

    PRDT *table = (PRDT *)prdt;
    uint64_t written = 0;

    for (uint32_t idx = 0; idx < prdtLength; idx++) {
      uint32_t size = (table[idx].dw3 & 0x3FFFF) + 1;

      size = MIN(size, length - written);

      pContext->counter++;
      pDMA->dmaWrite(table[idx].dataBaseAddress, size, buffer + written,
                     dmaHandler, context);

      written += size;

      if (written >= length) {
        break;
      }
    }

    if (pContext->counter == 0) {
      pContext->counter = 1;

      dmaHandler(getTick(), context);
    }
  };

  DMAContext *pContext = new DMAContext(func, context);

  execute(CPU::SATA__DEVICE, CPU::PRDT_WRITE, doWrite, pContext);
}

void Device::convertUnit(uint64_t slba, uint64_t nlblk, Request &req) {
  uint32_t lbaratio = logicalPageSize / lbaSize;
  uint64_t slpn;
  uint64_t nlp;
  uint64_t off;

  slpn = slba / lbaratio;
  off = slba % lbaratio;
  nlp = (nlblk + off + lbaratio - 1) / lbaratio;

  req.range.slpn = slpn;
  req.range.nlp = nlp;
  req.offset = off * lbaSize;
  req.length = nlblk * lbaSize;
}

void Device::read(uint64_t slba, uint64_t nlb, uint8_t *buffer,
                  DMAFunction &func, void *context) {
  Request *req = new Request(func, context);
  DMAFunction doRead = [this](uint64_t, void *context) {
    auto req = (Request *)context;

    pHIL->read(*req);

    delete req;
  };

  convertUnit(slba, nlb, *req);

  if (pDisk) {
    pDisk->read(slba, nlb, buffer);
  }

  execute(CPU::SATA__DEVICE, CPU::READ, doRead, req);
}

void Device::write(uint64_t slba, uint64_t nlb, uint8_t *buffer,
                   DMAFunction &func, void *context) {
  Request *req = new Request(func, context);
  DMAFunction doWrite = [this](uint64_t, void *context) {
    auto req = (Request *)context;

    pHIL->write(*req);

    delete req;
  };

  convertUnit(slba, nlb, *req);

  if (pDisk) {
    pDisk->write(slba, nlb, buffer);
  }

  execute(CPU::SATA__DEVICE, CPU::WRITE, doWrite, req);
}

void Device::flush(DMAFunction &func, void *context) {
  Request *req = new Request(func, context);
  DMAFunction doFlush = [this](uint64_t, void *context) {
    auto req = (Request *)context;

    pHIL->flush(*req);

    delete req;
  };

  req->range.slpn = 0;
  req->range.nlp = totalLogicalPages;
  req->offset = 0;
  req->length = totalLogicalPages * logicalPageSize;

  execute(CPU::SATA__DEVICE, CPU::FLUSH, doFlush, req);
}

void Device::_writeDMASetup(uint64_t, void *context) {
  NCQContext *pContext = (NCQContext *)context;

  debugprint(LOG_HIL_SATA, "ATA     | WRITE FPDMA QUEUED | Tag %d | Setup DMA",
             pContext->tag);

  Completion resp;

  resp.fis.dmaSetup.type = FIS_TYPE_DMA_SETUP;
  resp.fis.dmaSetup.flag = 0x00;  // DMA from Host to Device
  resp.fis.dmaSetup.reserved2[0] = pContext->tag;
  resp.fis.dmaSetup.transferCount =
      pContext->nlb * lbaSize;  // TODO: Unit correct?
  pContext->buffer = (uint8_t *)calloc(pContext->nlb, lbaSize);
  pContext->beginAt = getTick();

  pParent->submitFIS(resp);

  // Begin DMA
  CPUContext *pCPU = new CPUContext(writeDMADone, pContext, CPU::SATA__DEVICE,
                                    CPU::WRITE_DMA_SETUP);

  prdtRead(pContext->prdt, pContext->prdtLength, pContext->nlb * lbaSize,
           pContext->buffer, cpuHandler, pCPU);
}

void Device::_writeDMADone(uint64_t tick, void *context) {
  NCQContext *pContext = (NCQContext *)context;

  debugprint(LOG_HIL_SATA,
             "NVM     | WRITE FPDMA QUEUED | Tag %d | %" PRIu64
             " + %d | DMA %" PRIu64 " - %" PRIu64 " (%" PRIu64 ")",
             pContext->tag, pContext->slba, pContext->nlb, pContext->beginAt,
             tick, tick - pContext->beginAt);

  DMAFunction nandDone = [this](uint64_t tick, void *context) {
    NCQContext *pContext = (NCQContext *)context;

    debugprint(LOG_HIL_SATA,
               "NVM     | WRITE FPDMA QUEUED | Tag %d | %" PRIu64
               " + %d | NAND %" PRIu64 " - %" PRIu64 " (%" PRIu64 ")",
               pContext->tag, pContext->slba, pContext->nlb, pContext->tick,
               tick, tick - pContext->tick);

    Completion resp;

    resp.fis.sdb.type = FIS_TYPE_DEV_BITS;
    resp.fis.sdb.flag = 0x40;
    resp.fis.sdb.payload = (1 << pContext->tag);

    pParent->submitFIS(resp);
    pContext->release();
  };

  pContext->tick = tick;

  CPUContext *pCPU = new CPUContext(nandDone, pContext, CPU::SATA__DEVICE,
                                    CPU::WRITE_DMA_DONE);

  write(pContext->slba, pContext->nlb, pContext->buffer, cpuHandler, pCPU);
}

void Device::_readDMASetup(uint64_t tick, void *context) {
  NCQContext *pContext = (NCQContext *)context;

  debugprint(LOG_HIL_SATA,
             "ATA     | READ FPDMA QUEUED | Tag %d | %" PRIu64
             " + %d | NAND %" PRIu64 " - %" PRIu64 " (%" PRIu64 ")",
             pContext->tag, pContext->slba, pContext->nlb, pContext->beginAt,
             tick, tick - pContext->beginAt);

  Completion resp;

  resp.fis.dmaSetup.type = FIS_TYPE_DMA_SETUP;
  resp.fis.dmaSetup.flag = 0x20;  // DMA from Device to Host
  resp.fis.dmaSetup.reserved2[0] = pContext->tag;
  resp.fis.dmaSetup.transferCount =
      pContext->nlb * lbaSize;  // TODO: Unit correct?
  pContext->tick = tick;

  pParent->submitFIS(resp);

  // Begin DMA
  CPUContext *pCPU = new CPUContext(readDMADone, pContext, CPU::SATA__DEVICE,
                                    CPU::READ_DMA_SETUP);

  prdtWrite(pContext->prdt, pContext->prdtLength, pContext->nlb * lbaSize,
            pContext->buffer, cpuHandler, pCPU);
}

void Device::_readDMADone(uint64_t tick, void *context) {
  NCQContext *pContext = (NCQContext *)context;

  debugprint(LOG_HIL_SATA,
             "NVM     | READ FPDMA QUEUED | Tag %d | %" PRIu64
             " + %d | DMA %" PRIu64 " - %" PRIu64 " (%" PRIu64 ")",
             pContext->tag, pContext->slba, pContext->nlb, pContext->beginAt,
             tick, tick - pContext->beginAt);

  DMAFunction doSubmit = [this](uint64_t, void *context) {
    auto pContext = (NCQContext *)context;

    Completion resp;

    resp.fis.sdb.type = FIS_TYPE_DEV_BITS;
    resp.fis.sdb.flag = 0x40;
    resp.fis.sdb.payload = (1 << pContext->tag);

    pParent->submitFIS(resp);
    pContext->release();
  };

  execute(CPU::SATA__DEVICE, CPU::READ_DMA_DONE, doSubmit, pContext);
}

void Device::identifyDevice(CommandContext *cmd) {
  static uint16_t data[256];
  uint64_t sectors = totalLogicalPages / lbaSize * logicalPageSize;

  debugprint(LOG_HIL_SATA, "ATA     | IDENTIFY DEVICE");

  // Fill IDENTIFY DEVICE structure based on ACS-2
  {
    memset(data, 0, 512);

    // General configuration
    data[ATA_ID_CONFIG] = 0x0000;
    data[0x02] = 0xC837;

    strcpy((char *)(data + ATA_ID_SERNO), "00000000000000000000");
    strcpy((char *)(data + ATA_ID_FW_REV), "02.01.00");
    strcpy((char *)(data + ATA_ID_PROD), "SimpleSSD SATA Device by CAMELab");

    for (int i = 10; i < 47; i++) {
      data[i] = bswap16(data[i]);
    }

    data[ATA_ID_MAX_MULTSECT] = 0x8000;
    data[ATA_ID_CAPABILITY] = 0x0F00;
    data[ATA_ID_FIELD_VALID] = 0x0006;
    data[ATA_ID_MWDMA_MODES] = 0x0007;
    data[ATA_ID_PIO_MODES] = 0x0003;
    data[ATA_ID_EIDE_DMA_MIN] = 120;
    data[ATA_ID_EIDE_DMA_TIME] = 120;
    data[ATA_ID_EIDE_PIO] = 120;
    data[ATA_ID_EIDE_PIO_IORDY] = 120;
    data[ATA_ID_QUEUE_DEPTH] = 0x001F;
    data[ATA_ID_SATA_CAPABILITY] = 0x010E;    // Support 1.5/3/6Gbps + NCQ
    data[ATA_ID_SATA_CAPABILITY_2] = 0x0006;  // TODO: Set speed
    data[ATA_ID_FEATURE_SUPP] = 0x0000;
    data[ATA_ID_FEATURE_SUPP + 1] = 0x0000;
    data[ATA_ID_MAJOR_VER] = 0x0300;  // ACS-2 + ATA8-ACS
    data[ATA_ID_COMMAND_SET_1] = 0x0020;
    data[ATA_ID_COMMAND_SET_2] = 0x7400;  // FLUSH CACHE [EXT] and 48bit
    data[ATA_ID_CFSSE] = 0x4000;

    if (conf.readBoolean(CONFIG_ICL, ICL::ICL_USE_WRITE_CACHE)) {
      data[ATA_ID_CFS_ENABLE_1] = 0x0020;
    }
    else {
      data[ATA_ID_CFS_ENABLE_1] = 0x0000;
    }

    data[ATA_ID_CFS_ENABLE_2] = 0x2400;
    data[ATA_ID_CSF_DEFAULT] = 0x4000;
    data[ATA_ID_UDMA_MODES] = 0x003F;

    if (sectors < 0x0FFFFFFF) {
      *(uint32_t *)(data + ATA_ID_LBA_CAPACITY) = sectors;
    }
    else {
      *(uint32_t *)(data + ATA_ID_LBA_CAPACITY) = 0x0FFFFFFF;
    }
    *(uint64_t *)(data + ATA_ID_LBA_CAPACITY_2) = sectors;

    if (logicalPageSize > lbaSize) {
      data[ATA_ID_SECTOR_SIZE] |= 0x6000;
    }
    if (lbaSize > 512) {
      data[ATA_ID_SECTOR_SIZE] |= 0x5000;
    }
    if (lbaSize != logicalPageSize) {
      data[ATA_ID_SECTOR_SIZE] |=
          ((uint8_t)log2(logicalPageSize / lbaSize)) & 0x0F;
    }

    *(uint32_t *)(data + ATA_ID_LOGICAL_SECTOR_SIZE) = lbaSize / 2;
    data[ATA_ID_DATA_SET_MGMT] = 0x0001;

    data[0xDE] = 0x1020;  // Transport major version: SATA Rev 3.0
  }

  DMAFunction doWrite = [this](uint64_t, void *context) {
    auto pContext = (CommandContext *)context;

    Completion resp;

    resp.slotIndex = pContext->slotIndex;
    resp.maskIS |= PORT_IRQ_SG_DONE;
    resp.fis.regD2H.type = FIS_TYPE_REG_D2H;

    pParent->submitFIS(resp);
    pContext->release();
  };

  prdtWrite(cmd->prdt, cmd->prdtLength, 512, (uint8_t *)data, doWrite, cmd);
}

void Device::setMode(CommandContext *cmd) {
  uint8_t fid = cmd->request.regH2D.featureL;
  uint8_t id = cmd->request.regH2D.countL;

  switch (fid) {
    case FEATURE_SET_XFER_MODE:
      debugprint(LOG_HIL_SATA, "ATA     | SET MODE | Set Transfer Mode");

      switch (id & 0xF8) {
        case 0:
          if (id & 0x01) {
            debugprint(LOG_HIL_SATA,
                       "ATA     | SET MODE | PIO default mode, disable IORDY");
          }
          else {
            debugprint(LOG_HIL_SATA, "ATA     | SET MODE | PIO default mode");
          }

          break;
        case 1:
          debugprint(LOG_HIL_SATA, "ATA     | SET MODE | PIO%d", id & 0x07);
          break;
        case 4:
          debugprint(LOG_HIL_SATA, "ATA     | SET MODE | MWDMA%d", id & 0x07);
          break;
        case 8:
          debugprint(LOG_HIL_SATA, "ATA     | SET MODE | UDMA%d", id & 0x07);
          break;
      }

      break;
    default:
      debugprint(LOG_HIL_SATA, "ATA     | SET MODE | Not supported feature");
      break;
  }

  Completion resp;

  resp.slotIndex = cmd->slotIndex;
  resp.fis.regD2H.type = FIS_TYPE_REG_D2H;

  pParent->submitFIS(resp);
  cmd->release();
}

void Device::readVerify(CommandContext *cmd) {
  Request reqInternal;
  uint64_t slba;
  uint32_t nlb;

  if (cmd->request.regH2D.command == OPCODE_READ_VERIFY_SECTOR) {
    debugprint(LOG_HIL_SATA, "ATA     | READ VERIFY");

    slba = MAKE_LBA(cmd->request);
    nlb = makeCount(cmd->request);
  }
  else {
    debugprint(LOG_HIL_SATA, "ATA     | READ VERIFY EXT");

    slba = MAKE_LBA_EXT(cmd->request);
    nlb = makeCountExt(cmd->request);
  }

  convertUnit(slba, nlb, reqInternal);

  // Check range
  Completion resp;

  resp.slotIndex = cmd->slotIndex;
  resp.fis.regD2H.type = FIS_TYPE_REG_D2H;

  if (reqInternal.range.nlp >= totalLogicalPages) {
    resp.fis.regD2H.status = ATA_ERR;
    resp.fis.regD2H.error = ATA_ABORTED;
  }

  pParent->submitFIS(resp);
  cmd->release();
}

void Device::readDMA(CommandContext *cmd, bool isPIO) {
  uint64_t slba;
  uint32_t nlb;

  if (cmd->request.regH2D.command == OPCODE_READ_SECTOR ||
      cmd->request.regH2D.command == OPCODE_READ_DMA) {
    slba = MAKE_LBA(cmd->request);
    nlb = makeCount(cmd->request);

    debugprint(LOG_HIL_SATA, "ATA     | %s | %" PRIu64 " + %d",
               isPIO ? "READ SECTOR" : "READ DMA", slba, nlb);
  }
  else {
    slba = MAKE_LBA_EXT(cmd->request);
    nlb = makeCountExt(cmd->request);

    debugprint(LOG_HIL_SATA, "ATA     | %s | %" PRIu64 " + %d",
               isPIO ? "READ SECTOR EXT" : "READ DMA EXT", slba, nlb);
  }

  DMAFunction doRead = [this](uint64_t tick, void *context) {
    DMAFunction doWrite = [this](uint64_t tick, void *context) {
      IOContext *pContext = (IOContext *)context;

      debugprint(LOG_HIL_SATA,
                 "ATA     | READ  | %" PRIu64 " + %d | DMA %" PRIu64
                 " - %" PRIu64 " (%" PRIu64 ")",
                 pContext->slba, pContext->nlb, pContext->tick, tick,
                 tick - pContext->tick);

      Completion resp;

      resp.slotIndex = pContext->cmd->slotIndex;
      resp.maskIS |= PORT_IRQ_SG_DONE;
      resp.fis.regD2H.type = FIS_TYPE_REG_D2H;

      pParent->submitFIS(resp);

      pContext->cmd->release();
      free(pContext->buffer);
      delete pContext;
    };

    IOContext *pContext = (IOContext *)context;

    debugprint(LOG_HIL_SATA,
               "ATA     | READ  | %" PRIu64 " + %d | NAND %" PRIu64
               " - %" PRIu64 " (%" PRIu64 ")",
               pContext->slba, pContext->nlb, pContext->beginAt, tick,
               tick - pContext->beginAt);

    pContext->tick = tick;

    prdtWrite(pContext->cmd->prdt, pContext->cmd->prdtLength,
              pContext->nlb * lbaSize, pContext->buffer, doWrite, context);
  };

  IOContext *pContext = new IOContext(cmd);

  pContext->slba = slba;
  pContext->nlb = nlb;
  pContext->buffer = (uint8_t *)calloc(nlb, lbaSize);

  CPUContext *pCPU =
      new CPUContext(doRead, pContext, CPU::SATA__DEVICE, CPU::READ_DMA);

  read(slba, nlb, pContext->buffer, cpuHandler, pCPU);
}

void Device::readNCQ(CommandContext *cmd) {
  uint64_t slba = MAKE_LBA_EXT(cmd->request);
  uint32_t nlb = (cmd->request.regH2D.featureL |
                  ((uint32_t)cmd->request.regH2D.featureH << 8));
  uint8_t tag = (cmd->request.regH2D.countL >> 3) & 0x1F;

  if (nlb == 0) {
    nlb = 0x10000;
  }

  debugprint(LOG_HIL_SATA,
             "ATA     | READ FPDMA QUEUED | Tag %d | %" PRIu64 " + %d", tag,
             slba, nlb);

  // Schedule read
  NCQContext *pContext = new NCQContext(cmd);

  pContext->slba = slba;
  pContext->nlb = nlb;
  pContext->tag = tag;
  pContext->buffer = (uint8_t *)calloc(pContext->nlb, lbaSize);

  read(slba, nlb, pContext->buffer, readDMASetup, pContext);

  // Send read scheduled
  DMAFunction doSubmit = [this](uint64_t, void *context) {
    auto pContext = (CommandContext *)context;

    Completion resp;

    resp.slotIndex = pContext->slotIndex;
    resp.fis.regD2H.type = FIS_TYPE_REG_D2H;

    pParent->submitFIS(resp);
    pContext->release();
  };

  execute(CPU::SATA__DEVICE, CPU::READ_NCQ, doSubmit, cmd);
}

void Device::writeDMA(CommandContext *cmd, bool isPIO) {
  uint64_t slba;
  uint32_t nlb;

  if (cmd->request.regH2D.command == OPCODE_WRITE_SECTOR ||
      cmd->request.regH2D.command == OPCODE_WRITE_DMA) {
    slba = MAKE_LBA(cmd->request);
    nlb = makeCount(cmd->request);

    debugprint(LOG_HIL_SATA, "ATA     | %s | %" PRIu64 " + %d",
               isPIO ? "WRITE SECTOR" : "WRITE DMA", slba, nlb);
  }
  else {
    slba = MAKE_LBA_EXT(cmd->request);
    nlb = makeCountExt(cmd->request);

    debugprint(LOG_HIL_SATA, "ATA     | %s | %" PRIu64 " + %d",
               isPIO ? "WRITE SECTOR EXT" : "WRITE DMA EXT", slba, nlb);
  }

  DMAFunction doRead = [this](uint64_t tick, void *context) {
    DMAFunction doWrite = [this](uint64_t tick, void *context) {
      IOContext *pContext = (IOContext *)context;

      debugprint(LOG_HIL_SATA,
                 "ATA     | WRITE | %" PRIu64 " + %d | NAND %" PRIu64
                 " - %" PRIu64 " (%" PRIu64 ")",
                 pContext->slba, pContext->nlb, pContext->tick, tick,
                 tick - pContext->tick);

      Completion resp;

      resp.slotIndex = pContext->cmd->slotIndex;
      resp.maskIS |= PORT_IRQ_SG_DONE;
      resp.fis.regD2H.type = FIS_TYPE_REG_D2H;

      pParent->submitFIS(resp);

      pContext->cmd->release();
      free(pContext->buffer);
      delete pContext;
    };

    IOContext *pContext = (IOContext *)context;

    debugprint(LOG_HIL_SATA,
               "ATA     | WRITE | %" PRIu64 " + %d | DMA %" PRIu64 " - %" PRIu64
               " (%" PRIu64 ")",
               pContext->slba, pContext->nlb, pContext->beginAt, tick,
               tick - pContext->beginAt);

    pContext->tick = tick;

    write(pContext->slba, pContext->nlb, pContext->buffer, doWrite, context);
  };

  IOContext *pContext = new IOContext(cmd);

  pContext->slba = slba;
  pContext->nlb = nlb;
  pContext->buffer = (uint8_t *)calloc(nlb, lbaSize);

  CPUContext *pCPU =
      new CPUContext(doRead, pContext, CPU::SATA__DEVICE, CPU::WRITE_DMA);

  prdtRead(pContext->cmd->prdt, pContext->cmd->prdtLength, nlb * lbaSize,
           pContext->buffer, cpuHandler, pCPU);
}

void Device::writeNCQ(CommandContext *cmd) {
  struct Wrapper {
    CommandContext *cmd;
    NCQContext *ncq;
  };

  uint64_t slba = MAKE_LBA_EXT(cmd->request);
  uint32_t nlb = (cmd->request.regH2D.featureL |
                  ((uint32_t)cmd->request.regH2D.featureH << 8));
  uint8_t tag = (cmd->request.regH2D.countL >> 3) & 0x1F;

  if (nlb == 0) {
    nlb = 0x10000;
  }

  debugprint(LOG_HIL_SATA,
             "ATA     | WRITE FPDMA QUEUED | Tag %d | %" PRIu64 " + %d", tag,
             slba, nlb);

  // Schedule write
  NCQContext *pContext = new NCQContext(cmd);

  pContext->slba = slba;
  pContext->nlb = nlb;
  pContext->tag = tag;

  // Send write scheduled
  DMAFunction doSubmit = [this](uint64_t, void *context) {
    auto pContext = (Wrapper *)context;

    Completion resp(writeDMASetup, pContext->ncq);

    resp.slotIndex = pContext->cmd->slotIndex;
    resp.fis.regD2H.type = FIS_TYPE_REG_D2H;

    pParent->submitFIS(resp);
    pContext->cmd->release();

    delete pContext;
  };

  Wrapper *wrapper = new Wrapper();
  wrapper->cmd = cmd;
  wrapper->ncq = pContext;

  execute(CPU::SATA__DEVICE, CPU::WRITE_NCQ, doSubmit, wrapper);
}

void Device::flushCache(CommandContext *cmd) {
  if (cmd->request.regH2D.command == OPCODE_READ_VERIFY_SECTOR) {
    debugprint(LOG_HIL_SATA, "ATA     | FLUSH CACHE");
  }
  else {
    debugprint(LOG_HIL_SATA, "ATA     | FLUSH CACHE EXT");
  }

  DMAFunction doFlush = [this](uint64_t, void *context) {
    auto pContext = (CommandContext *)context;

    Completion resp;

    resp.slotIndex = pContext->slotIndex;
    resp.fis.regD2H.type = FIS_TYPE_REG_D2H;

    pParent->submitFIS(resp);
    pContext->release();
  };

  flush(doFlush, cmd);
}

void Device::submitCommand(RequestContext *req) {
  DMAFunction doRequest = [this](uint64_t, void *context) {
    auto pContext = (CommandContext *)context;

    bool processed = true;

    // Just use Register H2D because we are printing header only
    debugprint(LOG_HIL_SATA, "QUEUE   | FIS Type %02Xh | Command %02Xh",
               pContext->request.regH2D.type, pContext->request.regH2D.command);

    switch (pContext->request.regH2D.command) {
      case OPCODE_FLUSH_CACHE:
      case OPCODE_FLUSH_CACHE_EXT:
        flushCache(pContext);
        break;
      case OPCODE_IDENTIFY_DEVICE:
        identifyDevice(pContext);
        break;
      case OPCODE_READ_DMA:
      case OPCODE_READ_DMA_EXT:
        readDMA(pContext);
        break;
      case OPCODE_READ_FPDMA_QUEUED:
        readNCQ(pContext);
        break;
      case OPCODE_READ_SECTOR:
      case OPCODE_READ_SECTOR_EXT:
        readDMA(pContext, true);
        break;
      case OPCODE_READ_VERIFY_SECTOR:
      case OPCODE_READ_VERIFY_SECTOR_EXT:
        readVerify(pContext);
        break;
      case OPCODE_SET_FEATURE:
        setMode(pContext);
        break;
      case OPCODE_WRITE_DMA:
      case OPCODE_WRITE_DMA_EXT:
        writeDMA(pContext);
        break;
      case OPCODE_WRITE_FPDMA_QUEUED:
        writeNCQ(pContext);
        break;
      case OPCODE_WRITE_SECTOR:
      case OPCODE_WRITE_SECTOR_EXT:
        writeDMA(pContext, true);
        break;
      default:
        processed = false;
        break;
    }

    if (!processed) {
      Completion resp;

      resp.slotIndex = pContext->slotIndex;
      resp.fis.regD2H.type = FIS_TYPE_REG_D2H;  // Type
      resp.fis.regD2H.status = 0x01;            // Status (Error)
      resp.fis.regD2H.error = 0x04;             // Error (Abort)

      pParent->submitFIS(resp);
      pContext->release();
    }
  };

  CommandContext *pContext = new CommandContext();

  pContext->slotIndex = req->idx;

  // Read Command Table from Command List
  DMAFunction doReqRead = [doRequest](uint64_t now, void *context) {
    auto pContext = (CommandContext *)context;

    pContext->reqDone = true;

    if (pContext->prdtDone) {
      doRequest(now, context);
    }
  };

  pDMA->dmaRead(req->header.commandTableBaseAddress, 64, pContext->request.data,
                doReqRead, pContext);

  // We have PRDT
  if (req->header.prdtLength > 0) {
    DMAFunction doPRDTRead = [doRequest](uint64_t now, void *context) {
      auto pContext = (CommandContext *)context;

      pContext->prdtDone = true;

      if (pContext->reqDone) {
        doRequest(now, context);
      }
    };

    pContext->prdtLength = req->header.prdtLength;
    pContext->prdt = (uint8_t *)calloc(req->header.prdtLength, sizeof(PRDT));

    pDMA->dmaRead(req->header.commandTableBaseAddress + 0x80,
                  pContext->prdtLength * sizeof(PRDT), pContext->prdt,
                  doPRDTRead, pContext);
  }
  else {
    pContext->prdtDone = true;
  }
}

}  // namespace SATA

}  // namespace HIL

}  // namespace SimpleSSD
