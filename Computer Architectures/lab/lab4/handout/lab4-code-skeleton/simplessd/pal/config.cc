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

#include "pal/config.hh"

#include "util/simplessd.hh"

namespace SimpleSSD {

namespace PAL {

const char NAME_CHANNEL[] = "Channel";
const char NAME_PACKAGE[] = "Package";
const char NAME_PAGE_ALLOCATION[] = "PageAllocation";
const char NAME_SUPER_BLOCK[] = "SuperblockSize";

/* NAND config TODO: seperate this */
const char NAME_DIE[] = "Die";
const char NAME_PLANE[] = "Plane";
const char NAME_BLOCK[] = "Block";
const char NAME_PAGE[] = "Page";
const char NAME_PAGE_SIZE[] = "PageSize";
const char NAME_USE_MULTI_PLANE_OP[] = "EnableMultiPlaneOperation";
const char NAME_DMA_SPEED[] = "DMASpeed";
const char NAME_DMA_WIDTH[] = "DMAWidth";
const char NAME_FLASH_TYPE[] = "NANDType";

/* NAND timing TODO: seperate this */
const char NAME_NAND_LSB_READ[] = "LSBRead";
const char NAME_NAND_LSB_WRITE[] = "LSBWrite";
const char NAME_NAND_CSB_READ[] = "CSBRead";
const char NAME_NAND_CSB_WRITE[] = "CSBWrite";
const char NAME_NAND_MSB_READ[] = "MSBRead";
const char NAME_NAND_MSB_WRITE[] = "MSBWrite";
const char NAME_NAND_ERASE[] = "Erase";

/* NAND power TODO: seperate this */
const char NAME_NAND_VOLTAGE[] = "Voltage";
const char NAME_NAND_CURRENT_READ[] = "ReadCurrent";
const char NAME_NAND_CURRENT_PROGRAM[] = "ProgramCurrent";
const char NAME_NAND_CURRENT_ERASE[] = "EraseCurrent";
const char NAME_NAND_CURRENT_IDLE[] = "IdleCurrent";
const char NAME_NAND_CURRENT_STANDBY[] = "StandbyCurrent";

/* Constants for calculating DMA time based on ONFI 3.x spec */
// READ : <00h> <C1> <C2> <R1> <R2> <R3> <30h> [tWB] [tR] [tRR] <DATA>
// WRITE: <80h> <C1> <C2> <R1> <R2> <R3> [tADL] <DATA> <10h> [tWB] [tPROG]
// ERASE: <60h> <R1> <R2> <R3> <D0h> [tWB] [tBERS]
const uint8_t readCycle = 7;
const uint8_t writeCycle = 7;
const uint8_t eraseCycle = 5;

Config::Config() {
  channel = 8;
  package = 4;
  die = 2;
  plane = 1;
  block = 512;
  page = 512;
  pageSize = 16384;
  useMultiPlaneOperation = true;
  dmaSpeed = 400;
  dmaWidth = 8;
  nandType = NAND_MLC;

  // Set NAND timing (Default: MLC, csb is not used)
  nandTiming.lsb.read = 40000000;    // 40us
  nandTiming.lsb.write = 500000000;  // 500us
  nandTiming.csb.read = 0;
  nandTiming.csb.write = 0;
  nandTiming.msb.read = 65000000;     // 65us
  nandTiming.msb.write = 1300000000;  // 1300us
  nandTiming.erase = 3500000000;      // 3.5ms

  // Set NAND power (From: Micron's MT29F64*)
  nandPower.voltage = 3300;           // 3.3V
  nandPower.current.read = 25000;     // 25mA
  nandPower.current.program = 25000;  // 25mA
  nandPower.current.erase = 25000;    // 25mA
  nandPower.current.busIdle = 5000;   // 5mA
  nandPower.current.standby = 10;     // 10uA

  superblock = 0;
  memset(PageAllocation, 0, 4);
}

bool Config::setConfig(const char *name, const char *value) {
  bool ret = true;

  if (MATCH_NAME(NAME_CHANNEL)) {
    channel = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_PACKAGE)) {
    package = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DIE)) {
    die = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_PLANE)) {
    plane = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_BLOCK)) {
    block = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_PAGE)) {
    page = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_PAGE_SIZE)) {
    pageSize = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_USE_MULTI_PLANE_OP)) {
    useMultiPlaneOperation = convertBool(value);
  }
  else if (MATCH_NAME(NAME_DMA_SPEED)) {
    dmaSpeed = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DMA_WIDTH)) {
    dmaWidth = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_FLASH_TYPE)) {
    nandType = (NAND_TYPE)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_SUPER_BLOCK)) {
    _superblock = value;
  }
  else if (MATCH_NAME(NAME_PAGE_ALLOCATION)) {
    _pageAllocation = value;
  }
  else if (MATCH_NAME(NAME_NAND_LSB_READ)) {
    nandTiming.lsb.read = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_LSB_WRITE)) {
    nandTiming.lsb.write = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_CSB_READ)) {
    nandTiming.csb.read = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_CSB_WRITE)) {
    nandTiming.csb.write = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_MSB_READ)) {
    nandTiming.msb.read = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_MSB_WRITE)) {
    nandTiming.msb.write = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_ERASE)) {
    nandTiming.erase = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_VOLTAGE)) {
    nandPower.voltage = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_CURRENT_READ)) {
    nandPower.current.read = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_CURRENT_PROGRAM)) {
    nandPower.current.program = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_CURRENT_ERASE)) {
    nandPower.current.erase = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_CURRENT_IDLE)) {
    nandPower.current.busIdle = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_NAND_CURRENT_STANDBY)) {
    nandPower.current.standby = strtoul(value, nullptr, 10);
  }
  else {
    ret = false;
  }

  return ret;
}

void Config::update() {
  if (dmaWidth & 0x07) {
    panic("dmaWidth should be multiple of 8.");
  }

  // DMA time calculation
  //                 MT/s       MT -> T    ms     us     ns     ps
  float tCK = 1.f / (dmaSpeed * 1048576) * 1000 * 1000 * 1000 * 1000;

  nandTiming.dma0.read = readCycle * tCK / (dmaWidth / 8);
  nandTiming.dma0.write = (writeCycle + pageSize) * tCK / (dmaWidth / 8);
  nandTiming.dma0.erase = eraseCycle * tCK / (dmaWidth / 8);
  nandTiming.dma1.read = pageSize * tCK / (dmaWidth / 8);
  nandTiming.dma1.write = 1 * tCK / (dmaWidth / 8);
  nandTiming.dma1.erase = 1 * tCK / (dmaWidth / 8);

  // Parse page allocation setting
  int i = 0;
  uint8_t check = 0;
  bool fail = false;

  for (auto iter : _pageAllocation) {
    if ((iter == 'C') | (iter == 'c')) {
      if (check & INDEX_CHANNEL) {
        fail = true;
      }

      PageAllocation[i++] = INDEX_CHANNEL;
      check |= INDEX_CHANNEL;
    }
    else if ((iter == 'W') | (iter == 'w')) {
      if (check & INDEX_PACKAGE) {
        fail = true;
      }

      PageAllocation[i++] = INDEX_PACKAGE;
      check |= INDEX_PACKAGE;
    }
    else if ((iter == 'D') | (iter == 'd')) {
      if (check & INDEX_DIE) {
        fail = true;
      }

      PageAllocation[i++] = INDEX_DIE;
      check |= INDEX_DIE;
    }
    else if ((iter == 'P') | (iter == 'p')) {
      if (check & INDEX_PLANE) {
        fail = true;
      }

      PageAllocation[i++] = INDEX_PLANE;
      check |= INDEX_PLANE;
    }

    if (i == 4) {
      break;
    }
  }

  if (check != (INDEX_CHANNEL | INDEX_PACKAGE | INDEX_DIE | INDEX_PLANE)) {
    fail = true;
  }

  if (useMultiPlaneOperation) {
    // Move P to front
    for (i = 0; i < 4; i++) {
      if (PageAllocation[i] == INDEX_PLANE) {
        for (int j = i; j > 0; j--) {
          PageAllocation[j] = PageAllocation[j - 1];
        }

        PageAllocation[0] = INDEX_PLANE;

        break;
      }
    }
  }

  if (fail) {
    panic("Invalid page allocation string");
  }

  // Parse super block size setting
  if (_superblock.length() > 0) {
    superblock = 0x00;

    for (auto iter : _superblock) {
      if ((iter == 'C') | (iter == 'c')) {
        superblock |= INDEX_CHANNEL;
      }
      else if ((iter == 'W') | (iter == 'w')) {
        superblock |= INDEX_PACKAGE;
      }
      else if ((iter == 'D') | (iter == 'd')) {
        superblock |= INDEX_DIE;
      }
      else if ((iter == 'P') | (iter == 'p')) {
        superblock |= INDEX_PLANE;
      }
    }
  }

  if (useMultiPlaneOperation) {
    superblock |= INDEX_PLANE;
  }
}

int64_t Config::readInt(uint32_t idx) {
  int64_t ret = 0;

  switch (idx) {
    case NAND_FLASH_TYPE:
      ret = nandType;
      break;
  }

  return ret;
}

uint64_t Config::readUint(uint32_t idx) {
  uint64_t ret = 0;

  switch (idx) {
    case PAL_CHANNEL:
      ret = channel;
      break;
    case PAL_PACKAGE:
      ret = package;
      break;
    case NAND_DIE:
      ret = die;
      break;
    case NAND_PLANE:
      ret = plane;
      break;
    case NAND_BLOCK:
      ret = block;
      break;
    case NAND_PAGE:
      ret = page;
      break;
    case NAND_PAGE_SIZE:
      ret = pageSize;
      break;
    case NAND_DMA_SPEED:
      ret = dmaSpeed;
      break;
    case NAND_DMA_WIDTH:
      ret = dmaWidth;
      break;
  }

  return ret;
}

bool Config::readBoolean(uint32_t idx) {
  bool ret = false;

  switch (idx) {
    case NAND_USE_MULTI_PLANE_OP:
      ret = useMultiPlaneOperation;
      break;
  }

  return ret;
}

uint8_t Config::getSuperblockConfig() {
  return superblock;
}

uint32_t Config::getPageAllocationConfig() {
  return (uint32_t)PageAllocation[0] | ((uint32_t)PageAllocation[1] << 8) |
         ((uint32_t)PageAllocation[2] << 16) |
         ((uint32_t)PageAllocation[3] << 24);
}

Config::NANDTiming *Config::getNANDTiming() {
  return &nandTiming;
}

Config::NANDPower *Config::getNANDPower() {
  return &nandPower;
}

}  // namespace PAL

}  // namespace SimpleSSD
