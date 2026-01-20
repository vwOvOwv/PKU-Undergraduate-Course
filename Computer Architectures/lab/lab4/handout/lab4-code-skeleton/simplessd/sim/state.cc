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

#include "sim/state.hh"

#include <cstring>

#include "sim/trace.hh"

namespace SimpleSSD {

template <class T>
void StateObject::pushValue(std::vector<uint8_t> &data, T value) {
  uint32_t valueSize = sizeof(value);
  uint8_t *src = (uint8_t *)&value;
  uint8_t *dst = nullptr;

  data.resize(data.size() + valueSize);

  dst = data.data() + data.size() - valueSize;
  memcpy(dst, src, valueSize);
}

template <class T>
void StateObject::popValue(std::vector<uint8_t> &data, T value) {
  uint32_t valueSize = sizeof(value);
  uint8_t *dst = (uint8_t *)&value;
  uint8_t *src = nullptr;

  if (data.size() < valueSize) {
    panic("Invalid data stream size");
  }

  src = data.data() + data.size() - valueSize;
  memcpy(dst, src, valueSize);

  data.erase(data.begin() + data.size() - valueSize, data.end());
}

}  // namespace SimpleSSD
