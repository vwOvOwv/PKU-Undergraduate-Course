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

#pragma once

#ifndef __SIM_CONFIG_READER__
#define __SIM_CONFIG_READER__

#include <cinttypes>
#include <string>

#include "cpu/config.hh"
#include "dram/config.hh"
#include "ftl/config.hh"
#include "hil/nvme/config.hh"
#include "hil/sata/config.hh"
#include "hil/ufs/config.hh"
#include "icl/config.hh"
#include "lib/inih/ini.h"
#include "pal/config.hh"

namespace SimpleSSD {

typedef enum {
  CONFIG_CPU,
  CONFIG_DRAM,
  CONFIG_FTL,
  CONFIG_NVME,
  CONFIG_SATA,
  CONFIG_UFS,
  CONFIG_ICL,
  CONFIG_PAL,
} CONFIG_SECTION;

class ConfigReader {
 private:
  CPU::Config cpuConfig;
  DRAM::Config dramConfig;
  FTL::Config ftlConfig;
  HIL::NVMe::Config nvmeConfig;
  HIL::SATA::Config sataConfig;
  HIL::UFS::Config ufsConfig;
  ICL::Config iclConfig;
  PAL::Config palConfig;

  static int parserHandler(void *, const char *, const char *, const char *);

 public:
  bool init(std::string);

  int64_t readInt(CONFIG_SECTION, uint32_t);
  uint64_t readUint(CONFIG_SECTION, uint32_t);
  float readFloat(CONFIG_SECTION, uint32_t);
  std::string readString(CONFIG_SECTION, uint32_t);
  bool readBoolean(CONFIG_SECTION, uint32_t);

  // DRAM::Config
  DRAM::Config::DRAMStructure *getDRAMStructure();
  DRAM::Config::DRAMTiming *getDRAMTiming();
  DRAM::Config::DRAMPower *getDRAMPower();

  // PAL::Config
  uint8_t getSuperblockConfig();
  uint32_t getPageAllocationConfig();

  PAL::Config::NANDTiming *getNANDTiming();
  PAL::Config::NANDPower *getNANDPower();
};

}  // namespace SimpleSSD

#endif
