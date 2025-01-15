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

#ifndef __PAL_ABSTRACT_PAL__
#define __PAL_ABSTRACT_PAL__

#include <cinttypes>

#include "pal/pal.hh"

namespace SimpleSSD {

namespace PAL {

class AbstractPAL : public StatObject {
 protected:
  Parameter &param;
  ConfigReader &conf;

 public:
  AbstractPAL(Parameter &p, ConfigReader &c) : param(p), conf(c) {}
  virtual ~AbstractPAL() {}

  virtual void read(Request &, uint64_t &) = 0;
  virtual void write(Request &, uint64_t &) = 0;
  virtual void erase(Request &, uint64_t &) = 0;
};

}  // namespace PAL

}  // namespace SimpleSSD

#endif
