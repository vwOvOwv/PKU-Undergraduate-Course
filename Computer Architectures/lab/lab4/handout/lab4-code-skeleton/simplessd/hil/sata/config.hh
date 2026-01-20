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

#ifndef __HIL_SATA_CONFIG__
#define __HIL_SATA_CONFIG__

#include "sim/base_config.hh"
#include "util/interface.hh"

namespace SimpleSSD {

namespace HIL {

namespace SATA {

typedef enum {
  SATA_PCIE_GEN,
  SATA_PCIE_LANE,
  SATA_AXI_BUS_WIDTH,
  SATA_AXI_CLOCK,
  SATA_MODE,
  SATA_WORK_INTERVAL,
  SATA_MAX_REQUEST_COUNT,
  SATA_LBA_SIZE,
  SATA_ENABLE_DISK_IMAGE,
  SATA_STRICT_DISK_SIZE,
  SATA_DISK_IMAGE_PATH,
  SATA_USE_COW_DISK
} SATA_CONFIG;

class Config : public BaseConfig {
 private:
  PCIExpress::PCIE_GEN pcieGen;        //!< Default: PCIE_3_X
  uint8_t pcieLane;                    //!< Default: 4
  ARM::AXI::BUS_WIDTH axiWidth;        //!< Default: BUS_128BIT
  uint64_t axiClock;                   //!< Default: 250000000 (250MHz)
  SimpleSSD::SATA::SATA_GEN sataMode;  //!< Default SATA_3_0
  uint64_t workInterval;               //!< Default: 50000 (50ns)
  uint64_t maxRequestCount;            //!< Default: 4
  uint64_t lbaSize;                    //!< Default: 512
  bool enableDiskImage;                //!< Default: False
  bool strictDiskSize;                 //!< Default: False
  bool useCopyOnWriteDisk;             //!< Default: False
  std::string diskImagePath;           //!< Default: ""

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  int64_t readInt(uint32_t) override;
  uint64_t readUint(uint32_t) override;
  std::string readString(uint32_t) override;
  bool readBoolean(uint32_t) override;
};

}  // namespace SATA

}  // namespace HIL

}  // namespace SimpleSSD

#endif
