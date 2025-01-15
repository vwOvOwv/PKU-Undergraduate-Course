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

#pragma once

#ifndef __SIM_CONFIG_READER_2__
#define __SIM_CONFIG_READER_2__

#include <cinttypes>
#include <string>

#include "igl/request/request_config.hh"
#include "igl/trace/trace_config.hh"
#include "sim/global_config.hh"
#include "simplessd/lib/inih/ini.h"

typedef enum {
  CONFIG_GLOBAL,
  CONFIG_TRACE,
  CONFIG_REQ_GEN,
} CONFIG_SECTION;

class ConfigReader {
 private:
  Config globalConfig;
  IGL::TraceConfig traceConfig;
  IGL::RequestConfig requestConfig;

  static int parserHandler(void *, const char *, const char *, const char *);

 public:
  bool init(std::string);

  int64_t readInt(CONFIG_SECTION, uint32_t);
  uint64_t readUint(CONFIG_SECTION, uint32_t);
  float readFloat(CONFIG_SECTION, uint32_t);
  std::string readString(CONFIG_SECTION, uint32_t);
  bool readBoolean(CONFIG_SECTION, uint32_t);
};

#endif
