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

#include "sim/cfg_reader.hh"

#include "simplessd/sim/base_config.hh"
#include "simplessd/sim/trace.hh"

#ifdef _MSC_VER
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

const char SECTION_GLOBAL[] = "global";
const char SECTION_TRACE[] = "trace";
const char SECTION_REQ_GEN[] = "generator";

bool ConfigReader::init(std::string file) {
  if (ini_parse(file.c_str(), parserHandler, this) < 0) {
    return false;
  }

  // Update all
  globalConfig.update();
  traceConfig.update();
  requestConfig.update();

  return true;
}

int64_t ConfigReader::readInt(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_GLOBAL:
      return globalConfig.readInt(idx);
    case CONFIG_TRACE:
      return traceConfig.readInt(idx);
    case CONFIG_REQ_GEN:
      return requestConfig.readInt(idx);
    default:
      return 0;
  }
}

uint64_t ConfigReader::readUint(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_GLOBAL:
      return globalConfig.readUint(idx);
    case CONFIG_TRACE:
      return traceConfig.readUint(idx);
    case CONFIG_REQ_GEN:
      return requestConfig.readUint(idx);
    default:
      return 0;
  }
}

float ConfigReader::readFloat(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_GLOBAL:
      return globalConfig.readFloat(idx);
    case CONFIG_TRACE:
      return traceConfig.readFloat(idx);
    case CONFIG_REQ_GEN:
      return requestConfig.readFloat(idx);
    default:
      return 0.f;
  }
}

std::string ConfigReader::readString(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_GLOBAL:
      return globalConfig.readString(idx);
    case CONFIG_TRACE:
      return traceConfig.readString(idx);
    case CONFIG_REQ_GEN:
      return requestConfig.readString(idx);
    default:
      return std::string();
  }
}

bool ConfigReader::readBoolean(CONFIG_SECTION section, uint32_t idx) {
  switch (section) {
    case CONFIG_GLOBAL:
      return globalConfig.readBoolean(idx);
    case CONFIG_TRACE:
      return traceConfig.readBoolean(idx);
    case CONFIG_REQ_GEN:
      return requestConfig.readBoolean(idx);
    default:
      return false;
  }
}

int ConfigReader::parserHandler(void *context, const char *section,
                                const char *name, const char *value) {
  ConfigReader *pThis = (ConfigReader *)context;
  bool handled = false;

  if (MATCH_SECTION(SECTION_GLOBAL)) {
    handled = pThis->globalConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_TRACE)) {
    handled = pThis->traceConfig.setConfig(name, value);
  }
  else if (MATCH_SECTION(SECTION_REQ_GEN)) {
    handled = pThis->requestConfig.setConfig(name, value);
  }

  if (!handled) {
    SimpleSSD::warn("Config [%s] %s = %s not handled", section, name, value);
  }

  return 1;
}
