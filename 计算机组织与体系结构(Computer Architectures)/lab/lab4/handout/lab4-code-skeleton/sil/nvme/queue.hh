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

#ifndef __DRIVERS_NVME_QUEUE__
#define __DRIVERS_NVME_QUEUE__

#include <cinttypes>

namespace SIL {

namespace NVMe {

class Queue {
 private:
  uint8_t *memory;
  uint64_t capacity;

  uint16_t stride;
  uint16_t entries;
  uint16_t head;
  uint16_t tail;

 public:
  Queue(uint16_t, uint16_t);
  ~Queue();

  void getBaseAddress(uint64_t &);
  void getData(uint8_t *, uint16_t);  // Increase head
  void setData(uint8_t *, uint16_t);  // Increase tail
  uint16_t getHead();
  uint16_t getTail();
  void incrHead();
  void incrTail();
  void peekData(uint8_t *, uint16_t);  // Get data without increasing head
};

}  // namespace NVMe

}  // namespace SIL

#endif
