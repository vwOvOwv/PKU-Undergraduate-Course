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

#include "hil/nvme/ocssd.hh"

#include "hil/nvme/controller.hh"
#include "util/algorithm.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

struct OCSSDContext {
  Request req;
  std::vector<::CPDPBP> list;
  uint64_t beginAt;
};

BlockData::_BlockData() : index(0), value(0) {}

BlockData::_BlockData(uint32_t i, uint8_t v) : index(i), value(v) {}

ChunkUpdateEntry::_ChunkUpdateEntry(ChunkDescriptor *p, uint32_t i)
    : pDesc(p), pageIdx(i) {}

OpenChannelSSD12::OpenChannelSSD12(Controller *c, ConfigData &cfg)
    : Subsystem(c, cfg),
      lastScheduled(0),
      eraseCount(0),
      readCount(0),
      writeCount(0) {
  completionEvent = allocate([this](uint64_t) { completion(); });
}

OpenChannelSSD12::~OpenChannelSSD12() {
  for (auto &iter : lNamespaces) {
    delete iter;
  }

  delete pPALOLD;
  delete pDisk;
}

void OpenChannelSSD12::init() {
  bool useMP = conf.readBoolean(CONFIG_PAL, PAL::NAND_USE_MULTI_PLANE_OP);

  param.channel = conf.readUint(CONFIG_PAL, PAL::PAL_CHANNEL);
  param.package = conf.readUint(CONFIG_PAL, PAL::PAL_PACKAGE);

  param.die = conf.readUint(CONFIG_PAL, PAL::NAND_DIE);
  param.plane = conf.readUint(CONFIG_PAL, PAL::NAND_PLANE);
  param.block = conf.readUint(CONFIG_PAL, PAL::NAND_BLOCK);
  param.page = conf.readUint(CONFIG_PAL, PAL::NAND_PAGE);
  param.pageSize = conf.readUint(CONFIG_PAL, PAL::NAND_PAGE_SIZE);
  param.superPageSize = param.pageSize;

  // Super block includes plane
  if (useMP) {
    param.superPageSize *= param.plane;
  }

  // Print super block information
  debugprint(LOG_PAL,
             "Channel |   Way   |   Die   |  Plane  |  Block  |   Page  ");
  debugprint(LOG_PAL, "%7u | %7u | %7u | %7u | %7u | %7u", param.channel,
             param.package, param.die, param.plane, param.block, param.page);
  debugprint(LOG_PAL, "Multi-plane mode %s", useMP ? "enabled" : "disabled");
  debugprint(LOG_PAL, "Page size %u -> %u", param.pageSize,
             param.superPageSize);
  debugprint(
      LOG_PAL, "Total block count %u -> %u",
      param.channel * param.package * param.die * param.plane * param.block,
      param.superBlock);

  pPALOLD = new PAL::PALOLD(param, conf);

  // Set geometry
  structure.group = param.channel;
  structure.parallelUnit = param.package;
  structure.parallelUnit *= param.die;
  structure.chunk = param.block;
  structure.chunk *= useMP ? 1 : param.plane;
  structure.chunkSize = param.page * param.superPageSize / LBA_SIZE;
  structure.writeSize = param.superPageSize / LBA_SIZE;

  // Print structure
  debugprint(LOG_HIL_NVME, "OpenChannel SSD v1.2");
  debugprint(LOG_HIL_NVME, "OpenChannel SSD Structure: (Blk size is %u)",
             LBA_SIZE);
  debugprint(LOG_HIL_NVME, "   Group    |  PU (LUN)  |   Chunk    | Chunk Size "
                           "| Write Size");
  debugprint(LOG_HIL_NVME, "       %4u |     %6u |     %6u | %6u Blk | %6u Blk",
             structure.group, structure.parallelUnit, structure.chunk,
             structure.chunkSize, structure.writeSize);
  debugprint(LOG_HIL_NVME,
             "  In total: |     %6u |     %6u | %u Blk / %" PRIu64 " Bytes",
             structure.group * structure.parallelUnit,
             structure.group * structure.parallelUnit * structure.chunk,
             structure.group * structure.parallelUnit * structure.chunk *
                 structure.chunkSize,
             (uint64_t)structure.group * structure.parallelUnit *
                 structure.chunk * structure.chunkSize * LBA_SIZE);
  debugprint(LOG_HIL_NVME, "nvm notation (old): [%u/1/%u/%u/%u/%u]",
             structure.writeSize, param.page, structure.chunk,
             structure.group * structure.parallelUnit, structure.group);
  debugprint(LOG_HIL_NVME, "nvm notation (new): [%u/%u/%u/%u/%u]",
             structure.writeSize, structure.writeSize, structure.chunk,
             structure.group * structure.parallelUnit, structure.group);

  // Calculate mask
  uint32_t lastidx = 0;
  uint64_t sum = 0;

  int sectorPerPage = param.superPageSize / LBA_SIZE;

  ppaMask.sectorMask = generateMask(sectorPerPage, lastidx);
  ppaMask.pageShift = lastidx;
  ppaMask.pageMask = generateMask(param.page, lastidx);
  ppaMask.blockShift = lastidx;
  ppaMask.blockMask = generateMask(structure.chunk, lastidx);
  ppaMask.planeShift = lastidx;
  ppaMask.planeMask = 0;  // No plane
  ppaMask.dieShift = lastidx;
  ppaMask.dieMask = 0;  // No die
  ppaMask.wayShift = lastidx;
  ppaMask.wayMask = generateMask(structure.parallelUnit, lastidx);
  ppaMask.channelShift = lastidx;
  ppaMask.channelMask = generateMask(structure.group, lastidx);

  sum ^= ppaMask.sectorMask;
  sum ^= ppaMask.pageMask;
  sum ^= ppaMask.blockMask;
  sum ^= ppaMask.planeMask;
  sum ^= ppaMask.dieMask;
  sum ^= ppaMask.wayMask;
  sum ^= ppaMask.channelMask;

  ppaMask.padding = ~sum;

  // Create default namespace
  Namespace::Information info;

  info.lbaSize = LBA_SIZE;
  info.lbaFormatIndex = 3;  // See subsystem.cc
  info.dataProtectionSettings = 0;
  info.namespaceSharingCapabilities = 0;
  info.sizeInByteL = structure.group * structure.parallelUnit * structure.chunk;
  info.size = info.sizeInByteL * structure.chunkSize;
  info.sizeInByteL *= LBA_SIZE * structure.chunkSize;
  info.sizeInByteH = 0;
  info.capacity = info.size;
  info.utilization = info.size;

  Namespace *pNS = new Namespace(this, cfgdata);

  pNS->setData(1, &info);
  pNS->attach(true);

  lNamespaces.push_back(pNS);

  // Open Disk image
  pDisk = new MemDisk();

  pDisk->open("", info.sizeInByteL, LBA_SIZE);
}

void OpenChannelSSD12::submitCommand(SQEntryWrapper &req,
                                     RequestFunction func) {
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
      case OPCODE_DEVICE_IDENTIFICATION:
        processed = deviceIdentification(req, func);
        break;
      case OPCODE_GET_BAD_BLOCK_TABLE:
        processed = getBadBlockTable(req, func);
        break;
      case OPCODE_SET_BAD_BLOCK_TABLE:
        processed = setBadBlockTable(req, func);
        break;
      default:
        resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                        STATUS_INVALID_OPCODE);
        break;
    }
  }

  // NVM commands or Namespace specific Admin commands
  if (!processed) {
    if (req.entry.namespaceID == NSID_ALL || req.entry.namespaceID == 1) {
      processed = true;

      switch (req.entry.dword0.opcode) {
        case OPCODE_READ:
          // This for automatic partition table detection
          // Just return SUCCESS, which means null data
          debugprint(LOG_HIL_NVME, "OCSSD   | READ  | Ignored");

          func(resp);
          break;
        case OPCODE_PHYSICAL_BLOCK_ERASE:
          physicalBlockErase(req, func);
          break;
        case OPCODE_PHYSICAL_PAGE_READ:
          physicalPageRead(req, func);
          break;
        case OPCODE_PHYSICAL_PAGE_WRITE:
          physicalPageWrite(req, func);
          break;
        default:
          resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                          STATUS_INVALID_OPCODE);
          processed = false;
          break;
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

void OpenChannelSSD12::getNVMCapacity(uint64_t &total, uint64_t &used) {
  total = structure.group * structure.parallelUnit * structure.chunk *
          structure.chunkSize;
  used = total;
}

uint32_t OpenChannelSSD12::validNamespaceCount() {
  return 1;
}

bool OpenChannelSSD12::parsePPA(uint64_t ppa, ::CPDPBP &addr) {
  if ((ppa & ppaMask.padding) == 0) {
    addr.Channel = (ppa & ppaMask.channelMask) >> ppaMask.channelShift;
    addr.Package = (ppa & ppaMask.wayMask) >> ppaMask.wayShift;
    addr.Die = (ppa & ppaMask.dieMask) >> ppaMask.dieShift;
    addr.Plane = (ppa & ppaMask.planeMask) >> ppaMask.planeShift;
    addr.Block = (ppa & ppaMask.blockMask) >> ppaMask.blockShift;
    addr.Page = (ppa & ppaMask.pageMask) >> ppaMask.pageShift;

    return true;
  }

  return false;
}

void OpenChannelSSD12::convertUnit(::CPDPBP &addr) {
  static bool useMP =
      conf.readBoolean(CONFIG_PAL, PAL::NAND_USE_MULTI_PLANE_OP);

  addr.Die = addr.Package % param.die;
  addr.Package = addr.Package / param.die;
  if (useMP) {
    addr.Plane = 0;
  }
  else {
    addr.Plane = addr.Block % param.plane;
    addr.Block = addr.Block / param.plane;
  }
}

void OpenChannelSSD12::mergeList(std::vector<uint64_t> &lbaList,
                                 std::vector<::CPDPBP> &list, bool block) {
  ::CPDPBP temp;

  list.clear();

  for (auto &lba : lbaList) {
    if (parsePPA(lba, temp)) {
      convertUnit(temp);

      if (list.size() > 0) {
        auto &back = list.back();

        if (back.Channel == temp.Channel && back.Package == temp.Package &&
            back.Die == temp.Die && back.Plane == temp.Plane &&
            back.Block == temp.Block && (block || back.Page == temp.Page)) {
          continue;
        }
      }

      list.push_back(temp);
    }
  }
}

void OpenChannelSSD12::updateCompletion() {
  if (completionQueue.size() > 0) {
    if (lastScheduled != completionQueue.top().finishedAt) {
      lastScheduled = completionQueue.top().finishedAt;

      if (lastScheduled < getTick()) {
        warn("Invalid tick %" PRIu64, lastScheduled);
        lastScheduled = getTick();
      }

      schedule(completionEvent, lastScheduled);
    }
  }
}

void OpenChannelSSD12::completion() {
  uint64_t tick = getTick();

  while (completionQueue.size() > 0) {
    auto &req = completionQueue.top();

    if (req.finishedAt <= tick) {
      req.function(tick, req.context);

      completionQueue.pop();
    }
    else {
      break;
    }
  }

  updateCompletion();
}

bool OpenChannelSSD12::deviceIdentification(SQEntryWrapper &req,
                                            RequestFunction &func) {
  CQEntryWrapper resp(req);
  PAL::NAND_TYPE type =
      (PAL::NAND_TYPE)conf.readInt(CONFIG_PAL, PAL::NAND_FLASH_TYPE);

  RequestContext *pContext = new RequestContext(func, resp);

  uint8_t *data = (uint8_t *)calloc(0x1000, sizeof(uint8_t));
  debugprint(LOG_HIL_NVME, "OCSSD   | Device Identification");

  // Version
  data[0x00] = 0x01;  // OpenChannel SSD v1.x

  // Vendor NVM opcode command set
  data[0x01] = 0x00;

  // Configuration groups
  data[0x02] = 0x01;
  data[0x03] = 0x00;  // Reserved

  // Device capabilities and feature support
  // [Bits ] Description
  // [31:02] Reserved
  // [01:01] 1 for support hybrid command
  // [00:00] 1 for support bad block table management
  data[0x04] = 0x01;
  data[0x05] = 0x00;
  data[0x06] = 0x00;
  data[0x07] = 0x00;

  // Device operating mode
  // [Bits ] Description
  // [31:02] Reserved
  // [01:01] 1 for ECC is provided by host
  // [00:00] 1 for operating in hybrid mode
  data[0x08] = 0x00;
  data[0x09] = 0x00;
  data[0x0A] = 0x00;
  data[0x0B] = 0x00;

  // PPA Format
  {
    data[0x0C] = ppaMask.channelShift;
    data[0x0D] = popcount(ppaMask.channelMask);
    data[0x0E] = __builtin_ffsl(ppaMask.wayMask | ppaMask.dieMask) - 1;
    data[0x0F] = popcount(ppaMask.wayMask | ppaMask.dieMask);
    data[0x10] = ppaMask.planeShift;
    data[0x11] = popcount(ppaMask.planeMask);
    data[0x12] = ppaMask.blockShift;
    data[0x13] = popcount(ppaMask.blockMask);
    data[0x14] = ppaMask.pageShift;
    data[0x15] = popcount(ppaMask.pageMask);
    data[0x16] = 0;
    data[0x17] = popcount(ppaMask.sectorMask);
  }

  // Description of 1st group
  {
    // Media type
    data[0x100] = 0x00;  // NAND Flash

    // Flash type
    switch (type) {
      case PAL::NAND_SLC:
        data[0x101] = 0;
        break;
      case PAL::NAND_MLC:
        data[0x101] = 1;
        break;
      default:
        panic("Unsupported NAND type in OpenChannel SSD v1.2");
        break;
    }

    // Number of channels
    data[0x104] = structure.group;

    // Number of LUNs
    data[0x105] = structure.parallelUnit;

    // Number of planes
    data[0x106] = 1;

    // Number of blocks
    *(uint16_t *)(data + 0x108) = structure.chunk;

    // Number of pages
    *(uint16_t *)(data + 0x10A) = param.page;

    // Size of page
    *(uint16_t *)(data + 0x10C) = param.superPageSize;

    // Size of sector
    *(uint16_t *)(data + 0x10E) = LBA_SIZE;

    // Sector meta size
    *(uint16_t *)(data + 0x110) = 0;

    // Performance Related Metrics
    PAL::Config::NANDTiming *pNANDTiming = conf.getNANDTiming();

    // Typical page read time
    *(uint32_t *)(data + 0x114) = pNANDTiming->msb.read / 1000;

    // Typical page write time
    *(uint32_t *)(data + 0x11C) = pNANDTiming->msb.write / 1000;

    if (conf.readInt(CONFIG_PAL, PAL::NAND_FLASH_TYPE) != PAL::NAND_SLC) {
      // Max page read time
      *(uint32_t *)(data + 0x118) = pNANDTiming->lsb.read / 1000;

      // Max page write time
      *(uint32_t *)(data + 0x120) = pNANDTiming->lsb.write / 1000;
    }
    else {
      // Max page read time
      *(uint32_t *)(data + 0x118) = pNANDTiming->msb.read / 1000;

      // Max page write time
      *(uint32_t *)(data + 0x120) = pNANDTiming->msb.write / 1000;
    }

    // Typical block erase time
    *(uint32_t *)(data + 0x124) = pNANDTiming->erase / 1000;

    // Max block erase time
    *(uint32_t *)(data + 0x128) = pNANDTiming->erase / 1000;

    // Multi-plane operations
    // [Bits ] Description
    // [31:19] Reserved
    // [18:18] 1 for Quad plane erase
    // [17:17] 1 for Dual plane erase
    // [16:16] 1 for Single plane erase
    // [15:11] Reserved
    // [10:10] 1 for Quad plane write
    // [09:09] 1 for Dual plane write
    // [08:08] 1 for Single plane write
    // [07:03] Reserved
    // [02:02] 1 for Quad plane read
    // [01:01] 1 for Dual plane read
    // [00:00] 1 for Single plane read
    data[0x12C] = 0x00;
    data[0x12D] = 0x00;
    data[0x12E] = 0x00;
    data[0x12F] = 0x00;

    // Media and Controller Capabilities
    // [Bits ] Description
    // [31:04] Reserved
    // [03:03] 1 for support Encryption
    // [02:02] 1 for support Scramble On/Off
    // [01:01] 1 for support Command suspension
    // [00:00] 1 for support SLC mode
    data[0x130] = 0x01;
    data[0x131] = 0x00;
    data[0x132] = 0x00;
    data[0x133] = 0x00;

    // Channel parallelism
    *(uint16_t *)(data + 0x134) = 1;

    // Media type specific data
    memset(data + 0x140, 0, 896);

    if (type == PAL::NAND_MLC) {
      *(uint16_t *)(data + 0x148) = 1;
      data[0x14A] = 0x11;
    }
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

  pContext->buffer = data;

  if (req.useSGL) {
    pContext->dma =
        new SGL(cfgdata, doWrite, pContext, req.entry.data1, req.entry.data2);
  }
  else {
    pContext->dma = new PRPList(cfgdata, doWrite, pContext, req.entry.data1,
                                req.entry.data2, (uint64_t)0x1000);
  }

  return true;
}

bool OpenChannelSSD12::setBadBlockTable(SQEntryWrapper &req,
                                        RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  IOContext *pContext = new IOContext(func, resp);

  uint16_t nppa = (req.entry.dword12 & 0xFFFF) + 1;
  uint8_t val = (req.entry.dword12 & 0xFF0000) >> 16;
  uint64_t ppa = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;

  debugprint(LOG_HIL_NVME, "OCSSD   | Set Badblock Table");

  if (nppa < 1) {
    err = true;
    resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                    STATUS_INVALID_FIELD);
  }

  if (!err) {
    pContext->slba = ppa;
    pContext->nlb = nppa;

    DMAFunction doRead = [this](uint64_t, void *context) {
      DMAFunction dmaDone = [this](uint64_t, void *context) {
        IOContext *pContext = (IOContext *)context;

        std::vector<uint64_t> ppaList;
        CPDPBP addr;
        uint8_t val = pContext->tick;

        if (pContext->nlb == 1) {
          ppaList.push_back(pContext->slba);
        }
        else {
          for (uint8_t i = 0; i < pContext->nlb; i++) {
            ppaList.push_back(*(uint64_t *)(pContext->buffer + i * 8));
          }

          free(pContext->buffer);
        }

        for (auto &iter : ppaList) {
          if (parsePPA(iter, addr)) {
            auto data = badBlocks.find(iter);

            if (data == badBlocks.end()) {
              // Push new value
              if (val != 0) {
                badBlocks.insert({iter, BlockData(addr.Block, val)});
              }
            }
            else {
              if (val == 0) {
                badBlocks.erase(data);
              }
              else {
                data->second.index = addr.Block;
                data->second.value = val;
              }
            }

            convertUnit(addr);

            debugprint(
                LOG_HIL_NVME,
                "OCSSD   | C %5u | W %5u | D %5u | P %5u | B %5u | P %5u",
                addr.Channel, addr.Package, addr.Die, addr.Plane, addr.Block,
                addr.Page);
          }
        }

        pContext->function(pContext->resp);

        delete pContext->dma;
        delete pContext;
      };

      IOContext *pContext = (IOContext *)context;

      pContext->buffer = (uint8_t *)calloc(pContext->nlb * 8, 1);
      pContext->dma->read(0, pContext->nlb * 8, pContext->buffer, dmaDone,
                          context);
    };

    pContext->tick = val;
    pContext->dma = new PRPList(cfgdata, doRead, pContext, pContext->slba,
                                (uint64_t)nppa * 8, true);
  }

  if (err) {
    func(resp);
  }

  return true;
}

bool OpenChannelSSD12::getBadBlockTable(SQEntryWrapper &req,
                                        RequestFunction &func) {
  CQEntryWrapper resp(req);
  CPDPBP addr;
  uint64_t ppa = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;
  uint64_t prpsize = 64 + structure.chunk;

  RequestContext *pContext = new RequestContext(func, resp);

  debugprint(LOG_HIL_NVME, "OCSSD   | Get Badblock Table");

  uint64_t mask = ppaMask.channelMask | ppaMask.wayMask | ppaMask.dieMask;
  uint8_t *data = (uint8_t *)calloc(prpsize, 1);

  if (parsePPA(ppa, addr) && data) {
    convertUnit(addr);

    debugprint(LOG_HIL_NVME,
               "OCSSD   | C %5u | W %5u | D %5u | P %5u | B %5u | P %5u",
               addr.Channel, addr.Package, addr.Die, addr.Plane, addr.Block,
               addr.Page);

    // Table ID
    memcpy(data, "BBLT", 4);
    // Version number
    *(uint16_t *)(data + 0x04) = 1;
    // Total number of blocks in this LUN
    *(uint32_t *)(data + 0x0C) = prpsize - 64;
    // Number of factory bad blocks
    *(uint32_t *)(data + 0x10) = 0;
    // Number of grown bad blocks
    *(uint32_t *)(data + 0x14) = 0;
    // Number of device reserved blocks
    *(uint32_t *)(data + 0x18) = 0;
    // Number of host reserved blocks
    *(uint32_t *)(data + 0x1C) = 0;
    memset(data + 0x20, 0, 32);  // Reserved
    // Block table
    memset(data + 0x40, 0, prpsize - 64);

    for (auto iter = badBlocks.begin(); iter != badBlocks.end(); iter++) {
      if ((iter->first & mask) == (ppa & mask)) {
        data[0x40 + iter->second.index] = iter->second.value;
      }
    }

    static DMAFunction dmaDone = [](uint64_t, void *context) {
      RequestContext *pContext = (RequestContext *)context;

      pContext->function(pContext->resp);

      free(pContext->buffer);
      delete pContext->dma;
      delete pContext;
    };
    DMAFunction doWrite = [prpsize](uint64_t, void *context) {
      RequestContext *pContext = (RequestContext *)context;

      pContext->dma->write(0, prpsize, pContext->buffer, dmaDone, context);
    };

    pContext->buffer = data;

    if (req.useSGL) {
      pContext->dma =
          new SGL(cfgdata, doWrite, pContext, req.entry.data1, req.entry.data2);
    }
    else {
      pContext->dma = new PRPList(cfgdata, doWrite, pContext, req.entry.data1,
                                  req.entry.data2, prpsize);
    }
  }
  else {
    resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                    STATUS_INVALID_FIELD);

    func(resp);
  }

  return true;
}

void OpenChannelSSD12::physicalBlockErase(SQEntryWrapper &req,
                                          RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  IOContext *pContext = new IOContext(func, resp);

  uint8_t nppa = (req.entry.dword12 & 0x3F) + 1;
  uint64_t ppa = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;

  eraseCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | Physical Block Erase  | %d lbas", nppa);

  if (nppa < 1) {
    err = true;
    resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                    STATUS_INVALID_FIELD);
  }

  if (!err) {
    pContext->slba = ppa;
    pContext->nlb = nppa;

    DMAFunction doErase = [this](uint64_t now, void *context) {
      DMAFunction dmaDone = [this](uint64_t now, void *context) {
        DMAFunction nandDone = [](uint64_t now, void *context) {
          IOContext *pContext = (IOContext *)context;

          debugprint(LOG_HIL_NVME,
                     "OCSSD   | Physical Block Erase  | %" PRIu64 " - %" PRIu64
                     " (%" PRIu64 ")",
                     pContext->beginAt, now, now - pContext->beginAt);

          pContext->function(pContext->resp);

          delete pContext;
        };

        IOContext *pContext = (IOContext *)context;

        std::vector<uint64_t> ppaList;
        std::vector<::CPDPBP> list;
        uint64_t beginAt;
        uint64_t finishedAt = now;

        if (pContext->nlb == 1) {
          ppaList.push_back(pContext->slba);
        }
        else {
          for (uint8_t i = 0; i < pContext->nlb; i++) {
            ppaList.push_back(*(uint64_t *)(pContext->buffer + i * 8));
          }

          free(pContext->buffer);
        }

        for (uint64_t i = 0; i < pContext->nlb; i++) {
          pDisk->erase(ppaList.at(i), 1);
        }

        mergeList(ppaList, list, true);

        for (auto &iter : list) {
          beginAt = now;

          pPALOLD->erase(iter, beginAt);

          finishedAt = MAX(finishedAt, beginAt);
        }

        Request req = Request(nandDone, context);
        req.finishedAt = finishedAt;

        completionQueue.push(req);
        updateCompletion();

        delete pContext->dma;
      };

      IOContext *pContext = (IOContext *)context;

      if (pContext->nlb > 1) {
        pContext->buffer = (uint8_t *)calloc(pContext->nlb * 8, 1);
        pContext->dma->read(0, pContext->nlb * 8, pContext->buffer, dmaDone,
                            context);
      }
      else {
        dmaDone(now, context);
      }
    };

    CPUContext *pCPU = new CPUContext(doErase, pContext, CPU::NVME__OCSSD,
                                      CPU::PHYSICAL_BLOCK_ERASE);

    pContext->beginAt = getTick();

    if (pContext->nlb > 1) {
      pContext->dma = new PRPList(cfgdata, cpuHandler, pCPU, pContext->slba,
                                  (uint64_t)nppa * 8, true);
    }
    else {
      pContext->dma = nullptr;

      cpuHandler(getTick(), pCPU);
    }
  }

  if (err) {
    func(resp);
  }
}

void OpenChannelSSD12::physicalPageWrite(SQEntryWrapper &req,
                                         RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  VectorContext *pContext = new VectorContext(func, resp);

  uint8_t nppa = (req.entry.dword12 & 0x3F) + 1;
  uint64_t ppa = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;

  writeCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | Physical Page Write  | %d lbas", nppa);

  if (nppa < 1) {
    err = true;
    resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                    STATUS_INVALID_FIELD);
  }

  if (!err) {
    pContext->slba = ppa;
    pContext->nlb = nppa;

    DMAFunction doDMA = [this](uint64_t now, void *context) {
      DMAFunction doWrite = [this](uint64_t, void *context) {
        DMAFunction dmaDone = [this](uint64_t now, void *context) {
          DMAFunction nandDone = [](uint64_t now, void *context) {
            VectorContext *pContext = (VectorContext *)context;

            debugprint(LOG_HIL_NVME,
                       "OCSSD   | Physical Page Write  | %" PRIu64 " - %" PRIu64
                       " (%" PRIu64 ")",
                       pContext->beginAt, now, now - pContext->beginAt);

            pContext->resp.entry.dword0 = 0xFFFFFFFF;
            pContext->resp.entry.reserved = 0xFFFFFFFF;

            pContext->function(pContext->resp);

            delete pContext;
          };

          VectorContext *pContext = (VectorContext *)context;

          uint64_t beginAt;
          uint64_t finishedAt = now;
          std::vector<::CPDPBP> list;

          for (uint64_t i = 0; i < pContext->nlb; i++) {
            pDisk->write(pContext->lbaList.at(i), 1,
                         pContext->buffer + i * LBA_SIZE);
          }

          mergeList(pContext->lbaList, list);

          for (auto &iter : list) {
            beginAt = now;

            pPALOLD->write(iter, beginAt);

            finishedAt = MAX(finishedAt, beginAt);
          }

          Request req = Request(nandDone, context);
          req.finishedAt = finishedAt;

          completionQueue.push(req);
          updateCompletion();

          free(pContext->buffer);
          delete pContext->dma;
        };

        VectorContext *pContext = (VectorContext *)context;

        if (pContext->nlb == 1) {
          pContext->lbaList.push_back(pContext->slba);
        }
        else {
          for (uint8_t i = 0; i < pContext->nlb; i++) {
            pContext->lbaList.push_back(
                *(uint64_t *)(pContext->buffer + i * 8));
          }

          free(pContext->buffer);
        }

        pContext->buffer = (uint8_t *)calloc(pContext->nlb * LBA_SIZE, 1);
        pContext->dma->read(0, pContext->nlb * LBA_SIZE, pContext->buffer,
                            dmaDone, context);
      };

      VectorContext *pContext = (VectorContext *)context;

      if (pContext->nlb > 1) {
        pContext->buffer = (uint8_t *)calloc(pContext->nlb * 8, 1);

        cfgdata.pInterface->dmaRead(pContext->slba, pContext->nlb * 8,
                                    pContext->buffer, doWrite, context);
      }
      else {
        doWrite(now, context);
      }
    };

    CPUContext *pCPU = new CPUContext(doDMA, pContext, CPU::NVME__OCSSD,
                                      CPU::PHYSICAL_PAGE_WRITE);

    pContext->beginAt = getTick();

    if (req.useSGL) {
      pContext->dma =
          new SGL(cfgdata, cpuHandler, pCPU, req.entry.data1, req.entry.data2);
    }
    else {
      pContext->dma = new PRPList(cfgdata, cpuHandler, pCPU, req.entry.data1,
                                  req.entry.data2, (uint64_t)nppa * LBA_SIZE);
    }
  }

  if (err) {
    func(resp);
  }
}

void OpenChannelSSD12::physicalPageRead(SQEntryWrapper &req,
                                        RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  VectorContext *pContext = new VectorContext(func, resp);

  uint8_t nppa = (req.entry.dword12 & 0x3F) + 1;
  uint64_t ppa = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;

  readCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | Physical Page Read   | %d lbas", nppa);

  if (nppa < 1) {
    err = true;
    resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                    STATUS_INVALID_FIELD);
  }

  if (!err) {
    pContext->slba = ppa;
    pContext->nlb = nppa;

    DMAFunction doDMA = [this](uint64_t now, void *context) {
      DMAFunction doRead = [this](uint64_t now, void *context) {
        DMAFunction nandDone = [](uint64_t, void *context) {
          DMAFunction dmaDone = [](uint64_t now, void *context) {
            VectorContext *pContext = (VectorContext *)context;

            debugprint(LOG_HIL_NVME,
                       "OCSSD   | Physical Block Read   | %" PRIu64
                       " - %" PRIu64 " (%" PRIu64 ")",
                       pContext->beginAt, now, now - pContext->beginAt);

            pContext->resp.entry.dword0 = 0xFFFFFFFF;
            pContext->resp.entry.reserved = 0xFFFFFFFF;

            pContext->function(pContext->resp);

            free(pContext->buffer);
            delete pContext->dma;
            delete pContext;
          };

          VectorContext *pContext = (VectorContext *)context;

          pContext->dma->write(0, pContext->nlb * LBA_SIZE, pContext->buffer,
                               dmaDone, context);
        };

        VectorContext *pContext = (VectorContext *)context;

        std::vector<::CPDPBP> list;
        uint64_t beginAt;
        uint64_t finishedAt = now;

        if (pContext->nlb == 1) {
          pContext->lbaList.push_back(pContext->slba);
        }
        else {
          for (uint8_t i = 0; i < pContext->nlb; i++) {
            pContext->lbaList.push_back(
                *(uint64_t *)(pContext->buffer + i * 8));
          }

          free(pContext->buffer);
        }

        pContext->buffer = (uint8_t *)calloc(pContext->nlb * LBA_SIZE, 1);

        for (uint64_t i = 0; i < pContext->nlb; i++) {
          pDisk->read(pContext->lbaList.at(i), 1,
                      pContext->buffer + i * LBA_SIZE);
        }

        mergeList(pContext->lbaList, list);

        for (auto &iter : list) {
          beginAt = now;

          pPALOLD->read(iter, beginAt);

          finishedAt = MAX(finishedAt, beginAt);
        }

        Request req = Request(nandDone, context);
        req.finishedAt = finishedAt;

        completionQueue.push(req);
        updateCompletion();
      };

      VectorContext *pContext = (VectorContext *)context;

      if (pContext->nlb > 1) {
        pContext->buffer = (uint8_t *)calloc(pContext->nlb * 8, 1);

        cfgdata.pInterface->dmaRead(pContext->slba, pContext->nlb * 8,
                                    pContext->buffer, doRead, context);
      }
      else {
        doRead(now, context);
      }
    };

    CPUContext *pCPU = new CPUContext(doDMA, pContext, CPU::NVME__OCSSD,
                                      CPU::PHYSICAL_PAGE_READ);

    pContext->beginAt = getTick();

    if (req.useSGL) {
      pContext->dma =
          new SGL(cfgdata, cpuHandler, pCPU, req.entry.data1, req.entry.data2);
    }
    else {
      pContext->dma = new PRPList(cfgdata, cpuHandler, pCPU, req.entry.data1,
                                  req.entry.data2, (uint64_t)nppa * LBA_SIZE);
    }
  }

  if (err) {
    func(resp);
  }
}

void OpenChannelSSD12::getStatList(std::vector<Stats> &list,
                                   std::string prefix) {
  Stats temp;

  temp.name = prefix + "command_count";
  temp.desc = "Total number of OCSSD command handled";
  list.push_back(temp);

  temp.name = prefix + "erase";
  temp.desc = "Total number of Physical Block Erase command";
  list.push_back(temp);

  temp.name = prefix + "read";
  temp.desc = "Total number of Physical Page Read command";
  list.push_back(temp);

  temp.name = prefix + "write";
  temp.desc = "Total number of Physical Page Write command";
  list.push_back(temp);

  pPALOLD->getStatList(list, prefix + "pal.");
}

void OpenChannelSSD12::getStatValues(std::vector<double> &values) {
  values.push_back(commandCount);
  values.push_back(eraseCount);
  values.push_back(readCount);
  values.push_back(writeCount);

  pPALOLD->getStatValues(values);
}

void OpenChannelSSD12::resetStatValues() {
  commandCount = 0;
  eraseCount = 0;
  readCount = 0;
  writeCount = 0;

  pPALOLD->resetStatValues();
}

OpenChannelSSD20::OpenChannelSSD20(Controller *c, ConfigData &cfg)
    : OpenChannelSSD12(c, cfg),
      pDescriptor(nullptr),
      vectorEraseCount(0),
      vectorReadCount(0),
      vectorWriteCount(0) {}

OpenChannelSSD20::~OpenChannelSSD20() {
  free(pDescriptor);
}

void OpenChannelSSD20::init() {
  bool useMP = conf.readBoolean(CONFIG_PAL, PAL::NAND_USE_MULTI_PLANE_OP);

  param.channel = conf.readUint(CONFIG_PAL, PAL::PAL_CHANNEL);
  param.package = conf.readUint(CONFIG_PAL, PAL::PAL_PACKAGE);

  param.die = conf.readUint(CONFIG_PAL, PAL::NAND_DIE);
  param.plane = conf.readUint(CONFIG_PAL, PAL::NAND_PLANE);
  param.block = conf.readUint(CONFIG_PAL, PAL::NAND_BLOCK);
  param.page = conf.readUint(CONFIG_PAL, PAL::NAND_PAGE);
  param.pageSize = conf.readUint(CONFIG_PAL, PAL::NAND_PAGE_SIZE);
  param.superPageSize = param.pageSize;

  // Super block includes plane
  if (useMP) {
    param.superPageSize *= param.plane;
  }

  // Print super block information
  debugprint(LOG_PAL,
             "Channel |   Way   |   Die   |  Plane  |  Block  |   Page  ");
  debugprint(LOG_PAL, "%7u | %7u | %7u | %7u | %7u | %7u", param.channel,
             param.package, param.die, param.plane, param.block, param.page);
  debugprint(LOG_PAL, "Multi-plane mode %s", useMP ? "enabled" : "disabled");
  debugprint(LOG_PAL, "Page size %u -> %u", param.pageSize,
             param.superPageSize);
  debugprint(
      LOG_PAL, "Total block count %u -> %u",
      param.channel * param.package * param.die * param.plane * param.block,
      param.superBlock);

  pPALOLD = new PAL::PALOLD(param, conf);

  // Set geometry
  structure.group = param.channel;
  structure.parallelUnit = param.package;
  structure.parallelUnit *= param.die;
  structure.chunk = param.block;
  structure.chunk *= useMP ? 1 : param.plane;
  structure.chunkSize = param.page * param.superPageSize / LBA_SIZE;
  structure.writeSize = param.superPageSize / LBA_SIZE;

  // Print structure
  debugprint(LOG_HIL_NVME, "OpenChannel SSD v2.0");
  debugprint(LOG_HIL_NVME, "OpenChannel SSD Structure: (Blk size is %u)",
             LBA_SIZE);
  debugprint(LOG_HIL_NVME, "   Group    |  PU (LUN)  |   Chunk    | Chunk Size "
                           "| Write Size");
  debugprint(LOG_HIL_NVME, "       %4u |     %6u |     %6u | %6u Blk | %6u Blk",
             structure.group, structure.parallelUnit, structure.chunk,
             structure.chunkSize, structure.writeSize);
  debugprint(LOG_HIL_NVME,
             "  In total: |     %6u |     %6u | %u Blk / %" PRIu64 " Bytes",
             structure.group * structure.parallelUnit,
             structure.group * structure.parallelUnit * structure.chunk,
             structure.group * structure.parallelUnit * structure.chunk *
                 structure.chunkSize,
             (uint64_t)structure.group * structure.parallelUnit *
                 structure.chunk * structure.chunkSize * LBA_SIZE);
  debugprint(LOG_HIL_NVME, "nvm notation (old): [%u/1/%u/%u/%u/%u]",
             structure.writeSize, param.page, structure.chunk,
             structure.group * structure.parallelUnit, structure.group);
  debugprint(LOG_HIL_NVME, "nvm notation (new): [%u/%u/%u/%u/%u]",
             structure.writeSize, structure.writeSize, structure.chunk,
             structure.group * structure.parallelUnit, structure.group);

  // Calculate mask
  uint32_t lastidx = 0;

  ppaMask.sectorMask = generateMask(structure.chunkSize, lastidx);
  ppaMask.blockShift = lastidx;
  ppaMask.blockMask = generateMask(structure.chunk, lastidx);
  ppaMask.wayShift = lastidx;
  ppaMask.wayMask = generateMask(structure.parallelUnit, lastidx);
  ppaMask.channelShift = lastidx;
  ppaMask.channelMask = generateMask(structure.group, lastidx);

  // Chunk Descriptor
  descriptorLength = structure.group * structure.parallelUnit * structure.chunk;
  pDescriptor =
      (ChunkDescriptor *)calloc(descriptorLength, sizeof(ChunkDescriptor));

  for (uint32_t g = 0; g < structure.group; g++) {
    for (uint32_t p = 0; p < structure.parallelUnit; p++) {
      for (uint32_t c = 0; c < structure.chunk; c++) {
        auto desc = getChunkDescriptor(g, p, c);

        desc->chunkState = FREE;
        desc->chunkType = 0x01;  // Force sequential write
        desc->slba = makeLBA(g, p, c, 0);
        desc->nlb = structure.chunkSize;
      }
    }
  }

  // Create default namespace
  Namespace::Information info;

  info.lbaSize = LBA_SIZE;
  info.lbaFormatIndex = 3;  // See subsystem.cc
  info.dataProtectionSettings = 0;
  info.namespaceSharingCapabilities = 0;
  info.sizeInByteL = structure.group * structure.parallelUnit * structure.chunk;
  info.size = info.sizeInByteL * structure.chunkSize;
  info.sizeInByteL *= LBA_SIZE * structure.chunkSize;
  info.sizeInByteH = 0;
  info.capacity = info.size;
  info.utilization = info.size;

  Namespace *pNS = new Namespace(this, cfgdata);

  pNS->setData(1, &info);
  pNS->attach(true);

  lNamespaces.push_back(pNS);

  // Open Disk image
  pDisk = new MemDisk();

  pDisk->open("", info.sizeInByteL, LBA_SIZE);
}

void OpenChannelSSD20::submitCommand(SQEntryWrapper &req,
                                     RequestFunction func) {
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
      case OPCODE_GEOMETRY:
        processed = geometry(req, func);
        break;
      default:
        resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                        STATUS_INVALID_OPCODE);
        break;
    }
  }

  // NVM commands or Namespace specific Admin commands
  if (!processed) {
    if (req.entry.namespaceID == NSID_ALL || req.entry.namespaceID == 1) {
      processed = true;

      switch (req.entry.dword0.opcode) {
        case OPCODE_READ:
          read(req, func);
          break;
        case OPCODE_WRITE:
          write(req, func);
          break;
        case OPCODE_DATASET_MANAGEMEMT:
          datasetManagement(req, func);
          break;
        case OPCODE_VECTOR_CHUNK_RESET:
          vectorChunkReset(req, func);
          break;
        case OPCODE_VECTOR_CHUNK_READ:
          vectorChunkRead(req, func);
          break;
        case OPCODE_VECTOR_CHUNK_WRITE:
          vectorChunkWrite(req, func);
          break;
        default:
          resp.makeStatus(true, false, TYPE_GENERIC_COMMAND_STATUS,
                          STATUS_INVALID_OPCODE);
          processed = false;
          break;
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

ChunkDescriptor *OpenChannelSSD20::getChunkDescriptor(uint32_t g, uint32_t p,
                                                      uint32_t c) {
  return &pDescriptor[g * structure.parallelUnit * structure.chunk +
                      p * structure.chunk + c];
}

uint64_t OpenChannelSSD20::makeLBA(uint32_t g, uint32_t p, uint32_t c,
                                   uint32_t l) {
  uint64_t ret = 0;

  ret = l & ppaMask.sectorMask;
  ret |= (c << ppaMask.blockShift) & ppaMask.blockMask;
  ret |= (p << ppaMask.wayShift) & ppaMask.wayMask;
  ret |= (g << ppaMask.channelShift) & ppaMask.channelMask;

  return ret;
}

void OpenChannelSSD20::parseLBA(uint64_t lba, uint32_t &g, uint32_t &p,
                                uint32_t &c, uint32_t &l) {
  l = lba & ppaMask.sectorMask;
  c = (lba & ppaMask.blockMask) >> ppaMask.blockShift;
  p = (lba & ppaMask.wayMask) >> ppaMask.wayShift;
  g = (lba & ppaMask.channelMask) >> ppaMask.channelShift;
}

void OpenChannelSSD20::convertUnit(std::vector<uint64_t> &lbaList,
                                   std::vector<::CPDPBP> &list,
                                   std::vector<ChunkUpdateEntry> &chunk,
                                   bool block, bool mode) {
  static bool useMP =
      conf.readBoolean(CONFIG_PAL, PAL::NAND_USE_MULTI_PLANE_OP);

  list.clear();
  chunk.clear();

  // Optimize this
  auto convert = [&](uint64_t lba) {
    ::CPDPBP temp;
    uint32_t g, p, c, l;

    parseLBA(lba, g, p, c, l);

    // Group -> [Channel]
    // Parallel Unit -> [Package][Die]
    // Chunk -> [Block][Plane]
    temp.Channel = g;
    temp.Package = p / param.die;
    temp.Die = p % param.die;

    if (useMP) {
      temp.Plane = 0;
      temp.Block = c;
    }
    else {
      temp.Plane = c % param.plane;
      temp.Block = c / param.plane;
    }

    temp.Page = l / structure.writeSize;

    if (list.size() > 0) {
      auto &back = list.back();

      if (back.Channel == temp.Channel && back.Package == temp.Package &&
          back.Die == temp.Die && back.Plane == temp.Plane &&
          back.Block == temp.Block && (block || back.Page == temp.Page)) {
        return;
      }
    }

    chunk.push_back(ChunkUpdateEntry(getChunkDescriptor(g, p, c), temp.Page));
    list.push_back(temp);
  };

  if (mode) {
    for (auto &lba : lbaList) {
      convert(lba);
    }
  }
  else {
    for (uint64_t lba = lbaList.front(); lba < lbaList.back(); lba++) {
      convert(lba);
    }
  }
}

void OpenChannelSSD20::readInternal(std::vector<uint64_t> &lbaList,
                                    DMAFunction &func, void *context,
                                    bool mode) {
  OCSSDContext *pContext = new OCSSDContext();
  std::vector<ChunkUpdateEntry> chunks;
  bool err = false;
  uint64_t idx = 1;

  pContext->req = Request(func, context);
  pContext->beginAt =
      getTick() + applyLatency(CPU::NVME__OCSSD, CPU::CONVERT_UNIT);

  convertUnit(lbaList, pContext->list, chunks, false, mode);

  // Update ChunkDescriptor
  for (auto &iter : chunks) {
    // Check block status
    if (iter.pDesc->chunkState == OFFLINE) {
      warn("Reading dead block");
      ((IOContext *)context)
          ->resp.makeStatus(false, false, TYPE_MEDIA_AND_DATA_INTEGRITY_ERROR,
                            STATUS_DEALLOCATED_OR_UNWRITTEN_LOGICAL_BLOCK);
    }

    if (err) {
      if (idx <= 0xFFFFFFFF) {
        ((IOContext *)context)->resp.entry.dword0 |= idx;
      }
      else {
        ((IOContext *)context)->resp.entry.reserved |= (idx >> 32);
      }
    }

    idx <<= 1;
  }

  if (!err) {
    ((IOContext *)context)->resp.entry.dword0 = 0xFFFFFFFF;
    ((IOContext *)context)->resp.entry.reserved = 0xFFFFFFFF;
  }
  else {
    // Remove invalid LBAs
    uint64_t mask = ((IOContext *)context)->resp.entry.reserved;
    idx = 1;

    mask <<= 32;
    mask |= ((IOContext *)context)->resp.entry.dword0;

    for (auto iter = pContext->list.begin(); iter != pContext->list.end();) {
      if (mask & idx) {
        iter = pContext->list.erase(iter);
      }
      else {
        iter++;
      }

      idx <<= 1;
    }
  }

  DMAFunction doRead = [this](uint64_t tick, void *context) {
    auto pContext = (OCSSDContext *)context;

    tick = MAX(tick, pContext->beginAt);

    uint64_t beginAt;
    uint64_t finishedAt = tick;

    for (auto &iter : pContext->list) {
      beginAt = pContext->beginAt;

      pPALOLD->read(iter, beginAt);

      finishedAt = MAX(finishedAt, beginAt);
    }

    pContext->req.finishedAt = finishedAt;
    completionQueue.push(pContext->req);

    updateCompletion();

    delete pContext;
  };

  execute(CPU::NVME__OCSSD, CPU::READ_INTERNAL, doRead, pContext);
}

void OpenChannelSSD20::writeInternal(std::vector<uint64_t> &lbaList,
                                     DMAFunction &func, void *context,
                                     bool mode) {
  OCSSDContext *pContext = new OCSSDContext();
  std::vector<ChunkUpdateEntry> chunks;
  bool err = false;
  uint64_t idx = 1;

  pContext->req = Request(func, context);
  pContext->beginAt =
      getTick() + applyLatency(CPU::NVME__OCSSD, CPU::CONVERT_UNIT);

  convertUnit(lbaList, pContext->list, chunks, false, mode);

  // Update ChunkDescriptor
  for (auto &iter : chunks) {
    // Check block status
    if (iter.pDesc->chunkState == OFFLINE) {
      err = true;
      warn("Writing to dead chunk");
    }
    else if (iter.pDesc->chunkState == CLOSED) {
      err = true;
      warn("Writing to closed chunk");
    }
    // Check write pointer
    else if (iter.pDesc->chunkState == OPEN &&
             iter.pDesc->writePointer / structure.writeSize > iter.pageIdx) {
      err = true;
      warn("Write pointer violation");
    }

    if (err) {
      ((IOContext *)context)
          ->resp.makeStatus(false, false, TYPE_MEDIA_AND_DATA_INTEGRITY_ERROR,
                            0xF2);  // Out of order write
    }
    else {
      // Update write pointer
      iter.pDesc->writePointer = (iter.pageIdx + 1) * structure.writeSize;
      iter.pDesc->chunkState = OPEN;

      // Do we need to close this chunk?
      if (iter.pageIdx == param.page - 1) {
        iter.pDesc->chunkState = CLOSED;
      }
    }

    if (err) {
      if (idx <= 0xFFFFFFFF) {
        ((IOContext *)context)->resp.entry.dword0 |= idx;
      }
      else {
        ((IOContext *)context)->resp.entry.reserved |= (idx >> 32);
      }
    }

    idx <<= 1;
  }

  if (!err) {
    ((IOContext *)context)->resp.entry.dword0 = 0xFFFFFFFF;
    ((IOContext *)context)->resp.entry.reserved = 0xFFFFFFFF;
  }
  else {
    // Remove invalid LBAs
    uint64_t mask = ((IOContext *)context)->resp.entry.reserved;
    idx = 1;

    mask <<= 32;
    mask |= ((IOContext *)context)->resp.entry.dword0;

    for (auto iter = pContext->list.begin(); iter != pContext->list.end();) {
      if (mask & idx) {
        iter = pContext->list.erase(iter);
      }
      else {
        iter++;
      }

      idx <<= 1;
    }
  }

  DMAFunction doWrite = [this](uint64_t tick, void *context) {
    auto pContext = (OCSSDContext *)context;

    tick = MAX(tick, pContext->beginAt);

    uint64_t beginAt;
    uint64_t finishedAt = tick;

    for (auto &iter : pContext->list) {
      beginAt = pContext->beginAt;

      pPALOLD->write(iter, beginAt);

      finishedAt = MAX(finishedAt, beginAt);
    }

    pContext->req.finishedAt = finishedAt;
    completionQueue.push(pContext->req);

    updateCompletion();

    delete pContext;
  };

  execute(CPU::NVME__OCSSD, CPU::WRITE_INTERNAL, doWrite, pContext);
}

void OpenChannelSSD20::eraseInternal(std::vector<uint64_t> &lbaList,
                                     DMAFunction &func, void *context,
                                     bool mode) {
  OCSSDContext *pContext = new OCSSDContext();
  std::vector<ChunkUpdateEntry> chunks;
  bool err = false;
  uint64_t idx = 1;

  pContext->req = Request(func, context);
  pContext->beginAt =
      getTick() + applyLatency(CPU::NVME__OCSSD, CPU::CONVERT_UNIT);

  convertUnit(lbaList, pContext->list, chunks, true, mode);

  ((IOContext *)context)->resp.entry.dword0 = 0;
  ((IOContext *)context)->resp.entry.reserved = 0;

  // Update ChunkDescriptor
  for (auto &iter : chunks) {
    // Check block status
    if (iter.pDesc->chunkState == OFFLINE) {
      err = true;
      warn("Erasing dead chunk");
      ((IOContext *)context)
          ->resp.makeStatus(false, false, TYPE_MEDIA_AND_DATA_INTEGRITY_ERROR,
                            0xC0);  // Offline chunk
    }
    else if (iter.pDesc->chunkState == FREE || iter.pDesc->chunkState == OPEN) {
      err = true;
      warn("Erasing free or open chunk");

      ((IOContext *)context)
          ->resp.makeStatus(false, false, TYPE_MEDIA_AND_DATA_INTEGRITY_ERROR,
                            0xC1);  // Invalid reset
    }
    else {
      // Update
      iter.pDesc->writePointer = 0;
      iter.pDesc->chunkState = FREE;
    }

    if (err) {
      if (idx <= 0xFFFFFFFF) {
        ((IOContext *)context)->resp.entry.dword0 |= idx;
      }
      else {
        ((IOContext *)context)->resp.entry.reserved |= (idx >> 32);
      }
    }

    idx <<= 1;
  }

  if (!err) {
    ((IOContext *)context)->resp.entry.dword0 = 0xFFFFFFFF;
    ((IOContext *)context)->resp.entry.reserved = 0xFFFFFFFF;
  }
  else {
    // Remove invalid LBAs
    uint64_t mask = ((IOContext *)context)->resp.entry.reserved;
    idx = 1;

    mask <<= 32;
    mask |= ((IOContext *)context)->resp.entry.dword0;

    for (auto iter = pContext->list.begin(); iter != pContext->list.end();) {
      if (mask & idx) {
        iter = pContext->list.erase(iter);
      }
      else {
        iter++;
      }

      idx <<= 1;
    }
  }

  DMAFunction doErase = [this](uint64_t tick, void *context) {
    auto pContext = (OCSSDContext *)context;

    tick = MAX(tick, pContext->beginAt);

    uint64_t beginAt;
    uint64_t finishedAt = tick;

    for (auto &iter : pContext->list) {
      beginAt = pContext->beginAt;

      pPALOLD->erase(iter, beginAt);

      finishedAt = MAX(finishedAt, beginAt);
    }

    pContext->req.finishedAt = finishedAt;
    completionQueue.push(pContext->req);

    updateCompletion();

    delete pContext;
  };

  execute(CPU::NVME__OCSSD, CPU::ERASE_INTERNAL, doErase, pContext);
}

bool OpenChannelSSD20::getLogPage(SQEntryWrapper &req, RequestFunction &func) {
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
    IOContext *pContext = (IOContext *)context;

    pContext->function(pContext->resp);

    delete pContext->dma;
    delete pContext;
  };
  DMAFunction smartInfo = [this, offset](uint64_t, void *context) {
    IOContext *pContext = (IOContext *)context;

    pContext->dma->write(offset, pContext->nlb, pContext->buffer, dmaDone,
                         context);
  };

  switch (lid) {
    case LOG_ERROR_INFORMATION:
    case LOG_SMART_HEALTH_INFORMATION:
    case LOG_FIRMWARE_SLOT_INFORMATION:
      ret = true;
      break;
    case LOG_CHUNK_INFORMATION: {
      ret = true;
      submit = false;
      IOContext *pContext = new IOContext(func, resp);

      // Fill data
      pContext->buffer = (uint8_t *)pDescriptor;
      pContext->nlb = req_size;

      if (req.useSGL) {
        pContext->dma = new SGL(cfgdata, smartInfo, pContext, req.entry.data1,
                                req.entry.data2);
      }
      else {
        pContext->dma =
            new PRPList(cfgdata, smartInfo, pContext, req.entry.data1,
                        req.entry.data2, (uint64_t)req_size);
      }
    } break;
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

bool OpenChannelSSD20::geometry(SQEntryWrapper &req, RequestFunction &func) {
  CQEntryWrapper resp(req);

  RequestContext *pContext = new RequestContext(func, resp);

  pContext->buffer = (uint8_t *)calloc(0x1000, sizeof(uint8_t));
  debugprint(LOG_HIL_NVME, "OCSSD   | Geometry");

  // Fill Geometry Information
  pContext->buffer[0x00] = 2;  // OpenChannel SSD Spec Rev. 2.0
  pContext->buffer[0x01] = 0;
  pContext->buffer[0x08] = popcount(ppaMask.channelMask);
  pContext->buffer[0x09] = popcount(ppaMask.wayMask);
  pContext->buffer[0x0A] = popcount(ppaMask.blockMask);
  pContext->buffer[0x0B] = popcount(ppaMask.sectorMask);
  pContext->buffer[0x20] = 0x7F;

  // Geometry Related
  *(uint16_t *)(pContext->buffer + 0x40) = structure.group;
  *(uint16_t *)(pContext->buffer + 0x42) = structure.parallelUnit;
  *(uint32_t *)(pContext->buffer + 0x44) = structure.chunk;
  *(uint32_t *)(pContext->buffer + 0x48) = structure.chunkSize;

  // Write Data Requrements
  *(uint32_t *)(pContext->buffer + 0x80) = structure.writeSize;
  *(uint32_t *)(pContext->buffer + 0x84) = structure.writeSize;

  // Performance Related Metrics
  PAL::Config::NANDTiming *pNANDTiming = conf.getNANDTiming();

  *(uint32_t *)(pContext->buffer + 0xC0) = pNANDTiming->msb.read / 1000;
  *(uint32_t *)(pContext->buffer + 0xC8) = pNANDTiming->msb.write / 1000;

  if (conf.readInt(CONFIG_PAL, PAL::NAND_FLASH_TYPE) != PAL::NAND_SLC) {
    *(uint32_t *)(pContext->buffer + 0xC4) = pNANDTiming->lsb.read / 1000;
    *(uint32_t *)(pContext->buffer + 0xCC) = pNANDTiming->lsb.write / 1000;
  }
  else {
    *(uint32_t *)(pContext->buffer + 0xC4) = pNANDTiming->msb.read / 1000;
    *(uint32_t *)(pContext->buffer + 0xCC) = pNANDTiming->msb.write / 1000;
  }

  *(uint32_t *)(pContext->buffer + 0xD0) = pNANDTiming->erase / 1000;
  *(uint32_t *)(pContext->buffer + 0xD4) = pNANDTiming->erase / 1000;

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

  if (req.useSGL) {
    pContext->dma =
        new SGL(cfgdata, doWrite, pContext, req.entry.data1, req.entry.data2);
  }
  else {
    pContext->dma = new PRPList(cfgdata, doWrite, pContext, req.entry.data1,
                                req.entry.data2, (uint64_t)0x1000);
  }

  return true;
}

void OpenChannelSSD20::read(SQEntryWrapper &req, RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  uint64_t slba = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;
  uint16_t nlb = (req.entry.dword12 & 0xFFFF) + 1;
  // bool fua = req.entry.dword12 & 0x40000000;

  if (nlb == 0) {
    err = true;
    warn("nvme_namespace: host tried to read 0 blocks");
  }

  readCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | READ  | %" PRIX64 " + %d", slba, nlb);

  if (!err) {
    DMAFunction doRead = [this](uint64_t, void *context) {
      DMAFunction doWrite = [this](uint64_t tick, void *context) {
        DMAFunction dmaDone = [](uint64_t tick, void *context) {
          IOContext *pContext = (IOContext *)context;

          debugprint(LOG_HIL_NVME,
                     "OCSSD   | READ  | %" PRIX64 " + %d | DMA %" PRIu64
                     " - %" PRIu64 " (%" PRIu64 ")",
                     pContext->slba, pContext->nlb, pContext->tick, tick,
                     tick - pContext->tick);

          pContext->function(pContext->resp);

          if (pContext->buffer) {
            free(pContext->buffer);
          }

          delete pContext->dma;
          delete pContext;
        };

        IOContext *pContext = (IOContext *)context;

        debugprint(LOG_HIL_NVME,
                   "OCSSD   | READ  | %" PRIX64 " + %d | NAND %" PRIu64
                   " - %" PRIu64 " (%" PRIu64 ")",
                   pContext->slba, pContext->nlb, pContext->beginAt, tick,
                   tick - pContext->beginAt);

        pContext->tick = tick;
        pContext->buffer = (uint8_t *)calloc(pContext->nlb, LBA_SIZE);

        pDisk->read(pContext->slba, pContext->nlb, pContext->buffer);
        pContext->dma->write(0, pContext->nlb * LBA_SIZE, pContext->buffer,
                             dmaDone, context);
      };

      IOContext *pContext = (IOContext *)context;
      std::vector<uint64_t> input = {pContext->slba, pContext->nlb};

      readInternal(input, doWrite, pContext);
    };

    IOContext *pContext = new IOContext(func, resp);

    pContext->beginAt = getTick();
    pContext->slba = slba;
    pContext->nlb = nlb;

    CPUContext *pCPU =
        new CPUContext(doRead, pContext, CPU::NVME__OCSSD, CPU::READ);

    if (req.useSGL) {
      pContext->dma =
          new SGL(cfgdata, cpuHandler, pCPU, req.entry.data1, req.entry.data2);
    }
    else {
      pContext->dma = new PRPList(cfgdata, cpuHandler, pCPU, req.entry.data1,
                                  req.entry.data2, pContext->nlb * LBA_SIZE);
    }
  }
  else {
    func(resp);
  }
}

void OpenChannelSSD20::write(SQEntryWrapper &req, RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  uint64_t slba = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;
  uint16_t nlb = (req.entry.dword12 & 0xFFFF) + 1;

  if (nlb == 0) {
    err = true;
    warn("nvme_namespace: host tried to write 0 blocks");
  }

  writeCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | WRITE | %" PRIX64 " + %d", slba, nlb);

  if (!err) {
    DMAFunction doRead = [this](uint64_t, void *context) {
      DMAFunction dmaDone = [this](uint64_t tick, void *context) {
        DMAFunction doWrite = [](uint64_t tick, void *context) {
          IOContext *pContext = (IOContext *)context;

          debugprint(LOG_HIL_NVME,
                     "OCSSD   | WRITE | %" PRIX64 " + %d | NAND %" PRIu64
                     " - %" PRIu64 " (%" PRIu64 ")",
                     pContext->slba, pContext->nlb, pContext->tick, tick,
                     tick - pContext->tick);

          pContext->function(pContext->resp);

          delete pContext;
        };

        IOContext *pContext = (IOContext *)context;

        pContext->tick = tick;

        if (pContext->buffer) {
          pDisk->write(pContext->slba, pContext->nlb, pContext->buffer);

          free(pContext->buffer);
        }

        debugprint(LOG_HIL_NVME,
                   "OCSSD   | WRITE | %" PRIX64 " + %d | DMA %" PRIu64
                   " - %" PRIu64 " (%" PRIu64 ")",
                   pContext->slba, pContext->nlb, pContext->beginAt, tick,
                   tick - pContext->beginAt);

        delete pContext->dma;

        std::vector<uint64_t> input = {pContext->slba, pContext->nlb};

        writeInternal(input, doWrite, context);
      };

      IOContext *pContext = (IOContext *)context;

      pContext->buffer = (uint8_t *)calloc(pContext->nlb, LBA_SIZE);
      pContext->dma->read(0, pContext->nlb * LBA_SIZE, pContext->buffer,
                          dmaDone, context);
    };

    IOContext *pContext = new IOContext(func, resp);

    pContext->beginAt = getTick();
    pContext->slba = slba;
    pContext->nlb = nlb;

    CPUContext *pCPU =
        new CPUContext(doRead, pContext, CPU::NVME__OCSSD, CPU::WRITE);

    if (req.useSGL) {
      pContext->dma =
          new SGL(cfgdata, cpuHandler, pCPU, req.entry.data1, req.entry.data2);
    }
    else {
      pContext->dma = new PRPList(cfgdata, cpuHandler, pCPU, req.entry.data1,
                                  req.entry.data2, (uint64_t)nlb * LBA_SIZE);
    }
  }
  else {
    func(resp);
  }
}

void OpenChannelSSD20::datasetManagement(SQEntryWrapper &req,
                                         RequestFunction &func) {
  bool err = false;

  CQEntryWrapper resp(req);
  int nr = (req.entry.dword10 & 0xFF) + 1;
  bool ad = req.entry.dword11 & 0x04;

  if (!ad) {
    err = true;
    // Just ignore
  }

  eraseCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | TRIM  | %d ranges | Attr %1X", nr,
             req.entry.dword11 & 0x0F);

  if (!err) {
    static DMAFunction eachTrimDone = [](uint64_t tick, void *context) {
      DMAContext *pContext = (DMAContext *)context;

      pContext->counter--;

      if (pContext->counter == 0) {
        pContext->function(tick, pContext->context);
      }

      delete pContext;
    };
    DMAFunction doTrim = [this](uint64_t, void *context) {
      DMAFunction dmaDone = [this](uint64_t, void *context) {
        DMAFunction trimDone = [](uint64_t tick, void *context) {
          IOContext *pContext = (IOContext *)context;

          debugprint(LOG_HIL_NVME,
                     "NVM     | ERASE | %" PRIu64 " - %" PRIu64 " (%" PRIu64
                     ")",
                     pContext->beginAt, tick, tick - pContext->beginAt);

          pContext->function(pContext->resp);

          delete pContext;
        };

        DatasetManagementRange range;
        IOContext *pContext = (IOContext *)context;
        DMAContext *pDMA = new DMAContext(trimDone);

        pDMA->context = context;

        for (uint64_t i = 0; i < pContext->slba; i++) {
          memcpy(range.data,
                 pContext->buffer + i * sizeof(DatasetManagementRange),
                 sizeof(DatasetManagementRange));

          pDMA->counter++;

          pDisk->erase(range.slba, range.nlb);

          std::vector<uint64_t> input = {range.slba, range.nlb};

          eraseInternal(input, eachTrimDone, pDMA);
        }

        free(pContext->buffer);
        delete pContext->dma;
      };

      IOContext *pContext = (IOContext *)context;

      pContext->buffer =
          (uint8_t *)calloc(pContext->slba, sizeof(DatasetManagementRange));

      pContext->dma->read(0, pContext->slba * sizeof(DatasetManagementRange),
                          pContext->buffer, dmaDone, context);
    };

    IOContext *pContext = new IOContext(func, resp);

    pContext->beginAt = getTick();
    pContext->slba = nr;

    CPUContext *pCPU = new CPUContext(doTrim, pContext, CPU::NVME__OCSSD,
                                      CPU::DATASET_MANAGEMENT);

    if (req.useSGL) {
      pContext->dma =
          new SGL(cfgdata, cpuHandler, pCPU, req.entry.data1, req.entry.data2);
    }
    else {
      pContext->dma = new PRPList(cfgdata, cpuHandler, pCPU, req.entry.data1,
                                  req.entry.data2, (uint64_t)nr * 0x10);
    }
  }
  else {
    func(resp);
  }
}

void OpenChannelSSD20::vectorChunkRead(SQEntryWrapper &req,
                                       RequestFunction &func) {
  CQEntryWrapper resp(req);

  VectorContext *pContext = new VectorContext(func, resp);

  pContext->slba = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;
  pContext->nlb = (req.entry.dword12 & 0x0000003F) + 1;
  pContext->beginAt = getTick();

  vectorReadCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | Vector Chunk Read  | %d lbas",
             pContext->nlb);

  DMAFunction doDMA = [this](uint64_t, void *context) {
    DMAFunction doRead = [this](uint64_t, void *context) {
      DMAFunction eachRead = [this](uint64_t, void *context) {
        DMAFunction dmaDone = [](uint64_t now, void *context) {
          VectorContext *pContext = (VectorContext *)context;

          // Done
          debugprint(LOG_HIL_NVME,
                     "OCSSD   | Vector Chunk Read  | %" PRIu64 " - %" PRIu64
                     " (%" PRIu64 ")",
                     pContext->beginAt, now, now - pContext->beginAt);

          pContext->function(pContext->resp);

          free(pContext->buffer);
          delete pContext->dma;
          delete pContext;
        };

        VectorContext *pContext = (VectorContext *)context;

        // Do Read
        uint64_t size = (pContext->nlb + 1) * LBA_SIZE;
        pContext->buffer = (uint8_t *)calloc(size, 1);

        for (uint64_t i = 0; i < pContext->nlb; i++) {
          uint64_t lba = pContext->lbaList.at(i);

          pDisk->read(lba, 1, pContext->buffer + i * LBA_SIZE);
        }

        pContext->dma->write(0, size, pContext->buffer, dmaDone, context);
      };

      VectorContext *pContext = (VectorContext *)context;
      std::vector<::CPDPBP> list;

      // Collect LBA List
      if (pContext->nlb > 1) {
        for (uint64_t i = 0; i < pContext->nlb; i++) {
          pContext->lbaList.push_back(*(uint64_t *)(pContext->buffer + i * 8));
        }

        free(pContext->buffer);
      }
      else {
        pContext->lbaList.push_back(pContext->slba);
      }

      // Do Read
      readInternal(pContext->lbaList, eachRead, context, true);
    };

    VectorContext *pContext = (VectorContext *)context;

    if (pContext->nlb > 1) {
      uint64_t size = pContext->nlb * 8;

      pContext->buffer = (uint8_t *)calloc(size, 1);

      cfgdata.pInterface->dmaRead(pContext->slba, size, pContext->buffer,
                                  doRead, pContext);
    }
    else {
      doRead(0, pContext);
    }
  };

  CPUContext *pCPU = new CPUContext(doDMA, pContext, CPU::NVME__OCSSD,
                                    CPU::VECTOR_CHUNK_WRITE);

  if (req.useSGL) {
    pContext->dma =
        new SGL(cfgdata, cpuHandler, pCPU, req.entry.data1, req.entry.data2);
  }
  else {
    pContext->dma = new PRPList(cfgdata, cpuHandler, pCPU, req.entry.data1,
                                req.entry.data2, pContext->nlb * LBA_SIZE);
  }
}

void OpenChannelSSD20::vectorChunkWrite(SQEntryWrapper &req,
                                        RequestFunction &func) {
  CQEntryWrapper resp(req);

  VectorContext *pContext = new VectorContext(func, resp);

  pContext->slba = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;
  pContext->nlb = (req.entry.dword12 & 0x0000003F) + 1;
  pContext->beginAt = getTick();

  vectorWriteCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | Vector Chunk Write | %d lbas",
             pContext->nlb);

  DMAFunction doDMA = [this](uint64_t, void *context) {
    DMAFunction doRead = [this](uint64_t, void *context) {
      DMAFunction eachWrite = [this](uint64_t, void *context) {
        DMAFunction dmaDone = [this](uint64_t now, void *context) {
          VectorContext *pContext = (VectorContext *)context;
          uint64_t mask = pContext->resp.entry.reserved;

          mask <<= 32;
          mask |= pContext->resp.entry.dword0;

          if (pContext->resp.entry.dword3.status == 0) {
            mask = 0;
          }

          // Write to disk
          for (uint64_t i = 0; i < pContext->nlb; i++) {
            if ((1 << i) & mask) {
              continue;
            }

            uint64_t lba = pContext->lbaList.at(i);

            pDisk->write(lba, 1, pContext->buffer + i * LBA_SIZE);
          }

          // Done
          debugprint(LOG_HIL_NVME,
                     "OCSSD   | Vector Chunk Write | %" PRIu64 " - %" PRIu64
                     " (%" PRIu64 ")",
                     pContext->beginAt, now, now - pContext->beginAt);

          pContext->function(pContext->resp);

          free(pContext->buffer);
          delete pContext->dma;
          delete pContext;
        };

        VectorContext *pContext = (VectorContext *)context;

        // Do Read
        uint64_t size = pContext->nlb * LBA_SIZE;
        pContext->buffer = (uint8_t *)calloc(size, 1);
        pContext->dma->read(0, size, pContext->buffer, dmaDone, context);
      };

      VectorContext *pContext = (VectorContext *)context;
      std::vector<::CPDPBP> list;

      // Collect LBA List
      if (pContext->nlb > 1) {
        for (uint64_t i = 0; i < pContext->nlb; i++) {
          pContext->lbaList.push_back(*(uint64_t *)(pContext->buffer + i * 8));
        }

        free(pContext->buffer);
      }
      else {
        pContext->lbaList.push_back(pContext->slba);
      }

      // Do Write
      writeInternal(pContext->lbaList, eachWrite, context, true);
    };

    VectorContext *pContext = (VectorContext *)context;

    if (pContext->nlb > 1) {
      uint64_t size = pContext->nlb * 8;

      pContext->buffer = (uint8_t *)calloc(size, 1);

      cfgdata.pInterface->dmaRead(pContext->slba, size, pContext->buffer,
                                  doRead, pContext);
    }
    else {
      doRead(0, pContext);
    }
  };

  CPUContext *pCPU = new CPUContext(doDMA, pContext, CPU::NVME__OCSSD,
                                    CPU::VECTOR_CHUNK_WRITE);

  if (req.useSGL) {
    pContext->dma =
        new SGL(cfgdata, cpuHandler, pCPU, req.entry.data1, req.entry.data2);
  }
  else {
    pContext->dma = new PRPList(cfgdata, cpuHandler, pCPU, req.entry.data1,
                                req.entry.data2, pContext->nlb * LBA_SIZE);
  }
}

void OpenChannelSSD20::vectorChunkReset(SQEntryWrapper &req,
                                        RequestFunction &func) {
  CQEntryWrapper resp(req);

  IOContext *pContext = new IOContext(func, resp);

  pContext->slba = ((uint64_t)req.entry.dword11 << 32) | req.entry.dword10;
  pContext->nlb = (req.entry.dword12 & 0x0000003F) + 1;
  pContext->beginAt = getTick();

  vectorEraseCount++;

  debugprint(LOG_HIL_NVME, "OCSSD   | Vector Chunk Reset | %d lbas",
             pContext->nlb);

  DMAFunction doRead = [this](uint64_t, void *context) {
    DMAFunction eachErase = [](uint64_t now, void *context) {
      IOContext *pContext = (IOContext *)context;

      debugprint(LOG_HIL_NVME,
                 "OCSSD   | Vector Chunk Reset | %" PRIu64 " - %" PRIu64
                 " (%" PRIu64 ")",
                 pContext->beginAt, now, now - pContext->beginAt);

      pContext->function(pContext->resp);

      delete pContext;
    };

    IOContext *pContext = (IOContext *)context;
    std::vector<uint64_t> lbaList;
    std::vector<::CPDPBP> list;

    // Collect LBA List
    if (pContext->nlb > 1) {
      for (uint64_t i = 0; i < pContext->nlb; i++) {
        lbaList.push_back(*(uint64_t *)(pContext->buffer + i * 8));
      }

      free(pContext->buffer);
    }
    else {
      lbaList.push_back(pContext->slba);
    }

    // Do Reset
    eraseInternal(lbaList, eachErase, context, true);
  };

  CPUContext *pCPU = new CPUContext(doRead, pContext, CPU::NVME__OCSSD,
                                    CPU::VECTOR_CHUNK_RESET);

  if (pContext->nlb > 1) {
    uint64_t size = pContext->nlb * 8;

    pContext->buffer = (uint8_t *)calloc(size, 1);

    cfgdata.pInterface->dmaRead(pContext->slba, size, pContext->buffer,
                                cpuHandler, pCPU);
  }
  else {
    doRead(0, pContext);
  }
}

void OpenChannelSSD20::getStatList(std::vector<Stats> &list,
                                   std::string prefix) {
  Stats temp;

  temp.name = prefix + "command_count";
  temp.desc = "Total number of OCSSD command handled";
  list.push_back(temp);

  temp.name = prefix + "erase";
  temp.desc = "Total number of TRIM (Erase) command";
  list.push_back(temp);

  temp.name = prefix + "read";
  temp.desc = "Total number of Read command";
  list.push_back(temp);

  temp.name = prefix + "write";
  temp.desc = "Total number of Write command";
  list.push_back(temp);

  temp.name = prefix + "vector.erase";
  temp.desc = "Total number of Vector Chunk Reset command";
  list.push_back(temp);

  temp.name = prefix + "vector.read";
  temp.desc = "Total number of Vector Chunk Read command";
  list.push_back(temp);

  temp.name = prefix + "vector.write";
  temp.desc = "Total number of Vector Chunk Write command";
  list.push_back(temp);

  pPALOLD->getStatList(list, prefix + "pal.");
}

void OpenChannelSSD20::getStatValues(std::vector<double> &values) {
  values.push_back(commandCount);
  values.push_back(eraseCount);
  values.push_back(readCount);
  values.push_back(writeCount);
  values.push_back(vectorEraseCount);
  values.push_back(vectorReadCount);
  values.push_back(vectorWriteCount);

  pPALOLD->getStatValues(values);
}

void OpenChannelSSD20::resetStatValues() {
  commandCount = 0;
  eraseCount = 0;
  readCount = 0;
  writeCount = 0;
  vectorEraseCount = 0;
  vectorReadCount = 0;
  vectorWriteCount = 0;

  pPALOLD->resetStatValues();
}

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD
