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

#ifndef __UTIL_INTERFACE__
#define __UTIL_INTERFACE__

#include <cinttypes>

namespace SimpleSSD {

namespace PCIExpress {

typedef enum {
  PCIE_1_X,  // PCI Express Gen. 1.x
  PCIE_2_X,  // PCI Express Gen. 2.x
  PCIE_3_X,  // PCI Express Gen. 3.x
  PCIE_NUM
} PCIE_GEN;

uint64_t calculateDelay(PCIE_GEN, uint8_t, uint64_t);

}  // namespace PCIExpress

namespace SATA {

typedef enum {
  SATA_1_0,  // SATA 1.0 (1.5Gbps)
  SATA_2_0,  // SATA 2.0 (3Gbps)
  SATA_3_0,  // SATA 3.0/3.1 (6Gbps)
  SATA_NUM
} SATA_GEN;

uint64_t calculateDelay(SATA_GEN, uint64_t);

}  // namespace SATA

namespace MIPI {

namespace M_PHY {

typedef enum {
  HS_G1,  // High Speed Gear 1
  HS_G2,  // High Speed Gear 2
  HS_G3,  // High Speed Gear 3
  HS_G4,  // High Speed Gear 4
  HS_NUM
} M_PHY_MODE;

uint64_t calculateDelay(M_PHY_MODE, uint8_t, uint64_t);

}  // namespace M_PHY

namespace UniPro {

uint64_t calculateDelay(M_PHY::M_PHY_MODE, uint8_t, uint64_t);

}  // namespace UniPro

}  // namespace MIPI

namespace ARM {

namespace AXI {

typedef enum {
  BUS_32BIT = 4,
  BUS_64BIT = 8,
  BUS_128BIT = 16,
  BUS_256BIT = 32,
  BUS_512BIT = 64,
  BUS_1024BIT = 128,
} BUS_WIDTH;

uint64_t calculateDelay(uint64_t, BUS_WIDTH, uint64_t);

namespace Stream {

uint64_t calculateDelay(uint64_t, BUS_WIDTH, uint64_t);

}  // namespace Stream

}  // namespace AXI

}  // namespace ARM

}  // namespace SimpleSSD

#endif
