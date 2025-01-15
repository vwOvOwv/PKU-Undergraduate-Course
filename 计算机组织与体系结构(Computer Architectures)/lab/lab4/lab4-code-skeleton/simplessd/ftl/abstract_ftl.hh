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

#ifndef __FTL_ABSTRACT_FTL__
#define __FTL_ABSTRACT_FTL__

#include <cinttypes>

#include "ftl/ftl.hh"

namespace SimpleSSD {

namespace FTL {

typedef struct _Status {
  uint64_t totalLogicalPages;
  uint64_t mappedLogicalPages;
  uint64_t freePhysicalBlocks;
} Status;

class AbstractFTL : public StatObject {
 protected:
  Parameter &param;
  PAL::PAL *pPAL;
  DRAM::AbstractDRAM *pDRAM;
  Status status;

 public:
  AbstractFTL(Parameter &p, PAL::PAL *l, DRAM::AbstractDRAM *d)
      : param(p), pPAL(l), pDRAM(d) {}
  virtual ~AbstractFTL() {}

  virtual bool initialize() = 0;

  virtual void read(Request &, uint64_t &) = 0;
  virtual void write(Request &, uint64_t &) = 0;
  virtual void trim(Request &, uint64_t &) = 0;

  virtual void format(LPNRange &, uint64_t &) = 0;

  virtual Status *getStatus(uint64_t, uint64_t) = 0;
};

}  // namespace FTL

}  // namespace SimpleSSD

#endif
