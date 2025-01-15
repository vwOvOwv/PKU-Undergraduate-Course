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

#include "Latency.h"

/*==============================
    Latency
==============================*/

/*
    Latency class is inherited to:
    - TLC
    - MLC
    - SLC
*/

Latency::Latency(SimpleSSD::PAL::Config::NANDTiming t,
                 SimpleSSD::PAL::Config::NANDPower p)
    : timing(t), power(p) {}

Latency::~Latency() {}

// Unit conversion: mV * uA = nW
uint64_t Latency::GetPower(uint8_t Oper, uint8_t Busy) {
  switch (Busy) {
    case BUSY_DMA0:
    case BUSY_DMA1:
      return power.voltage * power.current.busIdle;
    case BUSY_MEM:
      if (Oper == OPER_READ) {
        return power.voltage * power.current.read;
      }
      else if (Oper == OPER_WRITE) {
        return power.voltage * power.current.program;
      }
      else {
        return power.voltage * power.current.erase;
      }

      break;
    default:
      return power.voltage * power.current.standby;
  }
}
