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

#include "sil/nvme/queue.hh"

#include <cstdlib>
#include <cstring>

#include "sil/nvme/prp.hh"
#include "simplessd/sim/trace.hh"

namespace SIL {

namespace NVMe {

Queue::Queue(uint16_t e, uint16_t s)
    : memory(nullptr), stride(s), entries(e), head(0), tail(0) {
  capacity = stride * entries;

#ifdef _MSC_VER
  memory = (uint8_t *)_aligned_malloc(capacity, PAGE_SIZE);
#else
  memory = (uint8_t *)aligned_alloc(PAGE_SIZE, capacity);
#endif

  if (memory == nullptr) {
    SimpleSSD::panic("Failed to allocate memory for PRP region");
  }

  memset(memory, 0, capacity);
}

Queue::~Queue() {
#ifdef _MSC_VER
  _aligned_free(memory);
#else
  free(memory);
#endif
}

void Queue::getBaseAddress(uint64_t &ptr) {
  ptr = (uint64_t)memory;
}

void Queue::peekData(uint8_t *buffer, uint16_t length) {
  memcpy(buffer, memory + head * stride, length);
}

void Queue::getData(uint8_t *buffer, uint16_t length) {
  peekData(buffer, length);

  incrHead();
}

void Queue::setData(uint8_t *buffer, uint16_t length) {
  memcpy(memory + tail * stride, buffer, length);

  incrTail();
}

uint16_t Queue::getHead() {
  return head;
}

uint16_t Queue::getTail() {
  return tail;
}

void Queue::incrHead() {
  head++;

  if (head == entries) {
    head = 0;
  }
}

void Queue::incrTail() {
  tail++;

  if (tail == entries) {
    tail = 0;
  }
}

}  // namespace NVMe

}  // namespace SIL
