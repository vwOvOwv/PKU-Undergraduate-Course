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

#include "hil/sata/config.hh"

#include "util/algorithm.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace HIL {

namespace SATA {

const char NAME_PCIE_GEN[] = "PCIEGeneration";
const char NAME_PCIE_LANE[] = "PCIELane";
const char NAME_AXI_BUS_WIDTH[] = "AXIBusWidth";
const char NAME_AXI_CLOCK[] = "AXIClock";
const char NAME_SATA_MODE[] = "SATAMode";
const char NAME_WORK_INTERVAL[] = "WorkInterval";
const char NAME_MAX_REQUEST_COUNT[] = "MaxRequestCount";
const char NAME_LBA_SIZE[] = "LBASize";
const char NAME_ENABLE_DISK_IMAGE[] = "EnableDiskImage";
const char NAME_STRICT_DISK_SIZE[] = "StrictSizeCheck";
const char NAME_DISK_IMAGE_PATH[] = "DiskImageFile";
const char NAME_USE_COW_DISK[] = "UseCopyOnWriteDisk";

Config::Config() {
  pcieGen = PCIExpress::PCIE_3_X;
  pcieLane = 4;
  axiWidth = ARM::AXI::BUS_128BIT;
  axiClock = 250000000;
  sataMode = SimpleSSD::SATA::SATA_3_0;
  workInterval = 50000;
  maxRequestCount = 4;
  lbaSize = 512;
  enableDiskImage = false;
  strictDiskSize = false;
  diskImagePath = "";
  useCopyOnWriteDisk = false;
}

bool Config::setConfig(const char *name, const char *value) {
  bool ret = true;

  if (MATCH_NAME(NAME_PCIE_GEN)) {
    switch (strtoul(value, nullptr, 10)) {
      case 0:
        pcieGen = PCIExpress::PCIE_1_X;
        break;
      case 1:
        pcieGen = PCIExpress::PCIE_2_X;
        break;
      case 2:
        pcieGen = PCIExpress::PCIE_3_X;
        break;
      default:
        panic("Invalid PCI Express Generation");
        break;
    }
  }
  else if (MATCH_NAME(NAME_PCIE_LANE)) {
    pcieLane = (uint8_t)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_AXI_BUS_WIDTH)) {
    switch (strtoul(value, nullptr, 10)) {
      case 0:
        axiWidth = ARM::AXI::BUS_32BIT;
        break;
      case 1:
        axiWidth = ARM::AXI::BUS_64BIT;
        break;
      case 2:
        axiWidth = ARM::AXI::BUS_128BIT;
        break;
      case 3:
        axiWidth = ARM::AXI::BUS_256BIT;
        break;
      case 4:
        axiWidth = ARM::AXI::BUS_512BIT;
        break;
      case 5:
        axiWidth = ARM::AXI::BUS_1024BIT;
        break;
      default:
        panic("Invalid AXI Stream Bus Width");
        break;
    }
  }
  else if (MATCH_NAME(NAME_AXI_CLOCK)) {
    axiClock = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_SATA_MODE)) {
    switch (strtoul(value, nullptr, 10)) {
      case 0:
        sataMode = SimpleSSD::SATA::SATA_1_0;
        break;
      case 1:
        sataMode = SimpleSSD::SATA::SATA_2_0;
        break;
      case 2:
        sataMode = SimpleSSD::SATA::SATA_3_0;
        break;
      default:
        panic("Invalid SATA Generation");
        break;
    }
  }
  else if (MATCH_NAME(NAME_WORK_INTERVAL)) {
    workInterval = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_MAX_REQUEST_COUNT)) {
    maxRequestCount = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_LBA_SIZE)) {
    lbaSize = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_ENABLE_DISK_IMAGE)) {
    enableDiskImage = convertBool(value);
  }
  else if (MATCH_NAME(NAME_STRICT_DISK_SIZE)) {
    strictDiskSize = convertBool(value);
  }
  else if (MATCH_NAME(NAME_DISK_IMAGE_PATH)) {
    diskImagePath = value;
  }
  else if (MATCH_NAME(NAME_USE_COW_DISK)) {
    useCopyOnWriteDisk = convertBool(value);
  }
  else {
    ret = false;
  }

  return ret;
}

void Config::update() {
  if (popcount(lbaSize) != 1) {
    panic("Invalid LBA size");
  }
  if (maxRequestCount == 0) {
    panic("MaxRequestCount should be larger then 0");
  }
}

int64_t Config::readInt(uint32_t idx) {
  int64_t ret = 0;

  switch (idx) {
    case SATA_PCIE_GEN:
      ret = pcieGen;
      break;
    case SATA_AXI_BUS_WIDTH:
      ret = axiWidth;
      break;
    case SATA_MODE:
      ret = sataMode;
      break;
  }

  return ret;
}

uint64_t Config::readUint(uint32_t idx) {
  uint64_t ret = 0;

  switch (idx) {
    case SATA_PCIE_LANE:
      ret = pcieLane;
      break;
    case SATA_AXI_CLOCK:
      ret = axiClock;
      break;
    case SATA_WORK_INTERVAL:
      ret = workInterval;
      break;
    case SATA_MAX_REQUEST_COUNT:
      ret = maxRequestCount;
      break;
    case SATA_LBA_SIZE:
      ret = lbaSize;
      break;
  }

  return ret;
}

std::string Config::readString(uint32_t idx) {
  std::string ret("");

  switch (idx) {
    case SATA_DISK_IMAGE_PATH:
      ret = diskImagePath;
      break;
  }

  return ret;
}

bool Config::readBoolean(uint32_t idx) {
  bool ret = false;

  switch (idx) {
    case SATA_ENABLE_DISK_IMAGE:
      ret = enableDiskImage;
      break;
    case SATA_STRICT_DISK_SIZE:
      ret = strictDiskSize;
      break;
    case SATA_USE_COW_DISK:
      ret = useCopyOnWriteDisk;
      break;
  }

  return ret;
}

}  // namespace SATA

}  // namespace HIL

}  // namespace SimpleSSD
