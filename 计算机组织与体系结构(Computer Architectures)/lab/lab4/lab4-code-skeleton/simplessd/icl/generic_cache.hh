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

#ifndef __ICL_GENERIC_CACHE__
#define __ICL_GENERIC_CACHE__

#include <functional>
#include <random>
#include <vector>

#include "icl/abstract_cache.hh"

namespace SimpleSSD {

namespace ICL {

class GenericCache : public AbstractCache {
 private:
  const uint32_t superPageSize;
  const uint32_t parallelIO;
  uint32_t lineCountInSuperPage;
  uint32_t lineCountInMaxIO;
  uint32_t lineSize;
  uint32_t setSize;
  uint32_t waySize;

  const uint32_t prefetchIOCount;
  const float prefetchIORatio;

  const bool useReadCaching;
  const bool useWriteCaching;
  const bool useReadPrefetch;

  bool bSuperPage;

  struct SequentialDetect {
    bool enabled;
    Request lastRequest;
    uint32_t hitCounter;
    uint32_t accessCounter;

    SequentialDetect() : enabled(false), hitCounter(0), accessCounter(0) {
      lastRequest.reqID = 1;
    }
  } readDetect;

  uint64_t prefetchTrigger;
  uint64_t lastPrefetched;

  PREFETCH_MODE prefetchMode;
  EVICT_MODE evictMode;
  EVICT_POLICY policy;
  std::function<uint32_t(uint32_t, uint64_t &)> evictFunction;
  std::function<Line *(Line *, Line *)> compareFunction;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<uint32_t> dist;

  std::vector<Line *> cacheData;
  std::vector<Line **> evictData;

  uint64_t getCacheLatency();

  uint32_t calcSetIndex(uint64_t);
  void calcIOPosition(uint64_t, uint32_t &, uint32_t &);

  uint32_t getEmptyWay(uint32_t, uint64_t &);
  uint32_t getValidWay(uint64_t, uint64_t &);
  void checkSequential(Request &, SequentialDetect &);

  void evictCache(uint64_t, bool = true);

  // Stats
  struct {
    uint64_t request[2];
    uint64_t cache[2];
  } stat;

 public:
  GenericCache(ConfigReader &, FTL::FTL *, DRAM::AbstractDRAM *);
  ~GenericCache();

  bool read(Request &, uint64_t &) override;
  bool write(Request &, uint64_t &) override;

  void flush(LPNRange &, uint64_t &) override;
  void trim(LPNRange &, uint64_t &) override;
  void format(LPNRange &, uint64_t &) override;

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace ICL

}  // namespace SimpleSSD

#endif
