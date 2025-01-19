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

#ifndef __DRIVERS_NONE__
#define __DRIVERS_NONE__

#include "bil/interface.hh"
#include "simplessd/hil/hil.hh"

namespace SIL {

namespace None {

class Driver : public BIL::DriverInterface {
 private:
  SimpleSSD::HIL::HIL *pHIL;

  uint64_t totalLogicalPages;
  uint32_t logicalPageSize;

 public:
  Driver(Engine &, SimpleSSD::ConfigReader &);
  ~Driver();

  void init(std::function<void()> &) override;
  void getInfo(uint64_t &, uint32_t &) override;
  void submitIO(BIL::BIO &) override;

  void initStats(std::vector<SimpleSSD::Stats> &) override;
  void getStats(std::vector<double> &) override;
};

}  // namespace None

}  // namespace SIL

#endif
