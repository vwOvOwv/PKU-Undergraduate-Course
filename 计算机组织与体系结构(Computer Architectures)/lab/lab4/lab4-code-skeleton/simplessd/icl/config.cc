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

#include "icl/config.hh"

#include "util/algorithm.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace ICL {

const char NAME_USE_READ_CACHE[] = "EnableReadCache";
const char NAME_USE_WRITE_CACHE[] = "EnableWriteCache";
const char NAME_USE_READ_PREFETCH[] = "EnableReadPrefetch";
const char NAME_EVICT_POLICY[] = "EvictPolicy";
const char NAME_EVICT_MODE[] = "EvictMode";
const char NAME_CACHE_SIZE[] = "CacheSize";
const char NAME_WAY_SIZE[] = "CacheWaySize";
const char NAME_PREFETCH_COUNT[] = "ReadPrefetchCount";
const char NAME_PREFETCH_RATIO[] = "ReadPrefetchRatio";
const char NAME_PREFETCH_MODE[] = "ReadPrefetchMode";
const char NAME_CACHE_LATENCY[] = "CacheLatency";

Config::Config() {
  readCaching = false;
  writeCaching = true;
  readPrefetch = false;
  evictPolicy = POLICY_LEAST_RECENTLY_USED;
  cacheSize = 33554432;
  cacheWaySize = 1;
  prefetchCount = 1;
  prefetchRatio = 0.5;
  prefetchMode = MODE_ALL;
  evictMode = MODE_ALL;
  cacheLatency = 10;
}

bool Config::setConfig(const char *name, const char *value) {
  bool ret = true;

  if (MATCH_NAME(NAME_USE_READ_CACHE)) {
    readCaching = convertBool(value);
  }
  else if (MATCH_NAME(NAME_USE_WRITE_CACHE)) {
    writeCaching = convertBool(value);
  }
  else if (MATCH_NAME(NAME_USE_READ_PREFETCH)) {
    readPrefetch = convertBool(value);
  }
  else if (MATCH_NAME(NAME_PREFETCH_COUNT)) {
    prefetchCount = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_PREFETCH_RATIO)) {
    prefetchRatio = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_EVICT_POLICY)) {
    evictPolicy = (EVICT_POLICY)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_EVICT_MODE)) {
    evictMode = (EVICT_MODE)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_CACHE_SIZE)) {
    cacheSize = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_WAY_SIZE)) {
    cacheWaySize = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_PREFETCH_MODE)) {
    prefetchMode = (PREFETCH_MODE)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_CACHE_LATENCY)) {
    cacheLatency = strtoul(value, nullptr, 10);
  }
  else {
    ret = false;
  }

  return ret;
}

void Config::update() {
  if (prefetchCount == 0) {
    panic("Invalid ReadPrefetchCount");
  }
  if (prefetchRatio <= 0.f) {
    panic("Invalid ReadPrefetchRatio");
  }
}

int64_t Config::readInt(uint32_t idx) {
  int64_t ret = 0;

  switch (idx) {
    case ICL_EVICT_POLICY:
      ret = evictPolicy;
      break;
    case ICL_PREFETCH_GRANULARITY:
      ret = prefetchMode;
      break;
    case ICL_EVICT_GRANULARITY:
      ret = evictMode;
      break;
  }

  return ret;
}

uint64_t Config::readUint(uint32_t idx) {
  uint64_t ret = 0;

  switch (idx) {
    case ICL_CACHE_SIZE:
      ret = cacheSize;
      break;
    case ICL_WAY_SIZE:
      ret = cacheWaySize;
      break;
    case ICL_PREFETCH_COUNT:
      ret = prefetchCount;
      break;
    case ICL_CACHE_LATENCY:
      ret = cacheLatency;
      break;
  }

  return ret;
}

float Config::readFloat(uint32_t idx) {
  float ret = 0.f;

  switch (idx) {
    case ICL_PREFETCH_RATIO:
      ret = prefetchRatio;
      break;
  }

  return ret;
}

bool Config::readBoolean(uint32_t idx) {
  bool ret = false;

  switch (idx) {
    case ICL_USE_READ_CACHE:
      ret = readCaching;
      break;
    case ICL_USE_WRITE_CACHE:
      ret = writeCaching;
      break;
    case ICL_USE_READ_PREFETCH:
      ret = readPrefetch;
      break;
  }

  return ret;
}

}  // namespace ICL

}  // namespace SimpleSSD
