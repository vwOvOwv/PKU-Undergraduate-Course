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

#ifndef __ICL_CONFIG__
#define __ICL_CONFIG__

#include "sim/base_config.hh"

namespace SimpleSSD {

namespace ICL {

typedef enum {
  /* Cache config */
  ICL_USE_READ_CACHE,
  ICL_USE_WRITE_CACHE,
  ICL_USE_READ_PREFETCH,
  ICL_PREFETCH_COUNT,
  ICL_PREFETCH_RATIO,
  ICL_PREFETCH_GRANULARITY,
  ICL_EVICT_POLICY,
  ICL_EVICT_GRANULARITY,
  ICL_CACHE_SIZE,
  ICL_WAY_SIZE,
  ICL_CACHE_LATENCY,
} ICL_CONFIG;

typedef enum {
  POLICY_RANDOM,               //!< Select way in random
  POLICY_FIFO,                 //!< Select way that lastly inserted
  POLICY_LEAST_RECENTLY_USED,  //!< Select way that least recently used
} EVICT_POLICY;

typedef enum {
  MODE_SUPERPAGE,  //!< Read one page from one super block (super page)
  MODE_ALL,        //!< Read one page from all NAND flashes
} PREFETCH_MODE;

typedef PREFETCH_MODE EVICT_MODE;

class Config : public BaseConfig {
 private:
  bool readCaching;            //!< Default: false
  bool writeCaching;           //!< Default: true
  bool readPrefetch;           //!< Default: false
  EVICT_POLICY evictPolicy;    //!< Default: POLICY_LEAST_RECENTLY_USED
  uint64_t cacheWaySize;       //!< Default: 1
  uint64_t cacheSize;          //!< Default: 33554432 (32MiB)
  uint64_t prefetchCount;      //!< Default: 1
  float prefetchRatio;         //!< Default: 0.5
  PREFETCH_MODE prefetchMode;  //!< Default: MODE_ALL
  EVICT_MODE evictMode;        //!< Default: MODE_ALL
  uint64_t cacheLatency;       //!< Default:

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  int64_t readInt(uint32_t) override;
  uint64_t readUint(uint32_t) override;
  float readFloat(uint32_t) override;
  bool readBoolean(uint32_t) override;
};

}  // namespace ICL

}  // namespace SimpleSSD

#endif
