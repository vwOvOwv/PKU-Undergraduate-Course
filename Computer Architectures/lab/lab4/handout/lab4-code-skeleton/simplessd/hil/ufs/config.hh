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

#ifndef __HIL_UFS_CONFIG__
#define __HIL_UFS_CONFIG__

#include "sim/base_config.hh"
#include "util/interface.hh"

namespace SimpleSSD {

namespace HIL {

namespace UFS {

typedef enum {
  UFS_WORK_INTERVAL,
  UFS_MAX_REQUEST_COUNT,
  UFS_LBA_SIZE,
  UFS_ENABLE_DISK_IMAGE,
  UFS_STRICT_DISK_SIZE,
  UFS_DISK_IMAGE_PATH,
  UFS_USE_COW_DISK,
  UFS_HOST_AXI_BUS_WIDTH,
  UFS_HOST_AXI_CLOCK,
  UFS_MPHY_MODE,
  UFS_MPHY_LANE,
} UFS_CONFIG;

class Config : public BaseConfig {
 private:
  ARM::AXI::BUS_WIDTH axiWidth;      //!< Default: BUS_64BIT
  uint64_t axiClock;                 //!< Default: 300MHz
  uint64_t workInterval;             //!< Default: 50000 (50ns)
  uint64_t maxRequestCount;          //!< Default: 4
  uint64_t lbaSize;                  //!< Default: 512
  bool enableDiskImage;              //!< Default: False
  bool strictDiskSize;               //!< Default: False
  bool useCopyOnWriteDisk;           //!< Default: False
  std::string diskImagePath;         //!< Default: ""
  MIPI::M_PHY::M_PHY_MODE mphyMode;  //!< Default: HS_G3
  uint8_t mphyLane;                  //!< Default: 2

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  int64_t readInt(uint32_t) override;
  uint64_t readUint(uint32_t) override;
  std::string readString(uint32_t) override;
  bool readBoolean(uint32_t) override;
};

}  // namespace UFS

}  // namespace HIL

}  // namespace SimpleSSD

#endif
