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

#include "util/bitset.hh"

#include <cstdlib>
#include <cstring>

#include "util/algorithm.hh"

namespace SimpleSSD {

Bitset::Bitset() : data(nullptr), dataSize(0), allocSize(0) {}

Bitset::Bitset(uint32_t size) : Bitset() {
  if (size > 0) {
    dataSize = size;
    allocSize = DIVCEIL(dataSize, 8);
    data = (uint8_t *)calloc(allocSize, 1);
  }
}

Bitset::Bitset(const Bitset &rhs) : Bitset(rhs.dataSize) {
  if (rhs.data) {
    memcpy(data, rhs.data, allocSize);
  }
}

Bitset::Bitset(Bitset &&rhs) noexcept {
  dataSize = std::move(rhs.dataSize);
  allocSize = std::move(rhs.allocSize);
  data = std::move(rhs.data);

  rhs.dataSize = 0;
  rhs.allocSize = 0;
  rhs.data = nullptr;
}

Bitset::~Bitset() {
  free(data);

  data = nullptr;
  dataSize = 0;
  allocSize = 0;
}

bool Bitset::test(uint32_t idx) noexcept {
  return data[idx / 8] & (0x01 << (idx % 8));
}

bool Bitset::all() noexcept {
  uint8_t ret = 0xFF;
  uint8_t mask = 0xFF << (dataSize + 8 - allocSize * 8);

  for (uint32_t i = 0; i < allocSize - 1; i++) {
    ret &= data[i];
  }

  ret &= data[allocSize - 1] | mask;

  return ret == 0xFF;
}

bool Bitset::any() noexcept {
  return !none();
}

bool Bitset::none() noexcept {
  uint8_t ret = 0x00;

  for (uint32_t i = 0; i < allocSize; i++) {
    ret |= data[i];
  }

  return ret == 0x00;
}

uint32_t Bitset::count() noexcept {
  uint32_t count = 0;

  for (uint32_t i = 0; i < allocSize; i++) {
    count += popcount(data[i]);
  }

  return count;
}

uint32_t Bitset::size() noexcept {
  return dataSize;
}

void Bitset::set() noexcept {
  uint8_t mask = 0xFF >> (allocSize * 8 - dataSize);

  for (uint32_t i = 0; i < allocSize - 1; i++) {
    data[i] = 0xFF;
  }

  data[allocSize - 1] = mask;
}

void Bitset::set(uint32_t idx, bool value) noexcept {
  data[idx / 8] &= ~(0x01 << (idx % 8));

  if (value) {
    data[idx / 8] = data[idx / 8] | (0x01 << (idx % 8));
  }
}

void Bitset::reset() noexcept {
  for (uint32_t i = 0; i < allocSize; i++) {
    data[i] = 0x00;
  }
}

void Bitset::reset(uint32_t idx) noexcept {
  data[idx / 8] &= ~(0x01 << (idx % 8));
}

void Bitset::flip() noexcept {
  uint8_t mask = 0xFF >> (allocSize * 8 - dataSize);

  for (uint32_t i = 0; i < allocSize; i++) {
    data[i] = ~data[i];
  }

  data[allocSize - 1] &= mask;
}

void Bitset::flip(uint32_t idx) noexcept {
  data[idx / 8] = (~data[idx / 8] & (0x01 << (idx % 8))) |
                  (data[idx / 8] & ~(0x01 << (idx % 8)));
}

bool Bitset::operator[](uint32_t idx) noexcept {
  return test(idx);
}

Bitset &Bitset::operator&=(const Bitset &rhs) {
  if (dataSize != rhs.dataSize) {
    panic("Size does not match");
  }

  for (uint32_t i = 0; i < allocSize; i++) {
    data[i] &= rhs.data[i];
  }

  return *this;
}

Bitset &Bitset::operator|=(const Bitset &rhs) {
  if (dataSize != rhs.dataSize) {
    panic("Size does not match");
  }

  for (uint32_t i = 0; i < allocSize; i++) {
    data[i] |= rhs.data[i];
  }

  return *this;
}

Bitset &Bitset::operator^=(const Bitset &rhs) {
  if (dataSize != rhs.dataSize) {
    panic("Size does not match");
  }

  for (uint32_t i = 0; i < allocSize; i++) {
    data[i] ^= rhs.data[i];
  }

  return *this;
}

Bitset &Bitset::operator=(const Bitset &rhs) {
  if (this != &rhs) {
    free(data);
    data = nullptr;

    *this = Bitset(rhs);
  }

  return *this;
}

Bitset &Bitset::operator=(Bitset &&rhs) noexcept {
  if (this != &rhs) {
    free(data);

    dataSize = std::move(rhs.dataSize);
    allocSize = std::move(rhs.allocSize);
    data = std::move(rhs.data);

    rhs.dataSize = 0;
    rhs.allocSize = 0;
    rhs.data = nullptr;
  }

  return *this;
}

Bitset Bitset::operator~() const {
  Bitset ret(*this);

  ret.flip();

  return ret;
}

}  // namespace SimpleSSD
