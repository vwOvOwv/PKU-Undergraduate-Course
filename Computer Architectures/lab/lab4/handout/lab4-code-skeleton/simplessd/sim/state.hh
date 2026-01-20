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

#ifndef __SIM_STATE__
#define __SIM_STATE__

#include <cinttypes>
#include <vector>

namespace SimpleSSD {

class StateObject {
 protected:
  template <class T>
  static void pushValue(std::vector<uint8_t> &, T);
  template <class T>
  static void popValue(std::vector<uint8_t> &, T);

 public:
  StateObject() {}
  virtual ~StateObject() {}

  virtual void saveState(std::vector<uint8_t> &) {}
  virtual void loadState(std::vector<uint8_t> &) {}
};

}  // namespace SimpleSSD

#endif
