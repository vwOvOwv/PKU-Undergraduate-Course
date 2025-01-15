/*
 * UFS Definitions
 *
 * Author: Donghyun Gouk <kukdh1@camelab.org>
 */

#include "hil/ufs/def.hh"

#ifdef _MSC_VER
#define htobe16(x) ((x) >> 8 | ((x)&0xFF) << 8)
#define htobe32(x) (htobe16((x) >> 16) & 0xFFFF | htobe16((x)&0xFFFF) << 16)
#define be16toh(x) htobe16(x)
#define be32toh(x) htobe32(x)
#else
#include <endian.h>
#endif

#include <cstring>
#include <iostream>

UFSHCIRegister::UFSHCIRegister() {
  memset(data, 0, 0xB0);
}

UPIU::UPIU() {
  header.transactionType = 0;
  header.flags = 0;
  header.lun = 0;
  header.taskTag = 0;
  header.commandSetType = 0;
  header.function = 0;
  header.response = 0;
  header.status = 0;
  header.ehsLength = 0;
  header.deviceInfo = 0;
  header.dataSegmentLength = 0;
}

UPIU::~UPIU() {}

bool UPIU::set(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 11) {
    header.transactionType = buffer[0];
    header.flags = buffer[1];
    header.lun = buffer[2];
    header.taskTag = buffer[3];
    header.commandSetType = buffer[4];
    header.function = buffer[5];
    header.response = buffer[6];
    header.status = buffer[7];
    header.ehsLength = buffer[8];
    header.deviceInfo = buffer[9];
    header.dataSegmentLength = ((uint16_t)buffer[10] << 8) | buffer[11];

    ret = true;
  }

  return ret;
}

bool UPIU::get(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 11) {
    buffer[0] = header.transactionType & 0x3F;
    buffer[1] = header.flags;
    buffer[2] = header.lun;
    buffer[3] = header.taskTag;
    buffer[4] = header.commandSetType;
    buffer[5] = header.function;
    buffer[6] = header.response;
    buffer[7] = header.status;
    buffer[8] = header.ehsLength;
    buffer[9] = header.deviceInfo;
    buffer[10] = header.dataSegmentLength >> 8;
    buffer[11] = header.dataSegmentLength & 0xFF;

    ret = true;
  }

  return ret;
}

uint32_t UPIU::getLength() {
  return 12;
}

UPIUCommand::UPIUCommand() : UPIU(), expectedDataLength(0) {
  memset(cdb, 0, 16);
}

UPIUCommand::~UPIUCommand() {}

bool UPIUCommand::set(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if ((buffer[0] & 0x3F) == OPCODE_COMMAND && length > 31) {
    uint32_t temp;

    ret = UPIU::set(buffer, length);

    memcpy(&temp, buffer + 12, 4);
    expectedDataLength = be32toh(temp);

    memcpy(cdb, buffer + 16, 16);
  }

  return ret;
}

bool UPIUCommand::get(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 31) {
    uint32_t temp;

    header.transactionType = OPCODE_COMMAND;
    ret = UPIU::get(buffer, length);

    temp = htobe32(expectedDataLength);
    memcpy(buffer + 12, &temp, 4);

    memcpy(buffer + 16, cdb, 16);
  }

  return ret;
}

uint32_t UPIUCommand::getLength() {
  return 32;
}

UPIUResponse::UPIUResponse() : UPIU(), residualCount(0), senseLength(0) {
  memset(senseData, 0, 18);
}

UPIUResponse::~UPIUResponse() {}

bool UPIUResponse::set(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if ((buffer[0] & 0x3F) == OPCODE_RESPONSE && length > 31) {
    uint32_t temp;

    ret = UPIU::set(buffer, length);

    memcpy(&temp, buffer + 12, 4);
    residualCount = be32toh(temp);

    if (header.dataSegmentLength > 0) {
      uint32_t offset = (header.transactionType & 0x80) ? 4 : 0;

      if (length > 31 + offset + header.dataSegmentLength) {
        senseLength =
            ((uint16_t)buffer[32 + offset] << 8) | buffer[33 + offset];
        memcpy(senseData, buffer + 34 + offset, senseLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

bool UPIUResponse::get(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 31) {
    uint32_t temp;

    header.transactionType = OPCODE_RESPONSE;
    ret = UPIU::get(buffer, length);

    temp = htobe32(residualCount);
    memcpy(buffer + 12, &temp, 4);

    memset(buffer + 16, 0, 16);

    if (header.dataSegmentLength > 0) {
      if (length > 31ul + header.dataSegmentLength) {
        buffer[32] = senseLength >> 8;
        buffer[33] = senseLength & 0xFF;
        memcpy(buffer + 34, senseData, senseLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

uint32_t UPIUResponse::getLength() {
  return 32 + header.dataSegmentLength;
}

UPIUQueryReq::UPIUQueryReq() : UPIU(), data(nullptr) {}

UPIUQueryReq::~UPIUQueryReq() {
  if (data) {
    free(data);
  }
}

bool UPIUQueryReq::set(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if ((buffer[0] & 0x3F) == OPCODE_QUERY_REQUEST && length > 31) {
    uint32_t temp;

    ret = UPIU::set(buffer, length);

    opcode = buffer[12];
    idn = buffer[13];
    index = buffer[14];
    selector = buffer[15];
    this->length = ((uint16_t)buffer[18] << 8) | buffer[19];

    memcpy(&temp, buffer + 20, 4);
    val1 = be32toh(temp);

    memcpy(&temp, buffer + 24, 4);
    val2 = be32toh(temp);

    if (header.dataSegmentLength > 0) {
      uint32_t offset = (header.transactionType & 0x80) ? 4 : 0;

      if (length > 31ul + offset + header.dataSegmentLength) {
        if (data) {
          free(data);
        }
        data = (uint8_t *)calloc(header.dataSegmentLength, 1);
        memcpy(data, buffer + 32 + offset, header.dataSegmentLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

bool UPIUQueryReq::get(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 31) {
    uint32_t temp;

    header.transactionType = OPCODE_QUERY_REQUEST;
    ret = UPIU::get(buffer, length);

    buffer[12] = opcode;
    buffer[13] = idn;
    buffer[14] = index;
    buffer[15] = selector;
    buffer[18] = this->length >> 8;
    buffer[19] = this->length & 0xFF;

    temp = htobe32(val1);
    memcpy(buffer + 20, &temp, 4);

    temp = htobe32(val2);
    memcpy(buffer + 24, &temp, 4);

    memset(buffer + 28, 0, 4);

    if (header.dataSegmentLength > 0) {
      if (length > 31ul + header.dataSegmentLength) {
        memcpy(buffer + 32, data, header.dataSegmentLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

uint32_t UPIUQueryReq::getLength() {
  return 32 + header.dataSegmentLength;
}

UPIUQueryResp::UPIUQueryResp() : UPIU(), data(nullptr) {}

UPIUQueryResp::~UPIUQueryResp() {
  if (data) {
    free(data);
  }
}

bool UPIUQueryResp::set(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if ((buffer[0] & 0x3F) == OPCODE_QUERY_RESPONSE && length > 31) {
    uint32_t temp;

    ret = UPIU::set(buffer, length);

    opcode = buffer[12];
    idn = buffer[13];
    index = buffer[14];
    selector = buffer[15];
    this->length = ((uint16_t)buffer[18] << 8) | buffer[19];

    memcpy(&temp, buffer + 20, 4);
    val1 = be32toh(temp);

    memcpy(&temp, buffer + 24, 4);
    val2 = be32toh(temp);

    if (header.dataSegmentLength > 0) {
      uint32_t offset = (header.transactionType & 0x80) ? 4 : 0;

      if (length > 31ul + offset + header.dataSegmentLength) {
        if (data) {
          free(data);
        }
        data = (uint8_t *)calloc(header.dataSegmentLength, 1);
        memcpy(data, buffer + 32 + offset, header.dataSegmentLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

bool UPIUQueryResp::get(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 31) {
    uint32_t temp;

    header.transactionType = OPCODE_QUERY_RESPONSE;
    ret = UPIU::get(buffer, length);

    buffer[12] = opcode;
    buffer[13] = idn;
    buffer[14] = index;
    buffer[15] = selector;
    buffer[18] = this->length >> 8;
    buffer[19] = this->length & 0xFF;

    temp = htobe32(val1);
    memcpy(buffer + 20, &temp, 4);

    temp = htobe32(val2);
    memcpy(buffer + 24, &temp, 4);

    memset(buffer + 28, 0, 4);

    if (header.dataSegmentLength > 0) {
      if (length > 31ul + header.dataSegmentLength) {
        memcpy(buffer + 32, data, header.dataSegmentLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

uint32_t UPIUQueryResp::getLength() {
  return 32 + header.dataSegmentLength;
}

UPIUDataOut::UPIUDataOut() : UPIU(), offset(0), count(0), data(nullptr) {}

UPIUDataOut::~UPIUDataOut() {
  if (data) {
    free(data);
  }
}

bool UPIUDataOut::set(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if ((buffer[0] & 0x3F) == OPCODE_DATA_OUT && length > 31) {
    uint32_t temp;

    ret = UPIU::set(buffer, length);

    memcpy(&temp, buffer + 12, 4);
    offset = be32toh(temp);

    memcpy(&temp, buffer + 16, 4);
    count = be32toh(temp);

    if (header.dataSegmentLength > 0) {
      uint32_t offset = (header.transactionType & 0x80) ? 4 : 0;

      if (length > 31ul + offset + header.dataSegmentLength) {
        if (data) {
          free(data);
        }
        data = (uint8_t *)calloc(header.dataSegmentLength, 1);
        memcpy(data, buffer + 32 + offset, header.dataSegmentLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

bool UPIUDataOut::get(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 31) {
    uint32_t temp;

    header.transactionType = OPCODE_DATA_OUT;
    ret = UPIU::get(buffer, length);

    temp = htobe32(offset);
    memcpy(buffer + 12, &temp, 4);

    temp = htobe32(count);
    memcpy(buffer + 16, &temp, 4);

    memset(buffer + 20, 0, 12);

    if (header.dataSegmentLength > 0) {
      if (length > 31ul + header.dataSegmentLength) {
        memcpy(buffer + 32, data, header.dataSegmentLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

uint32_t UPIUDataOut::getLength() {
  return 32 + header.dataSegmentLength;
}

UPIUDataIn::UPIUDataIn() : UPIU(), offset(0), count(0), data(nullptr) {}

UPIUDataIn::~UPIUDataIn() {
  if (data) {
    free(data);
  }
}

bool UPIUDataIn::set(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if ((buffer[0] & 0x3F) == OPCODE_DATA_IN && length > 31) {
    uint32_t temp;

    ret = UPIU::set(buffer, length);

    memcpy(&temp, buffer + 12, 4);
    offset = be32toh(temp);

    memcpy(&temp, buffer + 16, 4);
    count = be32toh(temp);

    if (header.dataSegmentLength > 0) {
      uint32_t offset = (header.transactionType & 0x80) ? 4 : 0;

      if (length > 31ul + offset + header.dataSegmentLength) {
        if (data) {
          free(data);
        }
        data = (uint8_t *)calloc(header.dataSegmentLength, 1);
        memcpy(data, buffer + 32 + offset, header.dataSegmentLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

bool UPIUDataIn::get(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 31) {
    uint32_t temp;

    header.transactionType = OPCODE_DATA_IN;
    ret = UPIU::get(buffer, length);

    temp = htobe32(offset);
    memcpy(buffer + 12, &temp, 4);

    temp = htobe32(count);
    memcpy(buffer + 16, &temp, 4);

    memset(buffer + 20, 0, 12);

    if (header.dataSegmentLength > 0) {
      if (length > 31ul + header.dataSegmentLength) {
        memcpy(buffer + 32, data, header.dataSegmentLength);
      }
      else {
        ret = false;
      }
    }
  }

  return ret;
}

uint32_t UPIUDataIn::getLength() {
  return 32 + header.dataSegmentLength;
}

UPIUReadyToTransfer::UPIUReadyToTransfer() : UPIU(), offset(0), count(0) {}

UPIUReadyToTransfer::~UPIUReadyToTransfer() {}

bool UPIUReadyToTransfer::set(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if ((buffer[0] & 0x3F) == OPCODE_DATA_IN && length > 31) {
    uint32_t temp;

    ret = UPIU::set(buffer, length);

    memcpy(&temp, buffer + 12, 4);
    offset = be32toh(temp);

    memcpy(&temp, buffer + 16, 4);
    count = be32toh(temp);
  }

  return ret;
}

bool UPIUReadyToTransfer::get(uint8_t *buffer, uint32_t length) {
  bool ret = false;

  if (length > 31) {
    uint32_t temp;

    header.transactionType = OPCODE_READY_TO_TRANSFER;
    ret = UPIU::get(buffer, length);

    temp = htobe32(offset);
    memcpy(buffer + 12, &temp, 4);

    temp = htobe32(count);
    memcpy(buffer + 16, &temp, 4);

    memset(buffer + 20, 0, 12);
  }

  return ret;
}

uint32_t UPIUReadyToTransfer::getLength() {
  return 32;
}
