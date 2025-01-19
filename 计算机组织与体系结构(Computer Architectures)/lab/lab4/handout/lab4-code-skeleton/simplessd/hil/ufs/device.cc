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

#include "hil/ufs/device.hh"

#include <cmath>

#include "hil/ufs/interface.hh"
#include "util/algorithm.hh"

namespace SimpleSSD {

namespace HIL {

namespace UFS {

struct IOContext {
  uint64_t beginAt;
  uint32_t slba;
  uint32_t nlb;
  uint64_t tick;

  uint8_t *buffer;

  uint8_t *prdt;
  uint32_t prdtLength;

  DMAFunction func;
  void *context;

  IOContext(DMAFunction &f) : beginAt(getTick()), func(f) {}
};

LUN::_LUN(bool b, uint8_t n, ConfigReader &cfg) : bWellknown(b), id(n) {
  uint8_t wn = bWellknown ? 0x80 : 0x00;

  // Initialize unit descriptor
  memset(unitDescriptor, 0, IDN_UNIT_LENGTH);
  unitDescriptor[0x00] = IDN_UNIT_LENGTH;
  unitDescriptor[0x01] = IDN_UNIT;
  unitDescriptor[0x02] = wn | id;  // Unit index
  unitDescriptor[0x03] = 0x01;     // LU enabled
  unitDescriptor[0x04] = 0x01;     // Boot LUN ID
  unitDescriptor[0x05] = 0x00;     // Write protect
  unitDescriptor[0x06] = 0x00;     // Queue depth
  unitDescriptor[0x07] = 0x00;     // PSA Sensitive
  unitDescriptor[0x08] = 0x00;     // Memory type
  unitDescriptor[0x09] = 0x00;     // Data reliability

  if (bWellknown) {
    unitDescriptor[0x0A] = 0x00;               // LBA size
    *(uint64_t *)(unitDescriptor + 0x0B) = 0;  // Total logical LBA
    *(uint32_t *)(unitDescriptor + 0x13) = 0;  // Total LBA in physical block
    unitDescriptor[0x17] = 0x00;               // Thin provisioning
    *(uint64_t *)(unitDescriptor + 0x18) = 0;  // Total physical LBA
  }
  else {
    // TODO: Fix this code (duplicated)
    uint64_t lbaSize = cfg.readUint(CONFIG_UFS, UFS_LBA_SIZE);
    uint64_t blockSize = cfg.readUint(CONFIG_PAL, PAL::NAND_PAGE_SIZE);
    uint64_t totalSize = cfg.readUint(CONFIG_PAL, PAL::PAL_CHANNEL);

    blockSize *= cfg.readUint(CONFIG_PAL, PAL::NAND_PAGE);

    totalSize *= cfg.readUint(CONFIG_PAL, PAL::PAL_PACKAGE);
    totalSize *= cfg.readUint(CONFIG_PAL, PAL::NAND_DIE);
    totalSize *= cfg.readUint(CONFIG_PAL, PAL::NAND_PLANE);
    totalSize *= cfg.readUint(CONFIG_PAL, PAL::NAND_BLOCK);
    totalSize *= blockSize;

    blockSize *= cfg.readBoolean(CONFIG_PAL, PAL::NAND_USE_MULTI_PLANE_OP)
                     ? cfg.readUint(CONFIG_PAL, PAL::NAND_PLANE)
                     : 1;

    unitDescriptor[0x0A] = (uint8_t)log2f(lbaSize);  // LBA size
    *(uint64_t *)(unitDescriptor + 0x0B) =
        (totalSize *
         (1 - cfg.readFloat(CONFIG_FTL, FTL::FTL_OVERPROVISION_RATIO))) /
        lbaSize;  // Total logical LBA
    *(uint32_t *)(unitDescriptor + 0x13) =
        blockSize / lbaSize;      // Total LBA in physical block
    unitDescriptor[0x17] = 0x00;  // Thin provisioning
    *(uint64_t *)(unitDescriptor + 0x18) =
        totalSize / lbaSize;  // Total physical LBA
  }

  unitDescriptor[0x20] = 0x00;
  unitDescriptor[0x21] = 0x00;  // Context capabilityes
  unitDescriptor[0x22] = 0x00;  // Large unit granuality

  // Initialize INQUIRY structure
  memset(inquiry, 0, SCSI_INQUIRY_LENGTH);
  inquiry[0] = bWellknown ? 0x1E : 0x00;
  inquiry[2] = 0x06;
  inquiry[3] = 0x02;
  inquiry[4] = 31;
  inquiry[7] = 0x02;

  sprintf((char *)inquiry + 8, "SimpleSSD UFS Device");
}

Device::Device(DMAInterface *d, ConfigReader &c)
    : pDMA(d),
      pHIL(nullptr),
      pDisk(nullptr),
      conf(c),
      lunReportLuns(true, WLUN_REPORT_LUNS, c),
      lunUFSDevice(true, WLUN_UFS_DEVICE, c),
      lunBoot(true, WLUN_BOOT, c),
      lunRPMB(true, WLUN_RPMB, c),
      lun(false, 0x00, c) {
  // Initialize Strings
  sprintf((char *)strManufacturer, "CAMELab");
  sprintf((char *)strProductName, "SimpleSSD UFS Device");
  sprintf((char *)strSerialNumber, "000000000000");
  sprintf((char *)strOEMID, "None");
  sprintf((char *)strProductRevision, "v02.01");

  // Initialize device descriptor
  memset(deviceDescriptor, 0, IDN_DEVICE_LENGTH);
  deviceDescriptor[0x00] = IDN_DEVICE_LENGTH;  // Length
  deviceDescriptor[0x01] = IDN_DEVICE;         // Descriptor IDN
  deviceDescriptor[0x02] = 0x00;               // Device Type
  deviceDescriptor[0x03] = 0x00;               // UFS Device Class
  deviceDescriptor[0x04] = 0x00;               // UFS Mass Storage Subclass
  deviceDescriptor[0x05] = 0x00;  // Protocol supported by UFS Device
  deviceDescriptor[0x06] = 0x01;  // Number of Logical Units
  deviceDescriptor[0x07] = 0x04;  // Number of Well known Logical Units
  deviceDescriptor[0x08] = 0x01;  // Boot Enable
  deviceDescriptor[0x09] = 0x00;  // Descriptor Access Enable
  deviceDescriptor[0x0A] = 0x01;  // Initial Power Mode
  deviceDescriptor[0x0B] = 0x7F;  // High Priority LUN
  deviceDescriptor[0x0C] = 0x00;  // Secure Removal Type
  deviceDescriptor[0x0D] = 0x00;  // Support for security LU
  deviceDescriptor[0x0E] = 0x00;  // Background Operations Termination Latency
  deviceDescriptor[0x0F] = 0x00;  // Initial Active ICC Level
  deviceDescriptor[0x10] = 0x10;
  deviceDescriptor[0x11] = 0x02;  // UFS Version 2.1
  deviceDescriptor[0x12] = 0x00;
  deviceDescriptor[0x13] = 0x00;  // Manufacturing Date
  deviceDescriptor[0x14] = STRING_MANUFACTURER;
  deviceDescriptor[0x15] = STRING_PRODUCT_NAME;
  deviceDescriptor[0x16] = STRING_SERIAL_NUMBER;
  deviceDescriptor[0x17] = STRING_OEM_ID;
  deviceDescriptor[0x18] = 0x00;
  deviceDescriptor[0x19] = 0x00;  // Manufacturer ID
  deviceDescriptor[0x1A] = 0x10;  // Unit Descriptor 0 Base Offset
  deviceDescriptor[0x1B] = 0x10;  // Unit Descriptor Config Parameter Length
  deviceDescriptor[0x1C] = 0x02;  // RTT Capability of device
  deviceDescriptor[0x1D] = 0x00;
  deviceDescriptor[0x1E] = 0x00;
  // deviceDescriptor[0x1F] = 0x00; // UFS Features Support
  // deviceDescriptor[0x20] = 0x00; // Field Firmware Update Timeout
  // deviceDescriptor[0x21] = 0x20; // Queue Depth
  // deviceDescriptor[0x22] = 0x00;
  // deviceDescriptor[0x23] = 0x00; // Device version
  // deviceDescriptor[0x24] = 0x00; // Number of Secure Write Protection Areas
  // deviceDescriptor[0x25] = 0x00;
  // deviceDescriptor[0x26] = 0x00;
  // deviceDescriptor[0x27] = 0x00;
  // deviceDescriptor[0x28] = 0x00; // PSA Maximum Data Size
  // deviceDescriptor[0x29] = 0x00; // PSA State Timeout
  // deviceDescriptor[0x2A] = STRING_PRODUCT_REVISION_LEVEL;

  // Initialize power descriptor
  memset(powerDescriptor, 0, IDN_POWER_LENGTH);
  powerDescriptor[0x00] = IDN_POWER_LENGTH;  // Length
  powerDescriptor[0x01] = IDN_POWER;         // Descriptor IDN
  *(uint16_t *)(powerDescriptor + 0x02) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x04) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x06) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x08) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x0A) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x0C) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x0E) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x10) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x12) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x14) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x16) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x18) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x1A) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x1C) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x1E) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x20) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x22) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x24) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x26) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x28) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x2A) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x2C) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x2E) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x30) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x32) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x34) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x36) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x38) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x3A) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x3C) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x3E) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x40) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x42) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x44) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x46) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x48) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x4A) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x4C) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x4E) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x50) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x52) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x54) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x56) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x58) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x5A) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x5C) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x5E) = 0x0100;
  *(uint16_t *)(powerDescriptor + 0x60) = 0x0100;

  // Create HIL
  pHIL = new HIL(c);
  pHIL->getLPNInfo(totalLogicalPages, logicalPageSize);

  // Create Disk
  if (conf.readBoolean(CONFIG_UFS, UFS_ENABLE_DISK_IMAGE)) {
    uint64_t diskSize;
    uint64_t expected;

    expected = totalLogicalPages * logicalPageSize;

    if (conf.readBoolean(CONFIG_UFS, UFS_USE_COW_DISK)) {
      pDisk = new CoWDisk();
    }
    else {
      pDisk = new Disk();
    }

    std::string filename = conf.readString(CONFIG_UFS, UFS_DISK_IMAGE_PATH);

    diskSize = pDisk->open(filename, expected,
                           conf.readUint(CONFIG_UFS, UFS_LBA_SIZE));

    if (diskSize == 0) {
      panic("Failed to open disk image");
    }
    else if (diskSize != expected) {
      if (conf.readBoolean(CONFIG_UFS, UFS_STRICT_DISK_SIZE)) {
        panic("Disk size not match");
      }
    }
  }

  // Set cache info
  bReadCache = conf.readBoolean(CONFIG_ICL, ICL::ICL_USE_READ_CACHE);
  bWriteCache = conf.readBoolean(CONFIG_ICL, ICL::ICL_USE_WRITE_CACHE);

  // DMA Handler for PRDT
  dmaHandler = [](uint64_t now, void *context) {
    DMAContext *pContext = (DMAContext *)context;

    pContext->counter--;

    if (pContext->counter == 0) {
      pContext->function(now, pContext->context);
      delete pContext;
    }
  };
}

Device::~Device() {
  delete pHIL;
  delete pDisk;
}

void Device::processQueryCommand(UPIUQueryReq *req, UPIUQueryResp *resp) {
  resp->opcode = req->opcode;
  resp->idn = req->idn;
  resp->index = req->index;
  resp->selector = req->selector;
  resp->length = req->length;

  debugprint(
      LOG_HIL_UFS,
      "COMMAND | UPIU Query Request | OPCODE %d | IDN %d | LUN 0x%X | Tag "
      "0x%X",
      req->opcode, req->idn, req->header.lun, req->header.taskTag);

  switch (req->opcode) {
    case OPCODE_NOP:
      break;
    case OPCODE_READ_DESCRIPTOR: {
      switch (req->idn) {
        case IDN_DEVICE:
          resp->header.dataSegmentLength = req->length;
          resp->length = IDN_DEVICE_LENGTH;
          req->length = MIN(req->length, resp->length);
          resp->data = (uint8_t *)calloc(req->length, 1);
          memcpy(resp->data, deviceDescriptor, req->length);
          break;
        case IDN_UNIT: {
          uint8_t *buffer = nullptr;

          switch (req->header.lun) {
            case 0x80 | WLUN_REPORT_LUNS:
              buffer = lunReportLuns.unitDescriptor;
              break;
            case 0x80 | WLUN_UFS_DEVICE:
              buffer = lunUFSDevice.unitDescriptor;
              break;
            case 0x80 | WLUN_BOOT:
              buffer = lunBoot.unitDescriptor;
              break;
            case 0x80 | WLUN_RPMB:
              buffer = lunRPMB.unitDescriptor;
              break;
            case 0x00:
              buffer = lun.unitDescriptor;
              break;
            default:
              warn("Unknown LUN 0x%2X", req->header.lun);
          }

          resp->header.dataSegmentLength = req->length;
          resp->length = IDN_UNIT_LENGTH;
          req->length = MIN(req->length, resp->length);
          resp->data = (uint8_t *)calloc(req->length, 1);

          if (buffer) {
            memcpy(resp->data, buffer, req->length);
          }
        } break;
        case IDN_POWER:
          resp->header.dataSegmentLength = req->length;
          resp->length = IDN_POWER_LENGTH;
          req->length = MIN(req->length, resp->length);
          resp->data = (uint8_t *)calloc(req->length, 1);
          memcpy(resp->data, powerDescriptor, req->length);
          break;
        case IDN_STRING: {
          resp->header.dataSegmentLength = req->length;
          resp->length = IDN_STRING_LENGTH;
          req->length = MIN(req->length, resp->length);
          resp->data = (uint8_t *)calloc(req->length, 1);

          resp->data[0] = IDN_STRING_LENGTH;
          resp->data[1] = IDN_STRING;

          switch (req->index) {
            case STRING_MANUFACTURER:
              memcpy(resp->data + 2, strManufacturer, req->length - 2);
              break;
            case STRING_PRODUCT_NAME:
              memcpy(resp->data + 2, strProductName, req->length - 2);
              break;
            case STRING_SERIAL_NUMBER:
              memcpy(resp->data + 2, strSerialNumber, req->length - 2);
              break;
            case STRING_OEM_ID:
              memcpy(resp->data + 2, strOEMID, req->length - 2);
              break;
            case STRING_PRODUCT_REVISION_LEVEL:
              memcpy(resp->data + 2, strProductRevision, req->length - 2);
              break;
            default:
              warn("Unknown string index %#x", req->index);

              break;
          }
        } break;
        default:
          warn("Not implemented descriptor idn %#x", req->idn);

          break;
      }
    } break;
    default:
      warn("Not implemented query opcode %#x", req->opcode);

      break;
  }
}

void Device::processCommand(UTP_TRANSFER_CMD cmd, UPIUCommand *req,
                            UPIUResponse *resp, uint8_t *prdt,
                            uint32_t prdtLength, DMAFunction &func,
                            void *context) {
  static uint64_t lbaSize = conf.readUint(CONFIG_UFS, UFS_LBA_SIZE);
  bool immediate = true;
  uint8_t *buffer = nullptr;
  uint64_t length = 0;

  resp->header.lun = req->header.lun;

  // debugprint(LOG_HIL_UFS, "COMMAND | UPIU Command | CT %d",
  //                    cmd);

  // TODO: Check LUN for I/O

  switch (cmd) {
    case CMD_SCSI: {
      uint8_t opcode = req->cdb[0];
      uint32_t slba = 0;
      uint16_t nlb = 0;

      debugprint(LOG_HIL_UFS,
                 "COMMAND | SCSI Command 0x%X | LUN 0x%X | Tag 0x%X", opcode,
                 req->header.lun, req->header.taskTag);

      switch (opcode) {
        case CMD_INQUIRY: {
          buffer = nullptr;

          switch (req->header.lun) {
            case 0x80 | WLUN_REPORT_LUNS:
              buffer = lunReportLuns.inquiry;
              break;
            case 0x80 | WLUN_UFS_DEVICE:
              buffer = lunUFSDevice.inquiry;
              break;
            case 0x80 | WLUN_BOOT:
              buffer = lunBoot.inquiry;
              break;
            case 0x80 | WLUN_RPMB:
              buffer = lunRPMB.inquiry;
              break;
            case 0x00:
              buffer = lun.inquiry;
              break;
            default:
              warn("Unknown LUN 0x%2X", req->header.lun);
          }

          if (buffer) {
            prdtWrite(prdt, prdtLength, 36, buffer, func, context);
            immediate = false;
          }
        } break;
        case CMD_MODE_SELECT_10:
          // Not Implemented
          break;
        case CMD_MODE_SENSE_10: {
          uint32_t size = 0;

          switch (req->cdb[2] & 0x3F) {
            case 0x0A: {
              size = 20;
              buffer = (uint8_t *)calloc(size, 1);

              // UFS Mode parameter header
              buffer[1] = 0x12;  // Length

              buffer[8] = 0x0A;  // Control Page
              buffer[9] = 0x0A;  // Length
            } break;
            case 0x01: {
              size = 20;
              buffer = (uint8_t *)calloc(size, 1);

              // UFS Mode parameter header
              buffer[1] = 0x12;  // Length

              buffer[8] = 0x01;  // Recovery Page
              buffer[9] = 0x0A;  // Length
            } break;
            case 0x08: {
              size = 28;
              buffer = (uint8_t *)calloc(size, 1);

              // UFS Mode parameter header
              buffer[1] = 0x1A;  // Length

              buffer[8] = 0x08;  // Caching Page
              buffer[9] = 0x12;  // Length
              buffer[10] = 0x00;

              buffer[10] |= bReadCache ? 0x00 : 0x01;   // Set RCD
              buffer[10] |= bWriteCache ? 0x04 : 0x00;  // Set WCE
            } break;
            case 0x3F: {
              size = 52;
              buffer = (uint8_t *)calloc(size, 1);

              // UFS Mode parameter header
              buffer[1] = 0x32;  // Length

              buffer[8] = 0x01;   // Recovery Page
              buffer[9] = 0x0A;   // Length
              buffer[20] = 0x0A;  // Control Page
              buffer[21] = 0x0A;  // Length
              buffer[32] = 0x08;  // Caching Page
              buffer[33] = 0x12;  // Length
              buffer[34] = 0x00;

              buffer[34] |= bReadCache ? 0x00 : 0x01;   // Set RCD
              buffer[34] |= bWriteCache ? 0x04 : 0x00;  // Set WCE
            } break;
            default:
              warn("Not implemented mode page %#x", req->cdb[2] & 0x3F);
              buffer = nullptr;
              break;
          }

          length = size;
        } break;
        case CMD_READ_6: {
          slba = (((uint64_t)(req->cdb[1]) & 0x1F) << 16) |
                 ((uint64_t)req->cdb[2] << 8) | req->cdb[3];
          nlb = req->cdb[4] == 0 ? 256 : req->cdb[4];
        } break;
        case CMD_READ_10: {
          // bool fua = req->cdb[1] & 0x08;
          memcpy(&slba, req->cdb + 2, 4);
          memcpy(&nlb, req->cdb + 7, 2);

          slba = __builtin_bswap32(slba);
          nlb = __builtin_bswap16(nlb);
        } break;
        case CMD_READ_CAPACITY_10: {
          uint32_t temp;
          // TODO: fill appropriate value for lba count
          buffer = (uint8_t *)calloc(8, 1);

          temp =
              (uint32_t)((totalLogicalPages * logicalPageSize / lbaSize) - 1);
          temp = __builtin_bswap32(temp);
          memcpy(buffer, &temp, 4);

          temp = lbaSize;
          temp = __builtin_bswap32(temp);
          memcpy(buffer + 4, &temp, 4);

          length = 8;
        } break;
        case CMD_READ_CAPACITY_16: {
          uint32_t temp32;
          uint64_t temp64;

          buffer = (uint8_t *)calloc(32, 1);

          temp64 = (totalLogicalPages * logicalPageSize) / lbaSize - 1;
          temp64 = __builtin_bswap64(temp64);
          memcpy(buffer, &temp64, 8);

          temp32 = lbaSize;
          temp32 = __builtin_bswap32(temp32);
          memcpy(buffer + 8, &temp32, 4);

          length = 32;
        } break;
        case CMD_START_STOP_UNIT:
          // Just response
          break;
        case CMD_TEST_UNIT_READY:
          // Just response
          break;
        case CMD_REPORT_LUNS: {
          uint8_t sel = req->cdb[2];

          buffer = (uint8_t *)calloc(40, 1);

          if (sel == 0) {
            // Return one LUN
            buffer[3] = 8;  // 8 bytes in Big Endian
          }
          else if (sel == 1) {
            // Return three LUN
            buffer[3] = 24;
            buffer[8] = 0xC1;
            buffer[9] = WLUN_REPORT_LUNS;
            buffer[16] = 0xC1;
            buffer[17] = WLUN_UFS_DEVICE;
            buffer[24] = 0xC1;
            buffer[25] = WLUN_BOOT;
          }
          else if (sel == 2) {
            // Return four LUN
            buffer[3] = 32;
            buffer[8] = 0xC1;
            buffer[9] = WLUN_REPORT_LUNS;
            buffer[16] = 0xC1;
            buffer[17] = WLUN_UFS_DEVICE;
            buffer[24] = 0xC1;
            buffer[25] = WLUN_BOOT;
          }
          else {
            warn("Unknown select report field %#x", sel);
          }

          length = 40;
        } break;
        case CMD_VERIFY_10: {
          uint32_t slba;
          uint16_t nlb;

          memcpy(&slba, req->cdb + 2, 4);
          memcpy(&nlb, req->cdb + 7, 2);

          slba = __builtin_bswap32(slba);
          nlb = __builtin_bswap16(nlb);

          if (slba + nlb > totalLogicalPages * logicalPageSize) {
            resp->header.status = 0x0;  // Check condition
            // Fill sense data
            // TODO: modulize this error sense part
            resp->header.dataSegmentLength = 20;
            resp->senseLength = 18;
            resp->senseData[0] = 0x70;
            resp->senseData[2] = 0x05;  // Illegal Request
            resp->senseData[7] = 0x0A;
          }
        } break;
        case CMD_WRITE_6: {
          slba = (((uint64_t)(req->cdb[1]) & 0x1F) << 16) |
                 ((uint64_t)req->cdb[2] << 8) | req->cdb[3];
          nlb = req->cdb[4] == 0 ? 256 : req->cdb[4];
        } break;
        case CMD_WRITE_10: {
          // bool fua = req->cdb[1] & 0x08;
          memcpy(&slba, req->cdb + 2, 4);
          memcpy(&nlb, req->cdb + 7, 2);

          slba = __builtin_bswap32(slba);
          nlb = __builtin_bswap16(nlb);
        } break;
        case CMD_REQUEST_SENSE:
        case CMD_FORMAT_UNIT:
        case CMD_PREFETCH_10:
        case CMD_SECURITY_PROTOCOL_IN:
        case CMD_SECURITY_PROTOCOL_OUT:
        case CMD_SEND_DIAGNOSTIC:
          // Not implemented
          break;
        case CMD_SYNCHRONIZE_CACHE_10: {
          bool immed = req->cdb[1] & 0x10;
          memcpy(&slba, req->cdb + 2, 4);
          memcpy(&nlb, req->cdb + 7, 2);

          slba = __builtin_bswap32(slba);
          nlb = __builtin_bswap16(nlb);

          if (immed) {
            static DMAFunction dummy = [](uint64_t, void *) {};

            flush(dummy, nullptr);
          }
          else {
            flush(func, context);

            immediate = false;
          }
        } break;
        default:
          warn("Not implemented SCSI command %#x", opcode);
          break;
      }

      if (opcode == CMD_READ_10 || opcode == CMD_READ_6) {
        if (nlb > 0) {
          DMAFunction doRead = [this](uint64_t tick, void *context) {
            DMAFunction doWrite = [](uint64_t tick, void *context) {
              IOContext *pContext = (IOContext *)context;

              debugprint(LOG_HIL_UFS,
                         "NVM     | READ  | %d + %d | DMA %" PRIu64
                         " - %" PRIu64 " (%" PRIu64 ")",
                         pContext->slba, pContext->nlb, pContext->tick, tick,
                         tick - pContext->tick);

              pContext->func(tick, pContext->context);

              free(pContext->buffer);
              delete pContext;
            };

            IOContext *pContext = (IOContext *)context;

            debugprint(LOG_HIL_UFS,
                       "NVM     | READ  | %d + %d | NAND %" PRIu64 " - %" PRIu64
                       " (%" PRIu64 ")",
                       pContext->slba, pContext->nlb, pContext->beginAt, tick,
                       tick - pContext->beginAt);

            pContext->tick = tick;

            prdtWrite(pContext->prdt, pContext->prdtLength,
                      pContext->nlb * lbaSize, pContext->buffer, doWrite,
                      context);
          };

          IOContext *pContext = new IOContext(func);

          pContext->buffer = (uint8_t *)calloc(nlb * lbaSize, 1);
          pContext->context = context;
          pContext->slba = slba;
          pContext->nlb = nlb;
          pContext->prdt = prdt;
          pContext->prdtLength = prdtLength;

          debugprint(LOG_HIL_UFS, "NVM     | READ  | %d + %d", pContext->slba,
                     pContext->nlb);

          read(slba, nlb, pContext->buffer, doRead, pContext);

          immediate = false;
        }
      }
      else if (opcode == CMD_WRITE_10 || opcode == CMD_WRITE_6) {
        if (nlb > 0) {
          DMAFunction doRead = [this](uint64_t tick, void *context) {
            DMAFunction doWrite = [](uint64_t tick, void *context) {
              IOContext *pContext = (IOContext *)context;

              debugprint(LOG_HIL_UFS,
                         "NVM     | WRITE | %d + %d | NAND %" PRIu64
                         " - %" PRIu64 " (%" PRIu64 ")",
                         pContext->slba, pContext->nlb, pContext->tick, tick,
                         tick - pContext->tick);

              pContext->func(tick, pContext->context);

              free(pContext->buffer);
              delete pContext;
            };

            IOContext *pContext = (IOContext *)context;

            debugprint(LOG_HIL_UFS,
                       "NVM     | WRITE | %d + %d | DMA %" PRIu64 " - %" PRIu64
                       " (%" PRIu64 ")",
                       pContext->slba, pContext->nlb, pContext->beginAt, tick,
                       tick - pContext->beginAt);

            pContext->tick = tick;

            write(pContext->slba, pContext->nlb, pContext->buffer, doWrite,
                  context);
          };

          IOContext *pContext = new IOContext(func);

          pContext->buffer = (uint8_t *)calloc(nlb * lbaSize, 1);
          pContext->context = context;
          pContext->slba = slba;
          pContext->nlb = nlb;

          debugprint(LOG_HIL_UFS, "NVM     | WRITE | %d + %d", pContext->slba,
                     pContext->nlb);

          prdtRead(prdt, prdtLength, nlb * lbaSize, pContext->buffer, doRead,
                   pContext);

          immediate = false;
        }
      }
      else if (opcode == CMD_MODE_SENSE_10 || opcode == CMD_READ_CAPACITY_10 ||
               opcode == CMD_READ_CAPACITY_16 || opcode == CMD_REPORT_LUNS) {
        if (buffer) {
          DMAFunction doWrite = [](uint64_t tick, void *context) {
            IOContext *pContext = (IOContext *)context;

            pContext->func(tick, pContext->context);

            free(pContext->buffer);
            delete pContext;
          };

          IOContext *pContext = new IOContext(func);
          pContext->buffer = buffer;
          pContext->context = context;

          prdtWrite(prdt, prdtLength, length, buffer, doWrite, pContext);

          immediate = false;
        }
      }

      if (immediate) {
        func(getTick(), context);
      }
    } break;
    case CMD_NATIVE_UFS_COMMAND:
      // Not used in UFS v2.1
      break;
    case CMD_DEVICE_MGMT_FUNCTION:
      // Should processed in processQueryCommand
      warn("Device management function on UPIU Command is not "
           "defined operation");
      break;
  }
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
      pDMA->dmaRead(table[idx].dataAddress, size, buffer + read, dmaHandler,
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

  execute(CPU::UFS__DEVICE, CPU::PRDT_READ, doRead, pContext);
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
      pDMA->dmaWrite(table[idx].dataAddress, size, buffer + written, dmaHandler,
                     context);

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

  execute(CPU::UFS__DEVICE, CPU::PRDT_WRITE, doWrite, pContext);
}

void Device::convertUnit(uint64_t slba, uint64_t nlblk, Request &req) {
  static uint64_t lbaSize = conf.readUint(CONFIG_UFS, UFS_LBA_SIZE);
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

  execute(CPU::UFS__DEVICE, CPU::READ, doRead, req);
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

  execute(CPU::UFS__DEVICE, CPU::WRITE, doWrite, req);
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

  execute(CPU::UFS__DEVICE, CPU::FLUSH, doFlush, req);
}

void Device::getStatList(std::vector<Stats> &list, std::string prefix) {
  pHIL->getStatList(list, prefix);
}

void Device::getStatValues(std::vector<double> &values) {
  pHIL->getStatValues(values);
}

void Device::resetStatValues() {
  pHIL->resetStatValues();
}

}  // namespace UFS

}  // namespace HIL

}  // namespace SimpleSSD
