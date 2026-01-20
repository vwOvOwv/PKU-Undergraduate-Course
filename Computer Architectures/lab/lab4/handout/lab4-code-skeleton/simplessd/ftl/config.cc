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

#include "ftl/config.hh"

#include "util/simplessd.hh"

namespace SimpleSSD {

namespace FTL {

const char NAME_MAPPING_MODE[] = "MappingMode";
const char NAME_OVERPROVISION_RATIO[] = "OverProvisioningRatio";
const char NAME_GC_THRESHOLD[] = "GCThreshold";
const char NAME_BAD_BLOCK_THRESHOLD[] = "EraseThreshold";
const char NAME_FILLING_MODE[] = "FillingMode";
const char NAME_FILL_RATIO[] = "FillRatio";
const char NAME_INVALID_PAGE_RATIO[] = "InvalidPageRatio";
const char NAME_GC_MODE[] = "GCMode";
const char NAME_GC_RECLAIM_BLOCK[] = "GCReclaimBlocks";
const char NAME_GC_RECLAIM_THRESHOLD[] = "GCReclaimThreshold";
const char NAME_GC_EVICT_POLICY[] = "EvictPolicy";
const char NAME_GC_D_CHOICE_PARAM[] = "DChoiceParam";
const char NAME_USE_RANDOM_IO_TWEAK[] = "EnableRandomIOTweak";

Config::Config() {
  mapping = PAGE_MAPPING;
  overProvision = 0.25f;
  gcThreshold = 0.05f;
  badBlockThreshold = 100000;
  fillingMode = FILLING_MODE_0;
  fillingRatio = 0.f;
  invalidRatio = 0.f;
  reclaimBlock = 1;
  reclaimThreshold = 0.1f;
  gcMode = GC_MODE_0;
  evictPolicy = POLICY_GREEDY;
  dChoiceParam = 3;
  randomIOTweak = true;
}

bool Config::setConfig(const char *name, const char *value) {
  bool ret = true;

  if (MATCH_NAME(NAME_MAPPING_MODE)) {
    mapping = (MAPPING)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_OVERPROVISION_RATIO)) {
    overProvision = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_GC_THRESHOLD)) {
    gcThreshold = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_BAD_BLOCK_THRESHOLD)) {
    badBlockThreshold = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_FILLING_MODE)) {
    fillingMode = (FILLING_MODE)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_FILL_RATIO)) {
    fillingRatio = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_INVALID_PAGE_RATIO)) {
    invalidRatio = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_GC_MODE)) {
    gcMode = (GC_MODE)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GC_RECLAIM_BLOCK)) {
    reclaimBlock = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GC_RECLAIM_THRESHOLD)) {
    reclaimThreshold = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_GC_EVICT_POLICY)) {
    evictPolicy = (EVICT_POLICY)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GC_D_CHOICE_PARAM)) {
    dChoiceParam = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_USE_RANDOM_IO_TWEAK)) {
    randomIOTweak = convertBool(value);
  }
  else {
    ret = false;
  }

  return ret;
}

void Config::update() {
  if (gcMode == GC_MODE_0 && reclaimBlock == 0) {
    panic("Invalid GCReclaimBlocks");
  }

  if (gcMode == GC_MODE_1 && reclaimThreshold < gcThreshold) {
    panic("Invalid GCReclaimThreshold");
  }

  if (fillingRatio < 0.f || fillingRatio > 1.f) {
    panic("Invalid FillingRatio");
  }

  if (invalidRatio < 0.f || invalidRatio > 1.f) {
    panic("Invalid InvalidPageRatio");
  }
}

int64_t Config::readInt(uint32_t idx) {
  int64_t ret = 0;

  switch (idx) {
    case FTL_MAPPING_MODE:
      ret = mapping;
      break;
    case FTL_GC_MODE:
      ret = gcMode;
      break;
    case FTL_GC_EVICT_POLICY:
      ret = evictPolicy;
      break;
  }

  return ret;
}

uint64_t Config::readUint(uint32_t idx) {
  uint64_t ret = 0;

  switch (idx) {
    case FTL_FILLING_MODE:
      ret = fillingMode;
      break;
    case FTL_BAD_BLOCK_THRESHOLD:
      ret = badBlockThreshold;
      break;
    case FTL_GC_RECLAIM_BLOCK:
      ret = reclaimBlock;
      break;
    case FTL_GC_D_CHOICE_PARAM:
      ret = dChoiceParam;
      break;
  }

  return ret;
}

float Config::readFloat(uint32_t idx) {
  float ret = 0.f;

  switch (idx) {
    case FTL_OVERPROVISION_RATIO:
      ret = overProvision;
      break;
    case FTL_GC_THRESHOLD_RATIO:
      ret = gcThreshold;
      break;
    case FTL_FILL_RATIO:
      ret = fillingRatio;
      break;
    case FTL_INVALID_PAGE_RATIO:
      ret = invalidRatio;
      break;
    case FTL_GC_RECLAIM_THRESHOLD:
      ret = reclaimThreshold;
      break;
  }

  return ret;
}

bool Config::readBoolean(uint32_t idx) {
  bool ret = false;

  switch (idx) {
    case FTL_USE_RANDOM_IO_TWEAK:
      ret = randomIOTweak;
      break;
  }

  return ret;
}

}  // namespace FTL

}  // namespace SimpleSSD
