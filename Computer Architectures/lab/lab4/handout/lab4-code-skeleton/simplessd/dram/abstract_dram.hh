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

#ifndef __DRAM_ABSTRACT_DRAM__
#define __DRAM_ABSTRACT_DRAM__

#include <cinttypes>

#include "libdrampower/LibDRAMPower.h"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace DRAM {

typedef enum : uint8_t {
  ACTIVE,                //!< Row active
  IDLE,                  //!< Precharged
  POWER_DOWN_PRECHARGE,  //!< Precharge power down
  POWER_DOWN_ACTIVE,     //!< Active power down
  SELF_REFRESH,          //!< Self refresh
} DRAMState;

class AbstractDRAM : public StatObject {
 protected:
  ConfigReader &conf;

  Config::DRAMStructure *pStructure;
  Config::DRAMTiming *pTiming;
  Config::DRAMPower *pPower;

  Data::MemorySpecification spec;
  libDRAMPower *dramPower;

  void convertMemspec();

  double totalEnergy;  // Unit: pJ
  double totalPower;   // Unit: mW

 public:
  AbstractDRAM(ConfigReader &);
  virtual ~AbstractDRAM();

  virtual void read(void *, uint64_t, uint64_t &) = 0;
  virtual void write(void *, uint64_t, uint64_t &) = 0;

  // TEMP: Should be removed on v2.2
  virtual void setScheduling(bool) {}
  virtual bool isScheduling() { return true; }

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace DRAM

}  // namespace SimpleSSD

#endif
