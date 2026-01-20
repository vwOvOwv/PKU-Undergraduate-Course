/**
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

#ifndef __Latency_h__
#define __Latency_h__

#include "util/old/SimpleSSD_types.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include "pal/config.hh"

/*==============================
    Latency
==============================*/
class Latency {
 protected:
  SimpleSSD::PAL::Config::NANDTiming timing;
  SimpleSSD::PAL::Config::NANDPower power;

 public:
  Latency(SimpleSSD::PAL::Config::NANDTiming,
          SimpleSSD::PAL::Config::NANDPower);
  virtual ~Latency();

  // Get Latency for PageAddress(L/C/MSBpage), Operation(RWE),
  // BusyFor(Ch.DMA/Mem.Work)
  virtual uint64_t GetLatency(uint32_t, uint8_t, uint8_t) { return 0; };
  virtual inline uint8_t GetPageType(uint32_t) { return PAGE_NUM; };

  // Setup DMA speed and pagesize
  virtual uint64_t GetPower(uint8_t, uint8_t);
};

#endif  //__Latency_h__
