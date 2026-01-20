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

#include "sim/config_reader.hh"

#include "sim/base_config.hh"
#include "sim/trace.hh"

#ifdef _MSC_VER
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

namespace SimpleSSD {

const char SECTION_CPU[] = "cpu";
const char SECTION_DRAM[] = "dram";
const char SECTION_FTL[] = "ftl";
const char SECTION_NVME[] = "nvme";
const char SECTION_ICL[] = "icl";
const char SECTION_PAL[] = "pal";
const char SECTION_SATA[] = "sata";
const char SECTION_UFS[] = "ufs";

bool BaseConfig::convertBool(const char *value) {
  bool ret = false;

  if (strcasecmp(value, "true") == 0 || strtoul(value, nullptr, 10)) {
    ret = true;
  }

  return ret;
}

bool ConfigReader::init(std::string file) {
  if (ini_parse(file.c_str(), parserHandler, this) < 0) {
    return false;
  }

  // Update all
  cpuConfig.update();
  dramConfig.update();
  ftlConfig.update();
  nvmeConfig.update();
  iclConfig.update();
  palConfig.update();
  sataConfig.update();
  ufsConfig.update();

  return true;
}

int64_t ConfigReader::readInt(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_CPU:
      return cpuConfig.readInt(idx);
    case CONFIG_DRAM:
      return dramConfig.readInt(idx);
    case CONFIG_FTL:
      return ftlConfig.readInt(idx);
    case CONFIG_NVME:
      return nvmeConfig.readInt(idx);
    case CONFIG_SATA:
      return sataConfig.readInt(idx);
    case CONFIG_UFS:
      return ufsConfig.readInt(idx);
    case CONFIG_ICL:
      return iclConfig.readInt(idx);
    case CONFIG_PAL:
      return palConfig.readInt(idx);
    default:
      return 0;
  }
}

uint64_t ConfigReader::readUint(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_CPU:
      return cpuConfig.readUint(idx);
    case CONFIG_DRAM:
      return dramConfig.readUint(idx);
    case CONFIG_FTL:
      return ftlConfig.readUint(idx);
    case CONFIG_NVME:
      return nvmeConfig.readUint(idx);
    case CONFIG_SATA:
      return sataConfig.readUint(idx);
    case CONFIG_UFS:
      return ufsConfig.readUint(idx);
    case CONFIG_ICL:
      return iclConfig.readUint(idx);
    case CONFIG_PAL:
      return palConfig.readUint(idx);
    default:
      return 0;
  }
}

float ConfigReader::readFloat(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_CPU:
      return cpuConfig.readFloat(idx);
    case CONFIG_DRAM:
      return dramConfig.readFloat(idx);
    case CONFIG_FTL:
      return ftlConfig.readFloat(idx);
    case CONFIG_NVME:
      return nvmeConfig.readFloat(idx);
    case CONFIG_SATA:
      return sataConfig.readFloat(idx);
    case CONFIG_UFS:
      return ufsConfig.readFloat(idx);
    case CONFIG_ICL:
      return iclConfig.readFloat(idx);
    case CONFIG_PAL:
      return palConfig.readFloat(idx);
    default:
      return 0.f;
  }
}

std::string ConfigReader::readString(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_CPU:
      return cpuConfig.readString(idx);
    case CONFIG_DRAM:
      return dramConfig.readString(idx);
    case CONFIG_FTL:
      return ftlConfig.readString(idx);
    case CONFIG_NVME:
      return nvmeConfig.readString(idx);
    case CONFIG_SATA:
      return sataConfig.readString(idx);
    case CONFIG_UFS:
      return ufsConfig.readString(idx);
    case CONFIG_ICL:
      return iclConfig.readString(idx);
    case CONFIG_PAL:
      return palConfig.readString(idx);
    default:
      return std::string();
  }
}

bool ConfigReader::readBoolean(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_CPU:
      return cpuConfig.readBoolean(idx);
    case CONFIG_DRAM:
      return dramConfig.readBoolean(idx);
    case CONFIG_FTL:
      return ftlConfig.readBoolean(idx);
    case CONFIG_NVME:
      return nvmeConfig.readBoolean(idx);
    case CONFIG_SATA:
      return sataConfig.readBoolean(idx);
    case CONFIG_UFS:
      return ufsConfig.readBoolean(idx);
    case CONFIG_ICL:
      return iclConfig.readBoolean(idx);
    case CONFIG_PAL:
      return palConfig.readBoolean(idx);
    default:
      return false;
  }
}

int ConfigReader::parserHandler(void *context, const char *section,
                                const char *name, const char *value) {
  ConfigReader *pThis = (ConfigReader *)context;
  bool handled = false;

  if (MATCH_SECTION(SECTION_CPU)) {
    handled = pThis->cpuConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_DRAM)) {
    handled = pThis->dramConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_FTL)) {
    handled = pThis->ftlConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_NVME)) {
    handled = pThis->nvmeConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_SATA)) {
    handled = pThis->sataConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_UFS)) {
    handled = pThis->ufsConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_ICL)) {
    handled = pThis->iclConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_PAL)) {
    handled = pThis->palConfig.setConfig(name, value);
  }

  if (!handled) {
    warn("Config [%s] %s = %s not handled", section, name, value);
  }

  return 1;
}

DRAM::Config::DRAMStructure *ConfigReader::getDRAMStructure() {
  return dramConfig.getDRAMStructure();
}

DRAM::Config::DRAMTiming *ConfigReader::getDRAMTiming() {
  return dramConfig.getDRAMTiming();
}

DRAM::Config::DRAMPower *ConfigReader::getDRAMPower() {
  return dramConfig.getDRAMPower();
}

uint8_t ConfigReader::getSuperblockConfig() {
  return palConfig.getSuperblockConfig();
}

uint32_t ConfigReader::getPageAllocationConfig() {
  return palConfig.getPageAllocationConfig();
}

PAL::Config::NANDTiming *ConfigReader::getNANDTiming() {
  return palConfig.getNANDTiming();
}

PAL::Config::NANDPower *ConfigReader::getNANDPower() {
  return palConfig.getNANDPower();
}

}  // namespace SimpleSSD
