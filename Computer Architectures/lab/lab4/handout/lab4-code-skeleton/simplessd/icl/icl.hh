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

#ifndef __ICL_ICL__
#define __ICL_ICL__

#include "dram/abstract_dram.hh"
#include "ftl/ftl.hh"
#include "icl/abstract_cache.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace ICL {

class ICL : public StatObject {
 private:
  FTL::FTL *pFTL;
  DRAM::AbstractDRAM *pDRAM;

  ConfigReader &conf;
  AbstractCache *pCache;

  uint64_t totalLogicalPages;
  uint32_t logicalPageSize;

 public:
  ICL(ConfigReader &);
  ~ICL();

  void read(Request &, uint64_t &);
  void write(Request &, uint64_t &);

  void flush(LPNRange &, uint64_t &);
  void trim(LPNRange &, uint64_t &);
  void format(LPNRange &, uint64_t &);

  void getLPNInfo(uint64_t &, uint32_t &);
  uint64_t getUsedPageCount(uint64_t, uint64_t);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace ICL

}  // namespace SimpleSSD

#endif
