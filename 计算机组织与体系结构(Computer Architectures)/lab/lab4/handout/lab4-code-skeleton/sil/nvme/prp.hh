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

#ifndef __DRIVERS_NVME_PRP__
#define __DRIVERS_NVME_PRP__

#include <cinttypes>
#include <vector>

#define PAGE_SIZE 4096

namespace SIL {

namespace NVMe {

class PRP {
 private:
  uint8_t *memory;
  uint64_t capacity;

  uint64_t ptr1;
  uint64_t ptr2;

  std::vector<uint64_t> ptrList;  // For faster access

 public:
  PRP(uint64_t);
  ~PRP();

  void getPointer(uint64_t &, uint64_t &);
  void readData(uint64_t, uint64_t, uint8_t *);
  void writeData(uint64_t, uint64_t, uint8_t *);
};

}  // namespace NVMe

}  // namespace SIL

#endif
