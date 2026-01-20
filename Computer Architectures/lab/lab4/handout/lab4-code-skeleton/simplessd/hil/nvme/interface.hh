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

#ifndef __HIL_NVME_INTERFACE__
#define __HIL_NVME_INTERFACE__

#include <cinttypes>

#include "sim/dma_interface.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

class Controller;

class Interface : public SimpleSSD::DMAInterface {
 protected:
  Controller *pController;

 public:
  virtual void updateInterrupt(uint16_t, bool) = 0;
  virtual void getVendorID(uint16_t &, uint16_t &) = 0;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD

#endif
