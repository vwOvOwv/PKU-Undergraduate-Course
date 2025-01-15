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

#ifndef __ICL_CACHE__
#define __ICL_CACHE__

#include "dram/abstract_dram.hh"
#include "ftl/ftl.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace ICL {

typedef struct _Line {
  uint64_t tag;
  uint64_t lastAccessed;
  uint64_t insertedAt;
  bool dirty;
  bool valid;

  _Line();
  _Line(uint64_t, bool);
} Line;

class AbstractCache : public StatObject {
 protected:
  ConfigReader &conf;
  FTL::FTL *pFTL;
  DRAM::AbstractDRAM *pDRAM;

 public:
  AbstractCache(ConfigReader &, FTL::FTL *, DRAM::AbstractDRAM *);
  virtual ~AbstractCache();

  virtual bool read(Request &, uint64_t &) = 0;
  virtual bool write(Request &, uint64_t &) = 0;

  virtual void flush(LPNRange &, uint64_t &) = 0;
  virtual void trim(LPNRange &, uint64_t &) = 0;
  virtual void format(LPNRange &, uint64_t &) = 0;
};

}  // namespace ICL

}  // namespace SimpleSSD

#endif
