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

#include "cpu/config.hh"

#include "util/algorithm.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace CPU {

const char NAME_CLOCK[] = "ClockSpeed";
const char NAME_CORE_HIL[] = "HILCoreCount";
const char NAME_CORE_ICL[] = "ICLCoreCount";
const char NAME_CORE_FTL[] = "FTLCoreCount";

Config::Config() {
  clock = 400000000;
  hilCore = 1;
  iclCore = 1;
  ftlCore = 1;
}

bool Config::setConfig(const char *name, const char *value) {
  bool ret = true;

  if (MATCH_NAME(NAME_CLOCK)) {
    clock = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_CORE_HIL)) {
    hilCore = (uint32_t)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_CORE_ICL)) {
    iclCore = (uint32_t)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_CORE_FTL)) {
    ftlCore = (uint32_t)strtoul(value, nullptr, 10);
  }
  else {
    ret = false;
  }

  return ret;
}

void Config::update() {
  if (clock == 0) {
    panic("Invalid ClockSpeed");
  }
}

uint64_t Config::readUint(uint32_t idx) {
  uint64_t ret = 0;

  switch (idx) {
    case CPU_CLOCK:
      ret = clock;
      break;
    case CPU_CORE_HIL:
      ret = hilCore;
      break;
    case CPU_CORE_ICL:
      ret = iclCore;
      break;
    case CPU_CORE_FTL:
      ret = ftlCore;
      break;
  }

  return ret;
}

}  // namespace CPU

}  // namespace SimpleSSD
