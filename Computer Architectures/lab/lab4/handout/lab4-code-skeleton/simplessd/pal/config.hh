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

#ifndef __PAL_CONFIG__
#define __PAL_CONFIG__

#include "sim/base_config.hh"

namespace SimpleSSD {

namespace PAL {

typedef enum {
  /* PAL config */
  PAL_CHANNEL,
  PAL_PACKAGE,

  /* NAND config TODO: seperate this */
  NAND_DIE,
  NAND_PLANE,
  NAND_BLOCK,
  NAND_PAGE,
  NAND_PAGE_SIZE,
  NAND_USE_MULTI_PLANE_OP,
  NAND_DMA_SPEED,
  NAND_DMA_WIDTH,
  NAND_FLASH_TYPE,
} PAL_CONFIG;

typedef enum {
  NAND_SLC,
  NAND_MLC,
  NAND_TLC,
} NAND_TYPE;

typedef enum : uint8_t {
  INDEX_CHANNEL = 0x01,
  INDEX_PACKAGE = 0x02,
  INDEX_DIE = 0x04,
  INDEX_PLANE = 0x08,
} ADDR_INDEX;

class Config : public BaseConfig {
 public:
  typedef struct {
    uint64_t read;
    uint64_t write;
  } PAGETiming;

  typedef struct {
    uint64_t read;
    uint64_t write;
    uint64_t erase;
  } DMATiming;

  typedef struct {
    PAGETiming lsb;
    PAGETiming csb;
    PAGETiming msb;
    DMATiming dma0;
    DMATiming dma1;
    uint64_t erase;
  } NANDTiming;

  typedef struct {
    uint64_t voltage;  //!< Unit: mV
    struct {           //!< Unit: uA
      uint64_t read;
      uint64_t program;
      uint64_t erase;
      uint64_t busIdle;
      uint64_t standby;
    } current;
  } NANDPower;

 private:
  uint32_t channel;  //!< Default: 8
  uint32_t package;  //!< Default: 4

  uint32_t die;                 //!< Default: 2
  uint32_t plane;               //!< Default: 1
  uint32_t block;               //!< Default: 512
  uint32_t page;                //!< Default: 512
  uint32_t pageSize;            //!< Default: 16384
  bool useMultiPlaneOperation;  //!< Default: true
  uint32_t dmaSpeed;            //!< Default: 400
  uint32_t dmaWidth;            //!< Default: 8
  NAND_TYPE nandType;           //!< Default: NAND_MLC
  uint8_t superblock;           //!< Default: All (0x0F)
  uint8_t PageAllocation[4];    //!< Default: CWDP (0x01, 0x02, 0x04, 0x08)

  NANDTiming nandTiming;
  NANDPower nandPower;

  // Raw variable
  std::string _superblock;
  std::string _pageAllocation;

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  int64_t readInt(uint32_t) override;
  uint64_t readUint(uint32_t) override;
  bool readBoolean(uint32_t) override;

  uint8_t getSuperblockConfig();
  uint32_t getPageAllocationConfig();

  NANDTiming *getNANDTiming();
  NANDPower *getNANDPower();
};

}  // namespace PAL

}  // namespace SimpleSSD

#endif
