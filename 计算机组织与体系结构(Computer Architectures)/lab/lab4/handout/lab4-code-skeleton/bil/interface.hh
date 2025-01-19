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

#ifndef __BIL_DRIVER_INTERFACE__
#define __BIL_DRIVER_INTERFACE__

#include <functional>

#include "bil/entry.hh"
#include "simplessd/sim/statistics.hh"

namespace BIL {

class DriverInterface {
 protected:
  Engine &engine;

  std::function<void()> beginFunction;

 public:
  DriverInterface(Engine &e) : engine(e) {}
  virtual ~DriverInterface() {}

  virtual void init(std::function<void()> &) = 0;
  virtual void getInfo(uint64_t &, uint32_t &) = 0;
  virtual void submitIO(BIO &) = 0;

  virtual void initStats(std::vector<SimpleSSD::Stats> &) = 0;
  virtual void getStats(std::vector<double> &) = 0;
};

}  // namespace BIL

#endif
