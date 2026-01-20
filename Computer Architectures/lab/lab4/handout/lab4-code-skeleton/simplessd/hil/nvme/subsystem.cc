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

#include "hil/nvme/subsystem.hh"

#include <algorithm>
#include <cmath>

#include "hil/nvme/controller.hh"
#include "util/algorithm.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

const uint32_t nLBAFormat = 4;
const uint32_t lbaFormat[nLBAFormat] = {
    0x02090000,  // 512B + 0, Good performance
    0x020A0000,  // 1KB + 0, Good performance
    0x010B0000,  // 2KB + 0, Better performance
    0x000C0000,  // 4KB + 0, Best performance
};
const uint32_t lbaSize[nLBAFormat] = {
    512,   // 512B
    1024,  // 1KB
    2048,  // 2KB
    4096,  // 4KB
};

Subsystem::Subsystem(Controller *ctrl, ConfigData &cfg)
    : AbstractSubsystem(ctrl, cfg),
      pHIL(nullptr),
      allocatedLogicalPages(0),
      commandCount(0) {}

Subsystem::~Subsystem() {
  for (auto &iter : lNamespaces) {
    delete iter;
  }

  delete pHIL;
}

void Subsystem::init() {
  pHIL = new HIL(conf);
  uint16_t nNamespaces =
      (uint16_t)conf.readUint(CONFIG_NVME, NVME_ENABLE_DEFAULT_NAMESPACE);

  pHIL->getLPNInfo(totalLogicalPages, logicalPageSize);

  if (nNamespaces > 0) {
    Namespace::Information info;
    uint64_t totalSize;
    uint32_t lba = (uint32_t)conf.readUint(CONFIG_NVME, NVME_LBA_SIZE);

    for (info.lbaFormatIndex = 0; info.lbaFormatIndex < nLBAFormat;
         info.lbaFormatIndex++) {
      if (lbaSize[info.lbaFormatIndex] == lba) {
        info.lbaSize = lbaSize[info.lbaFormatIndex];

        break;
      }
    }

    if (info.lbaFormatIndex == nLBAFormat) {
      panic("Failed to setting LBA size (512B ~ 4KB)");
    }

    // Fill Namespace information
    totalSize = totalLogicalPages * logicalPageSize / lba;
    info.dataProtectionSettings = 0x00;
    info.namespaceSharingCapabilities = 0x00;

    for (uint16_t i = 0; i < nNamespaces; i++) {
      info.size = totalSize / nNamespaces;
      info.capacity = info.size;

      if (createNamespace(NSID_LOWEST + i, &info)) {
        lNamespaces.back()->attach(true);
      }
      else {
        panic("Failed to create namespace");
      }
    }
  }
}

void Subsystem::convertUnit(Namespace *ns, uint64_t slba, uint64_t nlblk,
                            Request &req) {
  Namespace::Information *info = ns->getInfo();
  uint32_t lbaratio = logicalPageSize / info->lbaSize;
  uint64_t slpn;
  uint64_t nlp;
  uint64_t off;

  if (lbaratio == 0) {
    lbaratio = info->lbaSize / logicalPageSize;

    slpn = slba * lbaratio;
    nlp = nlblk * lbaratio;

    req.range.slpn = slpn + info->range.slpn;
    req.range.nlp = nlp;
    req.offset = 0;
    req.length = nlblk * info->lbaSize;
  }
  else {
    slpn = slba / lbaratio;
    off = slba % lbaratio;
    nlp = (nlblk + off + lbaratio - 1) / lbaratio;

    req.range.slpn = slpn + info->range.slpn;
    req.range.nlp = nlp;
    req.offset = off * info->lbaSize;
    req.length = nlblk * info->lbaSize;
  }
}

bool Subsystem::createNamespace(uint32_t nsid, Namespace::Information *info) {
  std::list<LPNRange> allocated;
  std::list<LPNRange> unallocated;

  // Allocate LPN
  uint64_t requestedLogicalPages =
      info->size / logicalPageSize * lbaSize[info->lbaFormatIndex];
  uint64_t unallocatedLogicalPages = totalLogicalPages - allocatedLogicalPages;

  if (requestedLogicalPages > unallocatedLogicalPages) {
    return false;
  }

  // Collect allocated slots
  for (auto &iter : lNamespaces) {
    allocated.push_back(iter->getInfo()->range);
  }

  // Sort
  allocated.sort([](const LPNRange &a, const LPNRange &b) -> bool {
    return a.slpn < b.slpn;
  });

  // Merge
  auto iter = allocated.begin();

  if (iter != allocated.end()) {
    auto next = iter;

    while (true) {
      next++;

      if (iter != allocated.end() || next == allocated.end()) {
        break;
      }

      if (iter->slpn + iter->nlp == next->slpn) {
        iter = allocated.erase(iter);
        next = iter;
      }
      else {
        iter++;
      }
    }
  }

  // Invert
  unallocated.push_back(LPNRange(0, totalLogicalPages));

  for (auto &iter : allocated) {
    // Split last item
    auto &last = unallocated.back();

    if (last.slpn <= iter.slpn &&
        last.slpn + last.nlp >= iter.slpn + iter.nlp) {
      unallocated.push_back(LPNRange(
          iter.slpn + iter.nlp, last.slpn + last.nlp - iter.slpn - iter.nlp));
      last.nlp = iter.slpn - last.slpn;
    }
    else {
      panic("BUG");
    }
  }

  // Allocated unallocated area to namespace
  for (auto iter = unallocated.begin(); iter != unallocated.end(); iter++) {
    if (iter->nlp >= requestedLogicalPages) {
      info->range = *iter;
      info->range.nlp = requestedLogicalPages;

      break;
    }
  }

  if (info->range.nlp == 0) {
    return false;
  }

  allocatedLogicalPages += requestedLogicalPages;

  // Fill Information
  info->sizeInByteL = requestedLogicalPages * logicalPageSize;
  info->sizeInByteH = 0;

  // Create namespace
  Namespace *pNS = new Namespace(this, cfgdata);
  pNS->setData(nsid, info);

  lNamespaces.push_back(pNS);
  debugprint(LOG_HIL_NVME,
             "NS %-5d| CREATE | LBA size %" PRIu32 " | Capacity %" PRIu64, nsid,
             info->lbaSize, info->size);

  lNamespaces.sort([](Namespace *lhs, Namespace *rhs) -> bool {
    return lhs->getNSID() < rhs->getNSID();
  });

  return true;
}

bool Subsystem::destroyNamespace(uint32_t nsid) {
  bool found = false;
  Namespace::Information *info;

  for (auto iter = lNamespaces.begin(); iter != lNamespaces.end(); iter++) {
    if ((*iter)->getNSID() == nsid) {
      found = true;

      debugprint(LOG_HIL_NVME, "NS %-5d| DELETE", nsid);

      info = (*iter)->getInfo();
      allocatedLogicalPages -= info->size * info->lbaSize / logicalPageSize;

      delete *iter;

      lNamespaces.erase(iter);

      break;
    }
  }

  return found;
}

void Subsystem::fillIdentifyNamespace(uint8_t *buffer,
                                      Namespace::Information *info) {
  // Namespace Size
  memcpy(buffer + 0, &info->size, 8);

  // Namespace Capacity
  memcpy(buffer + 8, &info->capacity, 8);

  // Namespace Utilization
  // This function also called from OpenChannelSSD subsystem
  if (pHIL) {
    info->utilization =
        pHIL->getUsedPageCount(info->range.slpn,
                               info->range.slpn + info->range.nlp) *
        logicalPageSize / info->lbaSize;
  }

  memcpy(buffer + 16, &info->utilization, 8);

  // Namespace Features
  // [Bits ] Description
  // [07:05] Reserved
  // [04:04] NVM Set capabilities
  // [03:03] Reuse of NGUID field
  // [02:02] 1 for Support Deallocated or Unwritten Logical Block error
  // [01:01] 1 for NAWUN, NAWUPF, NACWU are defined
  // [00:00] 1 for Support Thin Provisioning
  buffer[24] = 0x04;  // Trim supported

  // Number of LBA Formats
  buffer[25] = nLBAFormat - 1;  // 0's based

  // Formatted LBA Size
  buffer[26] = info->lbaFormatIndex;

  // End-to-end Data Protection Capabilities
  buffer[28] = info->dataProtectionSettings;

  // Namespace Multi-path I/O and Namespace Sharing Capabilities
  buffer[30] = info->namespaceSharingCapabilities;

  // NVM capacity
  memcpy(buffer + 48, &info->sizeInByteL, 8);
  memcpy(buffer + 56, &info->sizeInByteH, 8);

  // LBA Formats
  for (uint32_t i = 0; i < nLBAFormat; i++) {
    memcpy(buffer + 128 + i * 4, lbaFormat + i, 4);
  }

  // OpenChannel SSD
  if (!pHIL) {
    buffer[384] = 0x01;
  }
}

void Subsystem::submitCommand(SQEntryWrapper &req, RequestFunction func) {
  struct CommandContext {
    SQEntryWrapper req;
    RequestFunction func;

    CommandContext(SQEntryWrapper &r, RequestFunction &f) : req(r), func(f) {}
  };

  CQEntryWrapper resp(req);
  bool processed = false;

  commandCount++;

  // Admin command
  if (req.sqID == 0) {
    switch (req.entry.dword0.opcode) {
      case OPCODE_DELETE_IO_SQUEUE:
        processed = deleteSQueue(req, func);
        break;
      case OPCODE_CREATE_IO_SQUEUE:
        processed = createSQueue(req, func);
        break;
      case OPCODE_GET_LOG_PAGE:
        processed = getLogPage(req, func);
        break;
      case OPCODE_DELETE_IO_CQUEUE:
        processed = deleteCQueue(req, func);
        break;
      case OPCODE_CREATE_IO_CQUEUE:
        processed = createCQueue(req, func);
        break;
      case OPCODE_IDENTIFY:
        processed = identify(req, func);
        break;
      case OPCODE_ABORT:
        processed = abort(req, func);
        break;
      case OPCODE_SET_FEATURES:
        processed = setFeatures(req, func);
        break;
      case OPCODE_GET_FEATURES:
        processed = getFeatures(req, func);
        break;
      case OPCODE_ASYNC_EVENT_REQ:
        break;
      case OPCODE_NAMESPACE_MANAGEMENT:
        processed = namespaceManagement(req, func);
        break;
      case OPCODE_NAMESPACE_ATTACHMENT:
        processed = namespaceAttachment(req, func);
        break;
      case OPCODE_FORMAT_NVM:
        processed = formatNVM(req, func);
        break;
      default:
        resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                        STATUS_INVALID_OPCODE);
        break;
    }
  }

  // NVM commands or Namespace specific Admin commands
  if (!processed) {
    if (req.entry.namespaceID < NSID_ALL) {
      for (auto &iter : lNamespaces) {
        if (iter->getNSID() == req.entry.namespaceID) {
          auto pContext = new CommandContext(req, func);
          DMAFunction doSubmit = [iter](uint64_t, void *context) {
            auto pContext = (CommandContext *)context;

            iter->submitCommand(pContext->req, pContext->func);

            delete pContext;
          };

          execute(CPU::NVME__NAMESPACE, CPU::SUBMIT_COMMAND, doSubmit,
                  pContext);

          return;
        }
      }
    }
    else {
      if (lNamespaces.size() > 0) {
        auto pContext = new CommandContext(req, func);
        DMAFunction doSubmit = [this](uint64_t, void *context) {
          auto pContext = (CommandContext *)context;

          lNamespaces.front()->submitCommand(pContext->req, pContext->func);

          delete pContext;
        };

        execute(CPU::NVME__NAMESPACE, CPU::SUBMIT_COMMAND, doSubmit, pContext);

        processed = true;
      }
    }
  }

  // Invalid namespace
  if (!processed) {
    resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                    STATUS_ABORT_INVALID_NAMESPACE);

    func(resp);
  }
}

void Subsystem::getNVMCapacity(uint64_t &total, uint64_t &used) {
  total = totalLogicalPages * logicalPageSize;
  used = allocatedLogicalPages * logicalPageSize;
}

uint32_t Subsystem::validNamespaceCount() {
  return (uint32_t)lNamespaces.size();
}

void Subsystem::read(Namespace *ns, uint64_t slba, uint64_t nlblk,
                     DMAFunction &func, void *context) {
  Request *req = new Request(func, context);
  DMAFunction doRead = [this](uint64_t, void *context) {
    auto req = (Request *)context;

    pHIL->read(*req);

    delete req;
  };

  convertUnit(ns, slba, nlblk, *req);

  execute(CPU::NVME__SUBSYSTEM, CPU::CONVERT_UNIT, doRead, req);
}

void Subsystem::write(Namespace *ns, uint64_t slba, uint64_t nlblk,
                      DMAFunction &func, void *context) {
  Request *req = new Request(func, context);
  DMAFunction doWrite = [this](uint64_t, void *context) {
    auto req = (Request *)context;

    pHIL->write(*req);

    delete req;
  };

  convertUnit(ns, slba, nlblk, *req);

  execute(CPU::NVME__SUBSYSTEM, CPU::CONVERT_UNIT, doWrite, req);
}

void Subsystem::flush(Namespace *ns, DMAFunction &func, void *context) {
  Request req(func, context);
  Namespace::Information *info = ns->getInfo();

  req.range.slpn = info->range.slpn;
  req.range.nlp = info->range.nlp;
  req.offset = 0;
  req.length = info->range.nlp * logicalPageSize;

  pHIL->flush(req);
}

void Subsystem::trim(Namespace *ns, uint64_t slba, uint64_t nlblk,
                     DMAFunction &func, void *context) {
  Request *req = new Request(func, context);
  DMAFunction doTrim = [this](uint64_t, void *context) {
    auto req = (Request *)context;

    pHIL->trim(*req);

    delete req;
  };

  convertUnit(ns, slba, nlblk, *req);

  execute(CPU::NVME__SUBSYSTEM, CPU::CONVERT_UNIT, doTrim, req);
}

bool Subsystem::deleteSQueue(SQEntryWrapper &req, RequestFunction &func) {
  CQEntryWrapper resp(req);
  uint16_t sqid = req.entry.dword10 & 0xFFFF;

  debugprint(LOG_HIL_NVME, "ADMIN   | Delete I/O Submission Queue");

  int ret = pParent->deleteSQueue(sqid);

  if (ret == 1) {
    resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                    STATUS_INVALID_QUEUE_ID);
  }

  func(resp);

  return true;
}

bool Subsystem::createSQueue(SQEntryWrapper &req, RequestFunction &func) {
  CQEntryWrapper resp(req);
  bool err = false;

  uint16_t sqid = req.entry.dword10 & 0xFFFF;
  uint16_t entrySize = ((req.entry.dword10 & 0xFFFF0000) >> 16) + 1;
  uint16_t cqid = (req.entry.dword11 & 0xFFFF0000) >> 16;
  uint8_t priority = (req.entry.dword11 & 0x06) >> 1;
  bool pc = req.entry.dword11 & 0x01;

  debugprint(LOG_HIL_NVME, "ADMIN   | Create I/O Submission Queue");

  if (entrySize > cfgdata.maxQueueEntry) {
    err = true;
    resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                    STATUS_INVALID_QUEUE_SIZE);
  }

  if (!err) {
    RequestContext *pContext = new RequestContext(func, resp);
    static DMAFunction doQueue = [](uint64_t, void *context) {
      RequestContext *pContext = (RequestContext *)context;

      pContext->function(pContext->resp);

      delete pContext;
    };

    int ret = pParent->createSQueue(sqid, cqid, entrySize, priority, pc,
                                    req.entry.data1, doQueue, pContext);

    if (ret == 1) {
      resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                      STATUS_INVALID_QUEUE_ID);
    }
    else if (ret == 2) {
      resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                      STATUS_INVALID_COMPLETION_QUEUE);
    }
  }
  else {
    func(resp);
  }

  return true;
}

bool Subsystem::getLogPage(SQEntryWrapper &req, RequestFunction &func) {
  CQEntryWrapper resp(req);
  uint16_t numdl = (req.entry.dword10 & 0xFFFF0000) >> 16;
  uint16_t lid = req.entry.dword10 & 0xFFFF;
  uint16_t numdu = req.entry.dword11 & 0xFFFF;
  uint32_t lopl = req.entry.dword12;
  uint32_t lopu = req.entry.dword13;
  bool ret = false;
  bool submit = true;

  uint32_t req_size = (((uint32_t)numdu << 16 | numdl) + 1) * 4;
  uint64_t offset = ((uint64_t)lopu << 32) | lopl;

  debugprint(LOG_HIL_NVME,
             "ADMIN   | Get Log Page | Log %d | Size %d | NSID %d", lid,
             req_size, req.entry.namespaceID);

  static DMAFunction dmaDone = [](uint64_t, void *context) {
    RequestContext *pContext = (RequestContext *)context;

    pContext->function(pContext->resp);

    delete pContext->dma;
    delete pContext;
  };
  DMAFunction smartInfo = [offset](uint64_t, void *context) {
    RequestContext *pContext = (RequestContext *)context;

    pContext->dma->write(offset, 512, pContext->buffer, dmaDone, context);
  };

  switch (lid) {
    case LOG_ERROR_INFORMATION:
      ret = true;
      break;
    case LOG_SMART_HEALTH_INFORMATION: {
      if (req.entry.namespaceID == NSID_ALL) {
        ret = true;
        submit = false;
        RequestContext *pContext = new RequestContext(func, resp);

        pContext->buffer = globalHealth.data;

        if (req.useSGL) {
          pContext->dma = new SGL(cfgdata, smartInfo, pContext, req.entry.data1,
                                  req.entry.data2);
        }
        else {
          pContext->dma =
              new PRPList(cfgdata, smartInfo, pContext, req.entry.data1,
                          req.entry.data2, (uint64_t)req_size);
        }
      }
    } break;
    case LOG_FIRMWARE_SLOT_INFORMATION:
      ret = true;
      break;
    default:
      ret = true;
      resp.makeStatus(true, false, TYPE_COMMAND_SPECIFIC_STATUS,
                      STATUS_INVALID_LOG_PAGE);
      break;
  }

  if (submit) {
    func(resp);
  }

  return ret;
}

bool Subsystem::deleteCQueue(SQEntryWrapper &req, RequestFunction &func) {
  CQEntryWrapper resp(req);
  uint16_t cqid = req.entry.dword10 & 0xFFFF;

  debugprint(LOG_HIL_NVME, "ADMIN   | Delete I/O Completion Queue");

  int ret = pParent->deleteCQueue(cqid);

  if (ret == 1) {
    resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                    STATUS_INVALID_QUEUE_ID);
  }
  else if (ret == 2) {
    resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                    STATUS_INVALID_QUEUE_DELETE);
  }

  func(resp);

  return true;
}

bool Subsystem::createCQueue(SQEntryWrapper &req, RequestFunction &func) {
  CQEntryWrapper resp(req);
  bool err = false;

  uint16_t cqid = req.entry.dword10 & 0xFFFF;
  uint16_t entrySize = ((req.entry.dword10 & 0xFFFF0000) >> 16) + 1;
  uint16_t iv = (req.entry.dword11 & 0xFFFF0000) >> 16;
  bool ien = req.entry.dword11 & 0x02;
  bool pc = req.entry.dword11 & 0x01;

  debugprint(LOG_HIL_NVME, "ADMIN   | Create I/O Completion Queue");

  if (entrySize > cfgdata.maxQueueEntry) {
    err = true;
    resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                    STATUS_INVALID_QUEUE_SIZE);
  }

  if (!err) {
    RequestContext *pContext = new RequestContext(func, resp);
    static DMAFunction doQueue = [](uint64_t, void *context) {
      RequestContext *pContext = (RequestContext *)context;

      pContext->function(pContext->resp);

      delete pContext;
    };

    int ret = pParent->createCQueue(cqid, entrySize, iv, ien, pc,
                                    req.entry.data1, doQueue, pContext);

    if (ret == 1) {
      resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                      STATUS_INVALID_QUEUE_ID);
    }
  }
  else {
    func(resp);
  }

  return true;
}

bool Subsystem::identify(SQEntryWrapper &req, RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  uint8_t cns = req.entry.dword10 & 0xFF;
  uint16_t cntid = (req.entry.dword10 & 0xFFFF0000) >> 16;
  bool ret = true;

  RequestContext *pContext = new RequestContext(func, resp);
  uint16_t idx = 0;

  pContext->buffer = (uint8_t *)calloc(0x1000, sizeof(uint8_t));
  debugprint(LOG_HIL_NVME, "ADMIN   | Identify | CNS %d | CNTID %d | NSID %d",
             cns, cntid, req.entry.namespaceID);

  switch (cns) {
    case CNS_IDENTIFY_NAMESPACE:
      if (req.entry.namespaceID == NSID_ALL) {
        // FIXME: Not called by current(4.9.32) NVMe driver
      }
      else {
        for (auto &iter : lNamespaces) {
          if (iter->isAttached() && iter->getNSID() == req.entry.namespaceID) {
            fillIdentifyNamespace(pContext->buffer, iter->getInfo());
          }
        }
      }

      break;
    case CNS_IDENTIFY_CONTROLLER:
      pParent->identify(pContext->buffer);

      break;
    case CNS_ACTIVE_NAMESPACE_LIST:
      if (req.entry.namespaceID >= NSID_ALL - 1) {
        err = true;
        resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                        STATUS_ABORT_INVALID_NAMESPACE);
      }
      else {
        for (auto &iter : lNamespaces) {
          if (iter->isAttached() && iter->getNSID() > req.entry.namespaceID) {
            ((uint32_t *)pContext->buffer)[idx++] = iter->getNSID();
          }
        }
      }

      break;
    case CNS_ALLOCATED_NAMESPACE_LIST:
      if (req.entry.namespaceID >= NSID_ALL - 1) {
        err = true;
        resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                        STATUS_ABORT_INVALID_NAMESPACE);
      }
      else {
        for (auto &iter : lNamespaces) {
          if (iter->getNSID() > req.entry.namespaceID) {
            ((uint32_t *)pContext->buffer)[idx++] = iter->getNSID();
          }
        }
      }

      break;
    case CNS_IDENTIFY_ALLOCATED_NAMESPACE:
      for (auto &iter : lNamespaces) {
        if (iter->getNSID() == req.entry.namespaceID) {
          fillIdentifyNamespace(pContext->buffer, iter->getInfo());
        }
      }

      break;
    case CNS_ATTACHED_CONTROLLER_LIST:
      // Only one controller
      if (cntid == 0) {
        ((uint16_t *)pContext->buffer)[idx++] = 1;
      }

      break;
    case CNS_CONTROLLER_LIST:
      // Only one controller
      if (cntid == 0) {
        ((uint16_t *)pContext->buffer)[idx++] = 1;
      }

      break;
  }

  static DMAFunction dmaDone = [](uint64_t, void *context) {
    RequestContext *pContext = (RequestContext *)context;

    pContext->function(pContext->resp);

    free(pContext->buffer);
    delete pContext->dma;
    delete pContext;
  };
  static DMAFunction doWrite = [](uint64_t, void *context) {
    RequestContext *pContext = (RequestContext *)context;

    pContext->dma->write(0, 0x1000, pContext->buffer, dmaDone, context);
  };

  if (ret && !err) {
    if (req.useSGL) {
      pContext->dma =
          new SGL(cfgdata, doWrite, pContext, req.entry.data1, req.entry.data2);
    }
    else {
      pContext->dma = new PRPList(cfgdata, doWrite, pContext, req.entry.data1,
                                  req.entry.data2, (uint64_t)0x1000);
    }
  }
  else {
    func(resp);
    free(pContext->buffer);
    delete pContext;
  }

  return ret;
}

bool Subsystem::abort(SQEntryWrapper &req, RequestFunction &func) {
  CQEntryWrapper resp(req);
  uint16_t sqid = req.entry.dword10 & 0xFFFF;
  uint16_t cid = (req.entry.dword10 & 0xFFFF0000) >> 16;

  debugprint(LOG_HIL_NVME, "ADMIN   | Abort | SQID %d | CID %d", sqid, cid);

  int ret = pParent->abort(sqid, cid);

  resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS, STATUS_SUCCESS);

  if (ret == 0) {
    resp.entry.dword0 = 1;
  }
  else {
    resp.entry.dword0 = 0;
  }

  func(resp);

  return true;
}

bool Subsystem::setFeatures(SQEntryWrapper &req, RequestFunction &func) {
  bool err = false;
  static uint32_t cqsize = conf.readUint(CONFIG_NVME, NVME_MAX_IO_CQUEUE);
  static uint32_t sqsize = conf.readUint(CONFIG_NVME, NVME_MAX_IO_SQUEUE);

  CQEntryWrapper resp(req);
  uint16_t fid = req.entry.dword10 & 0x00FF;
  bool save = req.entry.dword10 & 0x80000000;

  debugprint(LOG_HIL_NVME, "ADMIN   | Set Features | Feature %d | NSID %d", fid,
             req.entry.namespaceID);

  if (save) {
    err = true;
    resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                    STATUS_FEATURE_ID_NOT_SAVEABLE);
  }

  if (!err) {
    switch (fid) {
      case FEATURE_NUMBER_OF_QUEUES:
        if ((req.entry.dword11 & 0xFFFF) == 0xFFFF ||
            (req.entry.dword11 & 0xFFFF0000) == 0xFFFF0000) {
          resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                          STATUS_INVALID_FIELD);
        }
        else {
          if ((req.entry.dword11 & 0xFFFF) >= sqsize) {
            req.entry.dword11 = (req.entry.dword11 & 0xFFFF0000) | (sqsize - 1);
          }
          if (((req.entry.dword11 & 0xFFFF0000) >> 16) >= cqsize) {
            req.entry.dword11 =
                (req.entry.dword11 & 0xFFFF) | ((uint32_t)(cqsize - 1) << 16);
          }

          resp.entry.dword0 = req.entry.dword11;
          queueAllocated = resp.entry.dword0;
        }

        break;
      case FEATURE_INTERRUPT_COALESCING:
        pParent->setCoalescingParameter((req.entry.dword11 >> 8) & 0xFF,
                                        req.entry.dword11 & 0xFF);
        break;
      case FEATURE_INTERRUPT_VECTOR_CONFIGURATION:
        pParent->setCoalescing(req.entry.dword11 & 0xFFFF,
                               req.entry.dword11 & 0x10000);
        break;
      default:
        resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                        STATUS_INVALID_FIELD);
        break;
    }
  }

  func(resp);

  return true;
}

bool Subsystem::getFeatures(SQEntryWrapper &req, RequestFunction &func) {
  CQEntryWrapper resp(req);
  uint16_t fid = req.entry.dword10 & 0x00FF;

  debugprint(LOG_HIL_NVME, "ADMIN   | Get Features | Feature %d | NSID %d", fid,
             req.entry.namespaceID);

  switch (fid) {
    case FEATURE_ARBITRATION:
      resp.entry.dword0 =
          ((uint8_t)(conf.readUint(CONFIG_NVME, NVME_WRR_HIGH) *
                         conf.readUint(CONFIG_NVME, NVME_WRR_MEDIUM) -
                     1)
           << 24) |
          ((conf.readUint(CONFIG_NVME, NVME_WRR_MEDIUM) - 1) << 16) | 0x03;
      break;
    case FEATURE_VOLATILE_WRITE_CACHE:
      resp.entry.dword0 =
          conf.readBoolean(CONFIG_ICL, ICL::ICL_USE_WRITE_CACHE) ? 0x01 : 0x00;
      break;
    case FEATURE_NUMBER_OF_QUEUES:
      resp.entry.dword0 = queueAllocated;
      break;
    case FEATURE_INTERRUPT_COALESCING:
      pParent->getCoalescingParameter(resp.entry.data + 1, resp.entry.data);
      break;
    case FEATURE_INTERRUPT_VECTOR_CONFIGURATION:
      resp.entry.dword0 = (req.entry.dword11 & 0xFFFF) | 0;

      if (pParent->getCoalescing(req.entry.dword11 & 0xFFFF)) {
        resp.entry.dword0 |= 0x10000;
      }

      break;
    default:
      resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                      STATUS_INVALID_FIELD);
      break;
  }

  func(resp);

  return true;
}

bool Subsystem::asyncEventReq(SQEntryWrapper &, RequestFunction &) {
  // FIXME: Not implemented
  return true;
}

bool Subsystem::namespaceManagement(SQEntryWrapper &req,
                                    RequestFunction &func) {
  struct NamespaceManagementContext : public RequestContext {
    uint32_t nsid;

    NamespaceManagementContext(RequestFunction &f, CQEntryWrapper &r)
        : RequestContext(f, r), nsid(NSID_NONE) {}
  };

  bool err = false;

  CQEntryWrapper resp(req);
  uint8_t sel = req.entry.dword10 & 0x0F;

  debugprint(LOG_HIL_NVME, "ADMIN   | Namespace Management | OP %d | NSID %d",
             sel, req.entry.namespaceID);

  static DMAFunction dmaDone = [this](uint64_t, void *context) {
    Namespace::Information info;
    NamespaceManagementContext *pContext =
        (NamespaceManagementContext *)context;

    // Copy data
    memcpy(&info.size, pContext->buffer + 0, 8);
    memcpy(&info.capacity, pContext->buffer + 8, 8);
    info.lbaFormatIndex = pContext->buffer[26];
    info.dataProtectionSettings = pContext->buffer[29];
    info.namespaceSharingCapabilities = pContext->buffer[30];

    if (info.lbaFormatIndex >= nLBAFormat) {
      pContext->resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                                STATUS_ABORT_INVALID_FORMAT);
    }
    else if (info.namespaceSharingCapabilities != 0x00) {
      pContext->resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                                STATUS_INVALID_FIELD);
    }
    else if (info.dataProtectionSettings != 0x00) {
      pContext->resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                                STATUS_INVALID_FIELD);
    }

    info.lbaSize = lbaSize[info.lbaFormatIndex];

    bool ret = createNamespace(pContext->nsid, &info);

    if (ret) {
      pContext->resp.entry.dword0 = pContext->nsid;
    }
    else {
      pContext->resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                                STATUS_NAMESPACE_INSUFFICIENT_CAPACITY);
    }

    pContext->function(pContext->resp);

    free(pContext->buffer);
    delete pContext->dma;
    delete pContext;
  };

  static DMAFunction doRead = [](uint64_t, void *context) {
    RequestContext *pContext = (RequestContext *)context;

    pContext->dma->read(0, 0x1000, pContext->buffer, dmaDone, context);
  };

  if ((sel == 0x00 && req.entry.namespaceID != NSID_NONE) || sel > 0x01) {
    err = true;
    resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                    STATUS_INVALID_FIELD);
  }

  if (!err) {
    err = false;

    if (sel == 0x00) {  // Create
      uint32_t nsid = NSID_LOWEST;

      for (auto &iter : lNamespaces) {
        if (iter->getNSID() == nsid) {
          nsid++;
        }
        else {
          break;
        }
      }

      if (nsid == NSID_ALL) {
        resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                        STATUS_NAMESPACE_ID_UNAVAILABLE);
      }
      else {
        // Do not response immediately
        err = true;

        NamespaceManagementContext *pContext =
            new NamespaceManagementContext(func, resp);

        pContext->buffer = (uint8_t *)calloc(0x1000, sizeof(uint8_t));
        pContext->nsid = nsid;

        if (req.useSGL) {
          pContext->dma = new SGL(cfgdata, doRead, pContext, req.entry.data1,
                                  req.entry.data2);
        }
        else {
          pContext->dma =
              new PRPList(cfgdata, doRead, pContext, req.entry.data1,
                          req.entry.data2, (uint64_t)0x1000);
        }
      }
    }
    else {  // Delete
      bool found = destroyNamespace(req.entry.namespaceID);

      if (!found) {
        resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                        STATUS_ABORT_INVALID_NAMESPACE);
      }
    }
  }

  if (!err) {
    func(resp);
  }

  return true;
}

bool Subsystem::namespaceAttachment(SQEntryWrapper &req,
                                    RequestFunction &func) {
  CQEntryWrapper resp(req);

  uint8_t sel = req.entry.dword10 & 0x0F;
  uint32_t nsid = req.entry.namespaceID;

  debugprint(LOG_HIL_NVME, "ADMIN   | Namespace Attachment | OP %d | NSID %d",
             req.entry.dword10 & 0x0F, req.entry.namespaceID);

  static DMAFunction dmaDone = [this](uint64_t, void *context) {
    bool err = false;
    IOContext *pContext = (IOContext *)context;
    uint16_t *ctrlList = (uint16_t *)pContext->buffer;
    std::vector<uint16_t> list;

    uint8_t sel = (uint8_t)pContext->slba;
    uint32_t nsid = (uint32_t)pContext->nlb;

    for (uint16_t i = 1; i <= ctrlList[0]; i++) {
      list.push_back(ctrlList[i]);
    }

    if (list.size() > 1 && sel == 0x00) {
      err = true;
      pContext->resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                                STATUS_NAMESPACE_IS_PRIVATE);
    }
    else if (list.size() == 0) {
      err = true;
      pContext->resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                                STATUS_CONTROLLER_LIST_INVALID);
    }
    else if (list.at(0) != 0) {  // Controller ID is zero
      err = true;
      pContext->resp.makeStatus(false, false, TYPE_COMMAND_SPECIFIC_STATUS,
                                STATUS_CONTROLLER_LIST_INVALID);
    }

    if (!err) {
      if (sel == 0x00) {  // Attach
        bool exist = false;

        for (auto &iter : lNamespaces) {
          if (iter->getNSID() == nsid) {
            exist = true;

            if (iter->isAttached()) {
              err = true;
              pContext->resp.makeStatus(true, false,
                                        TYPE_COMMAND_SPECIFIC_STATUS,
                                        STATUS_NAMESPACE_ALREADY_ATTACHED);
            }
            else {
              iter->attach(true);
            }
          }
        }

        if (!exist) {
          err = true;
          pContext->resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                                    STATUS_ABORT_INVALID_NAMESPACE);
        }
      }
      else if (sel == 0x01) {  // Detach
        for (auto &iter : lNamespaces) {
          if (iter->getNSID() == nsid) {
            if (iter->isAttached()) {
              iter->attach(false);
            }
            else {
              err = true;
              pContext->resp.makeStatus(true, false,
                                        TYPE_COMMAND_SPECIFIC_STATUS,
                                        STATUS_NAMESPACE_NOT_ATTACHED);
            }
          }
        }
      }
      else {
        pContext->resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                                  STATUS_INVALID_FIELD);
      }
    }

    pContext->function(pContext->resp);

    free(pContext->buffer);
    delete pContext->dma;
    delete pContext;
  };

  static DMAFunction doRead = [](uint64_t, void *context) {
    IOContext *pContext = (IOContext *)context;

    pContext->dma->read(0, 0x1000, pContext->buffer, dmaDone, context);
  };

  IOContext *pContext = new IOContext(func, resp);

  pContext->buffer = (uint8_t *)calloc(0x1000, 1);
  pContext->slba = sel;
  pContext->nlb = nsid;

  if (req.useSGL) {
    pContext->dma =
        new SGL(cfgdata, doRead, pContext, req.entry.data1, req.entry.data2);
  }
  else {
    pContext->dma = new PRPList(cfgdata, doRead, pContext, req.entry.data1,
                                req.entry.data2, (uint64_t)0x1000);
  }

  return true;
}

bool Subsystem::formatNVM(SQEntryWrapper &req, RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  uint8_t ses = (req.entry.dword10 & 0x0E00) >> 9;
  bool pil = req.entry.dword10 & 0x0100;
  uint8_t pi = (req.entry.dword10 & 0xE0) >> 5;
  bool mset = req.entry.dword10 & 0x10;
  uint8_t lbaf = req.entry.dword10 & 0x0F;
  bool submit = true;

  debugprint(LOG_HIL_NVME, "ADMIN   | Format NVM | SES %d | NSID %d", ses,
             req.entry.namespaceID);

  static DMAFunction doFormat = [](uint64_t, void *context) {
    RequestContext *pContext = (RequestContext *)context;

    pContext->function(pContext->resp);

    delete pContext;
  };

  if (ses != 0x00 && ses != 0x01) {
    err = true;
  }
  if (pil || mset) {
    err = true;
  }
  if (pi != 0x00) {
    err = true;
  }
  if (lbaf >= nLBAFormat) {
    err = true;
  }

  if (err) {
    resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                    STATUS_INVALID_FORMAT);
  }
  else {
    // Update Namespace
    auto iter = lNamespaces.begin();
    Namespace::Information *info = nullptr;

    for (; iter != lNamespaces.end(); iter++) {
      if ((*iter)->getNSID() == req.entry.namespaceID) {
        info = (*iter)->getInfo();

        break;
      }
    }

    if (info) {
      submit = false;

      info->lbaFormatIndex = lbaf;
      info->lbaSize = lbaSize[lbaf];
      info->size = totalLogicalPages * logicalPageSize / info->lbaSize;
      info->capacity = info->size;

      // Reset health stat and set format progress
      (*iter)->format(getTick());

      info->utilization =
          pHIL->getUsedPageCount(info->range.slpn,
                                 info->range.slpn + info->range.nlp) *
          logicalPageSize / info->lbaSize;

      // Send format command to HIL
      RequestContext *pContext = new RequestContext(func, resp);
      Request *req = new Request(doFormat, pContext);

      req->range = info->range;

      DMAFunction job = [this, ses](uint64_t, void *context) {
        auto req = (Request *)context;

        pHIL->format(*req, ses == 0x01);

        delete req;
      };

      execute(CPU::NVME__SUBSYSTEM, CPU::FORMAT_NVM, job, req);
    }
    else {
      resp.makeStatus(false, false, TYPE_GENERIC_COMMAND_STATUS,
                      STATUS_ABORT_INVALID_NAMESPACE);
    }
  }

  if (submit) {
    func(resp);
  }

  return true;
}

void Subsystem::getStatList(std::vector<Stats> &list, std::string prefix) {
  Stats temp;

  temp.name = prefix + "command_count";
  temp.desc = "Total number of NVMe command handled";
  list.push_back(temp);

  pHIL->getStatList(list, prefix);
}

void Subsystem::getStatValues(std::vector<double> &values) {
  values.push_back(commandCount);

  pHIL->getStatValues(values);
}

void Subsystem::resetStatValues() {
  commandCount = 0;

  pHIL->resetStatValues();
}

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD
