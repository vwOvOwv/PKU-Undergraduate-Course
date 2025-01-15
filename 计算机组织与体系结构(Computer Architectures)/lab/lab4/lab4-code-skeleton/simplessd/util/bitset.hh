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

#ifndef __UTIL_BITSET__
#define __UTIL_BITSET__

#include <cinttypes>
#include <vector>

#include "sim/trace.hh"

namespace SimpleSSD {

// TODO: use SIMD operation if possible
class Bitset {
 private:
  uint8_t *data;
  uint32_t dataSize;
  uint32_t allocSize;

 public:
  Bitset();
  Bitset(uint32_t);
  Bitset(const Bitset &);
  Bitset(Bitset &&) noexcept;
  ~Bitset();

  bool test(uint32_t) noexcept;
  bool all() noexcept;
  bool any() noexcept;
  bool none() noexcept;
  uint32_t count() noexcept;
  uint32_t size() noexcept;
  void set() noexcept;
  void set(uint32_t, bool = true) noexcept;
  void reset() noexcept;
  void reset(uint32_t) noexcept;
  void flip() noexcept;
  void flip(uint32_t) noexcept;

  bool operator[](uint32_t) noexcept;
  Bitset &operator&=(const Bitset &);
  Bitset &operator|=(const Bitset &);
  Bitset &operator^=(const Bitset &);
  Bitset &operator=(const Bitset &);
  Bitset &operator=(Bitset &&) noexcept;
  Bitset operator~() const;

  friend Bitset &operator&(Bitset lhs, const Bitset &rhs) { return lhs &= rhs; }
  friend Bitset &operator|(Bitset lhs, const Bitset &rhs) { return lhs |= rhs; }
  friend Bitset &operator^(Bitset lhs, const Bitset &rhs) { return lhs ^= rhs; }
  friend bool operator==(const Bitset &lhs, const Bitset &rhs) {
    bool ret = true;

    if (lhs.dataSize != rhs.dataSize) {
      panic("Size does not match");
    }

    for (uint32_t i = 0; i < lhs.allocSize; i++) {
      if (lhs.data[i] != rhs.data[i]) {
        ret = false;

        break;
      }
    }

    return ret;
  }

  friend bool operator!=(const Bitset &lhs, const Bitset &rhs) {
    return !operator==(lhs, rhs);
  }
};

}  // namespace SimpleSSD

#endif
