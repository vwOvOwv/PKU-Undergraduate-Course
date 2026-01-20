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

#include "sil/nvme/prp.hh"

#include <cstdlib>
#include <cstring>

#include "simplessd/sim/trace.hh"
#include "simplessd/util/algorithm.hh"

namespace SIL {

namespace NVMe {

PRP::PRP(uint64_t size) : memory(nullptr), capacity(0), ptr1(0), ptr2(0) {
  uint8_t mode = 0;
  uint32_t listCount = 1;
  uint32_t lastEntryCount = 0;
  const uint32_t maxEntryCount = PAGE_SIZE / 8;

  // Calculate size to allocate
  if (size <= PAGE_SIZE) {
    // PRP1 is PRP pointer, PRP2 is not used
    capacity = PAGE_SIZE;
    mode = 1;
  }
  else if (size <= PAGE_SIZE * 2) {
    // Both are PRP pointer
    capacity = PAGE_SIZE * 2;
    mode = 2;
  }
  else {
    // We need to create PRPList
    // PRP1 is PRP pointer (always)
    mode = 3;
    size -= PAGE_SIZE;
    capacity = PAGE_SIZE * 2;

    while (true) {
      if (size > PAGE_SIZE * maxEntryCount) {
        listCount++;

        // Last entry is pointing next PRP list
        size -= PAGE_SIZE * (maxEntryCount - 1);
        capacity += PAGE_SIZE * maxEntryCount;
      }
      else {
        lastEntryCount = (uint32_t)DIVCEIL(size, PAGE_SIZE);
        capacity += PAGE_SIZE * lastEntryCount;

        break;
      }
    }
  }

#ifdef _MSC_VER
  memory = (uint8_t *)_aligned_malloc(capacity, PAGE_SIZE);
#else
  memory = (uint8_t *)aligned_alloc(PAGE_SIZE, capacity);
#endif

  if (memory == nullptr) {
    SimpleSSD::panic("Failed to allocate memory for PRP region");
  }

  memset(memory, 0, capacity);

  if (mode == 1) {
    ptr1 = (uint64_t)memory;
    ptrList.push_back(ptr1);
  }
  else {
    ptr1 = (uint64_t)memory;
    ptr2 = (uint64_t)(memory + PAGE_SIZE);
    ptrList.push_back(ptr1);
    ptrList.push_back(ptr2);

    if (mode == 3) {
      uint64_t *listPtr = (uint64_t *)ptr2;
      uint8_t *page = memory + PAGE_SIZE * 2;
      uint32_t entry = 0;

      while (true) {
        *(listPtr + entry++) = (uint64_t)page;
        ptrList.push_back((uint64_t)page);
        page += PAGE_SIZE;

        if (listCount == 1 && entry == lastEntryCount) {
          *(listPtr + entry) = 0;

          break;
        }
        else if (listCount > 1 && entry == maxEntryCount - 1) {
          *(listPtr + entry) = (uint64_t)page;
          listPtr = (uint64_t *)page;
          page += PAGE_SIZE;
          listCount--;
          entry = 0;
        }
      }
    }
  }
}

PRP::~PRP() {
#ifdef _MSC_VER
  _aligned_free(memory);
#else
  free(memory);
#endif
}

void PRP::getPointer(uint64_t &prp1, uint64_t &prp2) {
  prp1 = ptr1;
  prp2 = ptr2;
}

void PRP::readData(uint64_t offset, uint64_t size, uint8_t *buffer) {
  uint64_t begin = offset / PAGE_SIZE;
  uint64_t end = DIVCEIL(offset + size, PAGE_SIZE);
  uint64_t copied = 0;

  for (uint64_t i = begin; i < end; i++) {
    uint64_t ptr = ptrList[i] + (offset - i * PAGE_SIZE);
    uint64_t len = MIN((i + 1) * PAGE_SIZE - offset, size - copied);

    memcpy(buffer, (uint8_t *)ptr, len);

    copied += len;
  }
}

void PRP::writeData(uint64_t offset, uint64_t size, uint8_t *buffer) {
  uint64_t begin = offset / PAGE_SIZE;
  uint64_t end = DIVCEIL(offset + size, PAGE_SIZE);
  uint64_t copied = 0;

  for (uint64_t i = begin; i < end; i++) {
    uint64_t ptr = ptrList[i] + (offset - i * PAGE_SIZE);
    uint64_t len = MIN((i + 1) * PAGE_SIZE - offset, size - copied);

    memcpy((uint8_t *)ptr, buffer, len);

    copied += len;
  }
}

}  // namespace NVMe

}  // namespace SIL
