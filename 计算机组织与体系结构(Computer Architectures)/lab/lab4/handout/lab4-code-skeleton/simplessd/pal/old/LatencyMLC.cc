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

#include "LatencyMLC.h"

LatencyMLC::LatencyMLC(SimpleSSD::PAL::Config::NANDTiming t,
                       SimpleSSD::PAL::Config::NANDPower p)
    : Latency(t, p) {}

LatencyMLC::~LatencyMLC() {}

inline uint8_t LatencyMLC::GetPageType(uint32_t AddrPage) {
  return AddrPage % 2;
}

uint64_t LatencyMLC::GetLatency(uint32_t AddrPage, uint8_t Oper, uint8_t Busy) {
  SimpleSSD::PAL::Config::PAGETiming *pTiming = nullptr;

  switch (Busy) {
    case BUSY_DMA0:
      if (Oper == OPER_READ) {
        return timing.dma0.read;
      }
      else if (Oper == OPER_WRITE) {
        return timing.dma0.write;
      }
      else {
        return timing.dma0.erase;
      }

      break;
    case BUSY_DMA1:
      if (Oper == OPER_READ) {
        return timing.dma1.read;
      }
      else if (Oper == OPER_WRITE) {
        return timing.dma1.write;
      }
      else {
        return timing.dma1.erase;
      }

      break;
    case BUSY_MEM: {
      if (Oper == OPER_ERASE) {
        return timing.erase;
      }

      if (GetPageType(AddrPage) == PAGE_LSB) {
        pTiming = &timing.lsb;
      }
      else {
        pTiming = &timing.msb;
      }

      if (Oper == OPER_READ) {
        return pTiming->read;
      }
      else {
        return pTiming->write;
      }

      break;
    }
    default:
      break;
  }

  return 10;
}
