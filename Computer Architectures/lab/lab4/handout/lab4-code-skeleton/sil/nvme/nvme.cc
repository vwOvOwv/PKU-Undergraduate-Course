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

#include "sil/nvme/nvme.hh"

#include "simplessd/hil/nvme/controller.hh"
#include "simplessd/hil/nvme/def.hh"
#include "simplessd/util/algorithm.hh"

namespace SIL {

namespace NVMe {

Driver::Driver(Engine &e, SimpleSSD::ConfigReader &conf)
    : BIL::DriverInterface(e),
      dmaReadPending(false),
      dmaWritePending(false),
      phase(true),
      adminSQ(nullptr),
      adminCQ(nullptr),
      ioSQ(nullptr),
      ioCQ(nullptr) {
  pcieGen = (SimpleSSD::PCIExpress::PCIE_GEN)conf.readInt(
      SimpleSSD::CONFIG_NVME, SimpleSSD::HIL::NVMe::NVME_PCIE_GEN);
  pcieLane = (uint8_t)conf.readUint(SimpleSSD::CONFIG_NVME,
                                    SimpleSSD::HIL::NVMe::NVME_PCIE_LANE);

  pController = new SimpleSSD::HIL::NVMe::Controller(this, conf);

  dmaReadEvent = engine.allocateEvent([this](uint64_t) { dmaReadDone(); });
  dmaWriteEvent = engine.allocateEvent([this](uint64_t) { dmaWriteDone(); });
}

Driver::~Driver() {
  delete pController;
  delete adminSQ;
  delete adminCQ;
  delete ioSQ;
  delete ioCQ;
}

void Driver::init(std::function<void()> &func) {
  beginFunction = func;

  // NVMe Initialization process (Register)
  // See Section 7.6.1. Initialization of NVMe 1.3c
  union {
    uint64_t value;
    uint8_t buffer[8];
  } temp;
  uint64_t tick = 0;

  // Step 1. Read CAP
  pController->readRegister(SimpleSSD::HIL::NVMe::REG_CONTROLLER_CAPABILITY, 8,
                            temp.buffer, tick);

  // MPSMAX/MIN is setted to 4KB
  // DSTRD is setted to 0 (4bytes)
  // Check MQES
  maxQueueEntries = (temp.value & 0xFFFF) + 1;

  // Step 2. Wait for CSTS.RDY = 0
  // Step 3. Configure admin queue
  // Step 3-1. Set admin queue entry size
  uint16_t entries = QUEUE_ENTRY_ADMIN;

  if (entries > maxQueueEntries) {
    entries = maxQueueEntries;
  }

  temp.value = entries - 1;
  temp.value |= (entries - 1) << 16;

  pController->writeRegister(SimpleSSD::HIL::NVMe::REG_ADMIN_QUEUE_ATTRIBUTE, 4,
                             temp.buffer, tick);

  adminSQ = new Queue(entries, 64);
  adminCQ = new Queue(entries, 16);

  // Step 3-2. Write base addresses
  adminSQ->getBaseAddress(temp.value);
  pController->writeRegister(SimpleSSD::HIL::NVMe::REG_ADMIN_SQUEUE_BASE_ADDR,
                             8, temp.buffer, tick);
  adminCQ->getBaseAddress(temp.value);
  pController->writeRegister(SimpleSSD::HIL::NVMe::REG_ADMIN_CQUEUE_BASE_ADDR,
                             8, temp.buffer, tick);

  // Step 4. Configure controller
  // Step 5. Enable controller
  temp.value = 1;            // Round Robin, 4K page, NVM command set, Enable
  temp.value |= 0x00460000;  // 64B SQEntry, 16B CQEntry
  pController->writeRegister(SimpleSSD::HIL::NVMe::REG_CONTROLLER_CONFIG, 4,
                             temp.buffer, tick);

  // Step 6. Wait for CSTS.RDY = 1
  // Step 7. Send Identify
  // Step 7-1. Submit Identify Controller
  uint32_t cmd[16];
  PRP *prp = new PRP(4096);
  ResponseHandler callback = [this](uint16_t status, uint32_t, void *context) {
    _init0(status, context);
  };

  memset(cmd, 0, 64);
  cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_IDENTIFY;  // CID, FUSE, OPC
  prp->getPointer(*(uint64_t *)(cmd + 6), *(uint64_t *)(cmd + 8));  // DPTR
  cmd[10] = SimpleSSD::HIL::NVMe::CNS_IDENTIFY_CONTROLLER;          // CNS

  submitCommand(0, (uint8_t *)cmd, callback, prp);
}

void Driver::_init0(uint16_t, void *context) {
  PRP *prp = (PRP *)context;

  // Step 7-2. Send Identify Active Namespace List
  // Reuse PRP here
  uint32_t cmd[16];
  ResponseHandler callback = [this](uint16_t status, uint32_t, void *context) {
    _init1(status, context);
  };

  memset(cmd, 0, 64);
  cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_IDENTIFY;  // CID, FUSE, OPC
  prp->getPointer(*(uint64_t *)(cmd + 6), *(uint64_t *)(cmd + 8));  // DPTR
  cmd[10] = SimpleSSD::HIL::NVMe::CNS_ACTIVE_NAMESPACE_LIST;        // CNS

  submitCommand(0, (uint8_t *)cmd, callback, prp);
}

void Driver::_init1(uint16_t, void *context) {
  PRP *prp = (PRP *)context;

  // Step 7-3. Check active Namespace
  // We will perform I/O on first Namespace
  uint32_t count = 0;
  uint32_t nsid = 0;

  for (count = 0; count < 1024; count++) {
    prp->readData(count * 4, 4, (uint8_t *)&nsid);

    if (nsid == 0) {
      break;
    }

    if (count == 0) {
      namespaceID = nsid;
    }
  }

  if (count == 0) {
    SimpleSSD::panic("This NVMe SSD does not have any namespaces.");
  }
  else if (count > 1) {
    SimpleSSD::warn("This NVMe SSD has %u namespaces.", count);
    SimpleSSD::warn("All I/O will performed on namespace ID %u.", namespaceID);
  }

  // Step 7-4. Send Identify Namespace
  // Reuse PRP here
  uint32_t cmd[16];
  ResponseHandler callback = [this](uint16_t status, uint32_t, void *context) {
    _init2(status, context);
  };

  memset(cmd, 0, 64);
  cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_IDENTIFY;  // CID, FUSE, OPC
  cmd[1] = namespaceID;                            // NSID
  prp->getPointer(*(uint64_t *)(cmd + 6), *(uint64_t *)(cmd + 8));  // DPTR
  cmd[10] = SimpleSSD::HIL::NVMe::CNS_IDENTIFY_NAMESPACE;           // CNS

  submitCommand(0, (uint8_t *)cmd, callback, prp);
}

void Driver::_init2(uint16_t, void *context) {
  union {
    uint64_t value;
    uint8_t buffer[8];
  } temp;
  PRP *prp = (PRP *)context;
  uint8_t nFormat, currentFormat;
  uint32_t formatData;

  // Step 7-4. Check structures
  prp->readData(0, 8, temp.buffer);
  capacity = temp.value;

  prp->readData(25, 1, &nFormat);
  nFormat++;

  prp->readData(26, 1, &currentFormat);
  prp->readData(128 + currentFormat * 4ull, 4, (uint8_t *)&formatData);

  LBAsize = (uint32_t)powf(2.f, (float)((formatData >> 16) & 0xFF));
  capacity *= LBAsize;

  delete prp;

  SimpleSSD::info("SIL::NVMe::Driver: Total SSD capacity: %" PRIu64 " bytes",
                  capacity);
  SimpleSSD::info("SIL::NVMe::Driver: Logical Block Size: %" PRIu32 " bytes",
                  LBAsize);

  // Step 8. Determine I/O queue count
  // Step 8-1. Send Set Feature
  uint32_t cmd[16];
  ResponseHandler callback = [this](uint16_t status, uint32_t dw0,
                                    void *context) {
    _init3(status, dw0, context);
  };

  memset(cmd, 0, 64);
  cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_SET_FEATURES;        // CID, FUSE, OPC
  cmd[10] = SimpleSSD::HIL::NVMe::FEATURE_NUMBER_OF_QUEUES;  // FID
  cmd[11] = 0x00000000;  // One I/O SQ, One I/O CQ

  submitCommand(0, (uint8_t *)cmd, callback, nullptr);
}

void Driver::_init3(uint16_t, uint32_t dw0, void *) {
  // Step 8-2. Check response
  if (dw0 != 0x00000000) {
    SimpleSSD::warn("NVMe SSD responsed too many I/O queue");
  }

  // Step 9. Allocate I/O Completion Queue
  // Step 9-1. Send Create I/O Completion Queue
  uint32_t cmd[16];
  uint16_t entries = QUEUE_ENTRY_IO;
  ResponseHandler callback = [this](uint16_t status, uint32_t, void *context) {
    _init4(status, context);
  };

  if (entries > maxQueueEntries) {
    entries = maxQueueEntries;
  }

  ioCQ = new Queue(entries, 16);

  memset(cmd, 0, 64);
  cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_CREATE_IO_CQUEUE;  // CID, FUSE, OPC
  ioCQ->getBaseAddress(*(uint64_t *)(cmd + 6));            // DPTR.PRP1
  cmd[10] = ((uint32_t)(entries - 1) << 16) | 0x0001;      // QSIZE, QID
  cmd[11] = 0x00010003;                                    // IV, IEN, PC

  submitCommand(0, (uint8_t *)cmd, callback, nullptr);
}

void Driver::_init4(uint16_t status, void *) {
  // Step 9-2. Check result
  if (status != 0) {
    SimpleSSD::panic("Failed to create I/O Completion Queue");
  }

  // Step 10. Allocate I/O Submission Queue
  // Step 10-1. Send Create I/O Submission Queue
  uint32_t cmd[16];
  uint16_t entries = QUEUE_ENTRY_IO;
  ResponseHandler callback = [this](uint16_t status, uint32_t, void *context) {
    _init5(status, context);
  };

  if (entries > maxQueueEntries) {
    entries = maxQueueEntries;
  }

  ioSQ = new Queue(entries, 64);

  memset(cmd, 0, 64);
  cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_CREATE_IO_SQUEUE;  // CID, FUSE, OPC
  ioSQ->getBaseAddress(*(uint64_t *)(cmd + 6));            // DPTR.PRP1
  cmd[10] = ((uint32_t)(entries - 1) << 16) | 0x0001;      // QSIZE, QID
  cmd[11] = 0x00010001;                                    // CQID, QPRIO, PC

  submitCommand(0, (uint8_t *)cmd, callback, nullptr);
}

void Driver::_init5(uint16_t status, void *) {
  // Step 10-2. Check result
  if (status != 0) {
    SimpleSSD::panic("Failed to create I/O Submission Queue");
  }

  SimpleSSD::info("SIL::NVMe::Driver: Initialization finished");

  // Now we initialized NVMe SSD
  beginFunction();
}

void Driver::submitCommand(uint16_t iv, uint8_t *cmd, ResponseHandler &func,
                           void *context) {
  uint16_t cid = 0;
  uint16_t opcode = cmd[0];
  uint16_t tail = 0;
  uint64_t tick = engine.getCurrentTick();
  Queue *queue = nullptr;

  // Push to queue
  if (iv == 0) {
    increaseCommandID(adminCommandID);
    cid = adminCommandID;
    queue = adminSQ;
  }
  else if (iv == 1 && ioSQ) {
    increaseCommandID(ioCommandID);
    cid = ioCommandID;
    queue = ioSQ;
  }
  else {
    SimpleSSD::panic("I/O Submission Queue is not initialized");
  }

  memcpy(cmd + 2, &cid, 2);
  queue->setData(cmd, 64);
  tail = queue->getTail();

  // Push to pending cmd list
  pendingCommandList.push_back(CommandEntry(iv, opcode, cid, context, func));

  // Ring doorbell
  pController->ringSQTailDoorbell(iv, tail, tick);
  queue->incrHead();
}

void Driver::increaseCommandID(uint16_t &id) {
  static const uint16_t maxID = 32767;

  id++;

  if (id > maxID) {
    id = 1;
  }
}

void Driver::getInfo(uint64_t &bytesize, uint32_t &minbs) {
  bytesize = capacity;
  minbs = LBAsize;
}

void Driver::submitIO(BIL::BIO &bio) {
  uint32_t cmd[16];
  PRP *prp = nullptr;
  static ResponseHandler callback = [this](uint16_t status, uint32_t,
                                           void *context) {
    _io(status, context);
  };

  memset(cmd, 0, 64);

  uint64_t slba = bio.offset / LBAsize;
  uint32_t nlb = (uint32_t)DIVCEIL(bio.length, LBAsize);

  cmd[1] = namespaceID;  // NSID

  if (bio.type == BIL::BIO_READ) {
    cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_READ;  // CID, FUSE, OPC
    cmd[10] = (uint32_t)slba;
    cmd[11] = slba >> 32;
    cmd[12] = nlb - 1;  // LR, FUA, PRINFO, NLB

    prp = new PRP(bio.length);
    prp->getPointer(*(uint64_t *)(cmd + 6), *(uint64_t *)(cmd + 8));  // DPTR
  }
  else if (bio.type == BIL::BIO_WRITE) {
    cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_WRITE;  // CID, FUSE, OPC
    cmd[10] = (uint32_t)slba;
    cmd[11] = slba >> 32;
    cmd[12] = nlb - 1;  // LR, FUA, PRINFO, DTYPE, NLB

    prp = new PRP(bio.length);
    prp->getPointer(*(uint64_t *)(cmd + 6), *(uint64_t *)(cmd + 8));  // DPTR
  }
  else if (bio.type == BIL::BIO_FLUSH) {
    cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_FLUSH;  // CID, FUSE, OPC
  }
  else if (bio.type == BIL::BIO_TRIM) {
    cmd[0] = SimpleSSD::HIL::NVMe::OPCODE_DATASET_MANAGEMEMT;  // CID, FUSE, OPC
    cmd[10] = 0;                                               // NR
    cmd[11] = 0x04;                                            // AD

    prp = new PRP(16);
    prp->getPointer(*(uint64_t *)(cmd + 6), *(uint64_t *)(cmd + 8));  // DPTR

    // Fill range definition
    uint8_t data[16];

    memset(data, 0, 16);
    memcpy(data + 4, &nlb, 4);
    memcpy(data + 8, &slba, 8);

    prp->writeData(0, 16, data);
  }

  submitCommand(1, (uint8_t *)cmd, callback,
                new IOWrapper(bio.id, prp, bio.callback));
}

void Driver::_io(uint16_t status, void *context) {
  IOWrapper *wrapper = (IOWrapper *)context;
  PRP *prp = wrapper->prp;

  if (status != 0) {
    SimpleSSD::warn("I/O error: %04X", status);
  }

  wrapper->bioCallback(wrapper->id);

  delete prp;
  delete wrapper;
}

void Driver::initStats(std::vector<SimpleSSD::Stats> &list) {
  pController->getStatList(list, "");
  SimpleSSD::getCPUStatList(list, "cpu");
}

void Driver::getStats(std::vector<double> &values) {
  pController->getStatValues(values);
  SimpleSSD::getCPUStatValues(values);
}

void Driver::dmaRead(uint64_t addr, uint64_t size, uint8_t *buffer,
                     SimpleSSD::DMAFunction &func, void *context) {
  if (size == 0) {
    SimpleSSD::warn("nvme_interface: zero-size DMA read request. Ignore.");

    return;
  }

  dmaReadQueue.push(DMAEntry(func));

  auto &iter = dmaReadQueue.back();
  iter.addr = addr;
  iter.size = size;
  iter.buffer = buffer;
  iter.context = context;

  if (!dmaReadPending) {
    submitDMARead();
  }
}

void Driver::dmaReadDone() {
  auto &iter = dmaReadQueue.front();
  uint64_t tick = engine.getCurrentTick();

  if (tick < iter.finishedAt) {
    engine.scheduleEvent(dmaReadEvent, iter.finishedAt);

    return;
  }

  iter.func(tick, iter.context);
  dmaReadQueue.pop();
  dmaReadPending = false;

  if (dmaReadQueue.size() > 0) {
    submitDMARead();
  }
}

void Driver::submitDMARead() {
  auto &iter = dmaReadQueue.front();

  dmaReadPending = true;

  iter.beginAt = engine.getCurrentTick();
  iter.finishedAt = iter.beginAt + SimpleSSD::PCIExpress::calculateDelay(
                                       pcieGen, pcieLane, iter.size);

  if (iter.buffer) {
    memcpy(iter.buffer, (uint8_t *)iter.addr, iter.size);
  }

  engine.scheduleEvent(dmaReadEvent, iter.finishedAt);
}

void Driver::dmaWrite(uint64_t addr, uint64_t size, uint8_t *buffer,
                      SimpleSSD::DMAFunction &func, void *context) {
  if (size == 0) {
    SimpleSSD::warn("nvme_interface: zero-size DMA write request. Ignore.");

    return;
  }

  dmaWriteQueue.push(DMAEntry(func));

  auto &iter = dmaWriteQueue.back();
  iter.addr = addr;
  iter.size = size;
  iter.buffer = buffer;
  iter.context = context;

  if (!dmaWritePending) {
    submitDMAWrite();
  }
}

void Driver::dmaWriteDone() {
  auto &iter = dmaWriteQueue.front();
  uint64_t tick = engine.getCurrentTick();

  if (tick < iter.finishedAt) {
    engine.scheduleEvent(dmaWriteEvent, iter.finishedAt);

    return;
  }

  iter.func(tick, iter.context);
  dmaWriteQueue.pop();
  dmaWritePending = false;

  if (dmaWriteQueue.size() > 0) {
    submitDMAWrite();
  }
}

void Driver::submitDMAWrite() {
  auto &iter = dmaWriteQueue.front();

  dmaWritePending = true;

  iter.beginAt = engine.getCurrentTick();
  iter.finishedAt = iter.beginAt + SimpleSSD::PCIExpress::calculateDelay(
                                       pcieGen, pcieLane, iter.size);

  if (iter.buffer) {
    memcpy((uint8_t *)iter.addr, iter.buffer, iter.size);
  }

  engine.scheduleEvent(dmaWriteEvent, iter.finishedAt);
}

void Driver::updateInterrupt(uint16_t iv, bool post) {
  uint32_t cqdata[4];

  if (post) {
    uint64_t tick = engine.getCurrentTick();
    uint16_t count = 0;
    Queue *queue = nullptr;

    if (iv == 0) {
      queue = adminCQ;
    }
    else if (iv == 1 && ioCQ) {
      queue = ioCQ;
    }
    else {
      SimpleSSD::panic("I/O Completion Queue is not initialized");
    }

    // Peek queue for count how many requests are finished
    while (true) {
      queue->peekData((uint8_t *)cqdata, 16);

      // Check phase tag
      if (((cqdata[3] >> 16) & 0x01) == phase) {
        bool found = false;

        queue->incrTail();
        count++;

        // Search pending command list
        for (auto iter = pendingCommandList.begin();
             iter != pendingCommandList.end(); iter++) {
          if (iter->iv == iv && iter->cid == (cqdata[3] & 0xFFFF)) {
            iter->callback((uint16_t)(cqdata[3] >> 17), cqdata[0],
                           iter->context);

            pendingCommandList.erase(iter);
            found = true;

            break;
          }
        }

        if (found) {
          queue->incrHead();

          if (queue->getHead() == 0) {
            // Inverted
            phase = !phase;
          }
        }
        else {
          SimpleSSD::panic("Invalid interrupt");
        }
      }
      else {
        if (count > 0) {
          pController->ringCQHeadDoorbell(iv, queue->getHead(), tick);
        }

        break;
      }
    }
  }
}

void Driver::getVendorID(uint16_t &vid, uint16_t &ssvid) {
  // Copied from SimpleSSD-FullSystem
  vid = 0x144D;
  ssvid = 0x8086;
}

}  // namespace NVMe

}  // namespace SIL
