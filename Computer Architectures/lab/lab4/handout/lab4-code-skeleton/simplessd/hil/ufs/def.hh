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

#ifndef __HIL_UFS_DEF__
#define __HIL_UFS_DEF__

#include <cinttypes>

union UFSHCIRegister {
  uint8_t data[0xB0];
  struct {
    /* Host Capabilities */
    uint32_t cap;
    uint32_t reserved0;
    uint32_t version;
    uint32_t reserved1;
    uint32_t hcddid;
    uint32_t hcpmid;
    uint32_t reserved2[2];

    /* Operation Runtime */
    uint32_t is;
    uint32_t ie;
    uint32_t reserved3[2];
    uint32_t hcs;
    uint32_t hce;
    uint32_t uecpa;
    uint32_t uecdl;
    uint32_t uecn;
    uint32_t uect;
    uint32_t uecdme;
    uint32_t utriacr;

    /* UFS Transport Protocol Transfer */
    uint64_t utrlba;
    uint32_t utrldbr;
    uint32_t utrlclr;
    uint32_t utrlrsr;
    uint32_t reserved4[3];

    /* UFS Transport Protocol Task Management */
    uint64_t utmrlba;
    uint32_t utmrldbr;
    uint32_t utmrlclr;
    uint32_t utmrlrsr;
    uint32_t reserved5[3];

    /* UFS InterConnect Command */
    uint32_t uiccmdr;
    uint32_t ucmdarg1;
    uint32_t ucmdarg2;
    uint32_t ucmdarg3;
    uint32_t reserved6[4];
  };

  UFSHCIRegister();
};

enum UFSHCIREG {
  REG_CAP = 0x00,
  REG_VER = 0x08,
  REG_HCDDID = 0x10,
  REG_HCPMID = 0x14,
  REG_IS = 0x20,
  REG_IE = 0x24,
  REG_HCS = 0x30,
  REG_HCE = 0x34,
  REG_UECPA = 0x38,
  REG_UECDL = 0x3C,
  REG_UECN = 0x40,
  REG_UECT = 0x44,
  REG_UECDME = 0x48,
  REG_UTRIACR = 0x4C,
  REG_UTRLBA = 0x50,
  REG_UTRLBAU = 0x54,
  REG_UTRLDBR = 0x58,
  REG_UTRLCLR = 0x5C,
  REG_UTRLRSR = 0x60,
  REG_UTMRLBA = 0x70,
  REG_UTMRLBAU = 0x74,
  REG_UTMRLDBR = 0x78,
  REG_UTMRLCLR = 0x7C,
  REG_UTMRLRSR = 0x80,
  REG_UICCMDR = 0x90,
  REG_UCMDARG1 = 0x94,
  REG_UCMDARG2 = 0x98,
  REG_UCMDARG3 = 0x9C,
};

enum DMECMD {
  DME_GET = 0x01,
  DME_SET,
  DME_PEER_GET,
  DME_PEER_SET,
  DME_POWERON = 0x10,
  DME_POWEROFF,
  DME_ENABLE,
  DME_RESET = 0x14,
  DME_ENDPOINTRESET,
  DME_LINKSTARTUP,
  DME_HIBERNATE_ENTER,
  DME_HIBERNATE_EXIT,
  DME_TEST_MODE = 0x1A
};

enum DMEERR {
  ERR_SUCCESS,
  ERR_INVALID_MIB,
  ERR_INVALID_MIB_VALUE,
  ERR_READ_ONLY_MIB,
  ERR_WRITE_ONLY_MIB,
  ERR_BAD_INDEX,
  ERR_LOCKED_MIB,
  ERR_BAD_TEST_FEATURE_INDEX,
  ERR_PEER_COMMUNICATION_FAILURE,
  ERR_BUSY,
  ERR_DME_FAILURE
};

enum MIB {
  // See Linux kernel /drivers/scsi/ufs/unipro.h
  MIB_VS_POWERSTATE = 0xD083
};

enum UFSLINK {
  // See Linux kernel /drivers/scsi/ufs/unihci.h
  UFSHCD_LINK_IS_DOWN = 1,
  UFSHCD_LINK_IS_UP
};

#define MAKE_UICARG(mib, selector)                                             \
  (((uint32_t)(mib) << 16) | ((uint32_t)(selector)&0xFFFF))

#define UFS_BIT(x) (1L << (x))

#define UTP_TRANSFER_REQ_COMPL UFS_BIT(0)
#define UIC_DME_END_PT_RESET UFS_BIT(1)
#define UIC_ERROR UFS_BIT(2)
#define UIC_TEST_MODE UFS_BIT(3)
#define UIC_POWER_MODE UFS_BIT(4)
#define UIC_HIBERNATE_EXIT UFS_BIT(5)
#define UIC_HIBERNATE_ENTER UFS_BIT(6)
#define UIC_LINK_LOST UFS_BIT(7)
#define UIC_LINK_STARTUP UFS_BIT(8)
#define UTP_TASK_REQ_COMPL UFS_BIT(9)
#define UIC_COMMAND_COMPL UFS_BIT(10)
#define DEVICE_FATAL_ERROR UFS_BIT(11)
#define CONTROLLER_FATAL_ERROR UFS_BIT(16)
#define SYSTEM_BUS_FATAL_ERROR UFS_BIT(17)

#define UTP_TRANSFER_REQ_DESC_SIZE 32

union UTPTransferReqDesc {
  uint8_t data[UTP_TRANSFER_REQ_DESC_SIZE];
  struct {
    uint32_t dw0;
    uint32_t dw1;
    uint32_t dw2;
    uint32_t dw3;
    uint32_t cmdAddress;
    uint32_t cmdAddressUpper;
    uint16_t respUPIULength;
    uint16_t respUPIUOffset;
    uint16_t prdtLength;
    uint16_t prdtOffset;
  };
};

enum UTP_TRANSFER_CMD {
  CMD_SCSI,
  CMD_NATIVE_UFS_COMMAND,
  CMD_DEVICE_MGMT_FUNCTION
};

union PRDT {
  uint8_t data[16];
  struct {
    uint64_t dataAddress;
    uint32_t dw2;
    uint32_t dw3;
  };
};

enum UPIU_OPCODE {              // Response OPCODE
  OPCODE_NOP_OUT = 0x00,        // OPCODE_NOP_IN
  OPCODE_COMMAND,               // OPCODE_RESPONSE
  OPCODE_DATA_OUT,              // OPCODE_RESPONSE
  OPCODE_TASK_MGMT_REQ = 0x04,  // OPCODE_TASK_MGMT_RESP
  OPCODE_QUERY_REQUEST = 0x16,  // OPCODE_QUERY_RESPONSE
  OPCODE_NOP_IN = 0x20,
  OPCODE_RESPONSE,
  OPCODE_DATA_IN,
  OPCODE_TASK_MGMT_RESP = 0x24,
  OPCODE_READY_TO_TRANSFER = 0x31,
  OPCODE_QUERY_RESPONSE = 0x36,
  OPCODE_REJECT_UPIU = 0x3F
};

struct UPIUHeader {
  uint8_t transactionType;
  uint8_t flags;
  uint8_t lun;
  uint8_t taskTag;
  uint8_t commandSetType;
  uint8_t function;
  uint8_t response;
  uint8_t status;
  uint8_t ehsLength;
  uint8_t deviceInfo;
  uint16_t dataSegmentLength;
};

struct UPIU {
  UPIUHeader header;

  UPIU();
  virtual ~UPIU();

  virtual bool set(uint8_t *, uint32_t);
  virtual bool get(uint8_t *, uint32_t);
  virtual uint32_t getLength();
};

struct UPIUCommand : public UPIU {
  uint32_t expectedDataLength;
  uint8_t cdb[16];

  UPIUCommand();
  ~UPIUCommand();

  bool set(uint8_t *, uint32_t);
  bool get(uint8_t *, uint32_t);
  uint32_t getLength();
};

struct UPIUResponse : public UPIU {
  uint32_t residualCount;

  uint16_t senseLength;
  uint8_t senseData[18];

  UPIUResponse();
  ~UPIUResponse();

  bool set(uint8_t *, uint32_t);
  bool get(uint8_t *, uint32_t);
  uint32_t getLength();
};

struct UPIUQueryReq : public UPIU {
  uint8_t opcode;
  uint8_t idn;
  uint8_t index;
  uint8_t selector;
  uint16_t length;
  uint32_t val1;
  uint32_t val2;

  uint8_t *data;

  UPIUQueryReq();
  ~UPIUQueryReq();

  bool set(uint8_t *, uint32_t);
  bool get(uint8_t *, uint32_t);
  uint32_t getLength();
};

struct UPIUQueryResp : public UPIU {
  uint8_t opcode;
  uint8_t idn;
  uint8_t index;
  uint8_t selector;
  uint16_t length;
  uint32_t val1;
  uint32_t val2;

  uint8_t *data;

  UPIUQueryResp();
  ~UPIUQueryResp();

  bool set(uint8_t *, uint32_t);
  bool get(uint8_t *, uint32_t);
  uint32_t getLength();
};

struct UPIUDataOut : public UPIU {
  uint32_t offset;
  uint32_t count;

  uint8_t *data;

  UPIUDataOut();
  ~UPIUDataOut();

  bool set(uint8_t *, uint32_t);
  bool get(uint8_t *, uint32_t);
  uint32_t getLength();
};

struct UPIUDataIn : public UPIU {
  uint32_t offset;
  uint32_t count;

  uint8_t *data;

  UPIUDataIn();
  ~UPIUDataIn();

  bool set(uint8_t *, uint32_t);
  bool get(uint8_t *, uint32_t);
  uint32_t getLength();
};

struct UPIUReadyToTransfer : public UPIU {
  uint32_t offset;
  uint32_t count;

  UPIUReadyToTransfer();
  ~UPIUReadyToTransfer();

  bool set(uint8_t *, uint32_t);
  bool get(uint8_t *, uint32_t);
  uint32_t getLength();
};

enum QUERY_OPCODE {
  OPCODE_NOP,
  OPCODE_READ_DESCRIPTOR,
  OPCODE_WRITE_DESCRIPTOR,
  OPCODE_READ_ATTRIBUTE,
  OPCODE_WRITE_ATTRIBUTE,
  OPCODE_READ_FLAG,
  OPCODE_SET_FLAG,
  OPCODE_CLEAR_FLAG,
  OPCODE_TOGGLE_FLAG
};

enum DESCRIPTOR_IDN {
  IDN_DEVICE,
  IDN_CONFIGURATION,
  IDN_UNIT,
  IDN_INTERCONNECT = 0x04,
  IDN_STRING,
  IDN_GEOMETRY = 0x07,
  IDN_POWER,
  IDN_DEVICE_HEALTH
};

#define IDN_DEVICE_LENGTH 0x1F  // Linux kernel v4.9.30 /driver/scsi/ufs/ufs.h
#define IDN_POWER_LENGTH 0x62
#define IDN_STRING_LENGTH 0xFE  // Linux kernel v4.9.30 /driver/scsi/ufs/ufs.h
#define IDN_UNIT_LENGTH 0x23

enum SCSI_COMMAND {
  /* SPC-4 */
  CMD_INQUIRY = 0x12,
  CMD_MODE_SELECT_10 = 0x55,
  CMD_MODE_SENSE_10 = 0x5A,
  CMD_REPORT_LUNS = 0xA0,
  CMD_REQUEST_SENSE = 0x03,
  CMD_SECURITY_PROTOCOL_IN = 0xA2,
  CMD_SECURITY_PROTOCOL_OUT = 0xB5,
  CMD_SEND_DIAGNOSTIC = 0x1D,
  CMD_TEST_UNIT_READY = 0x00,

  /* SBC-3 */
  CMD_FORMAT_UNIT = 0x04,
  CMD_PREFETCH_10 = 0x34,
  CMD_READ_6 = 0x08,
  CMD_READ_10 = 0x28,
  CMD_READ_CAPACITY_10 = 0x25,
  CMD_READ_CAPACITY_16 = 0x9E,
  CMD_START_STOP_UNIT = 0x1B,
  CMD_VERIFY_10 = 0x2F,
  CMD_WRITE_6 = 0x0A,
  CMD_WRITE_10 = 0x2A,
  CMD_SYNCHRONIZE_CACHE_10 = 0x35,
};

#define SCSI_INQUIRY_LENGTH 36

enum WLUN {
  WLUN_REPORT_LUNS = 0x01,
  WLUN_UFS_DEVICE = 0x50,
  WLUN_BOOT = 0x30,
  WLUN_RPMB = 0x44,
};

/* This is vendor specific */
enum DEVICE_STRING_INDEX {
  STRING_MANUFACTURER,
  STRING_PRODUCT_NAME,
  STRING_SERIAL_NUMBER,
  STRING_OEM_ID,
  STRING_PRODUCT_REVISION_LEVEL
};

#endif
