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

#ifndef __IGL_TRACE_CONFIG__
#define __IGL_TRACE_CONFIG__

#include "simplessd/sim/base_config.hh"

namespace IGL {

typedef enum {
  TRACE_FILE,
  TRACE_TIMING_MODE,
  TRACE_QUEUE_DEPTH,
  TRACE_IO_LIMIT,
  TRACE_LINE_REGEX,
  TRACE_GROUP_OPERATION,
  TRACE_GROUP_BYTE_OFFSET,
  TRACE_GROUP_BYTE_LENGTH,
  TRACE_GROUP_LBA_OFFSET,
  TRACE_GROUP_LBA_LENGTH,
  TRACE_GROUP_SEC,
  TRACE_GROUP_MILI_SEC,
  TRACE_GROUP_MICRO_SEC,
  TRACE_GROUP_NANO_SEC,
  TRACE_GROUP_PICO_SEC,
  TRACE_LBA_SIZE,
  TRACE_USE_HEX,
} TRACE_CONFIG;

typedef enum {
  MODE_SYNC,
  MODE_ASYNC,
  MODE_STRICT,
  MODE_NUM,
} TIMING_MODE;

class TraceConfig : public SimpleSSD::BaseConfig {
 private:
  std::string file;
  TIMING_MODE mode;
  uint32_t queueDepth;
  uint64_t iolimit;
  std::string regex;
  uint32_t groupOperation;
  uint32_t groupByteOffset;
  uint32_t groupByteLength;
  uint32_t groupLBAOffset;
  uint32_t groupLBALength;
  uint32_t groupSecond;
  uint32_t groupMiliSecond;
  uint32_t groupMicroSecond;
  uint32_t groupNanoSecond;
  uint32_t groupPicoSecond;
  uint32_t lbaSize;
  bool useHexadecimal;

 public:
  TraceConfig();

  bool setConfig(const char *, const char *) override;
  void update() override;

  uint64_t readUint(uint32_t) override;
  std::string readString(uint32_t) override;
  bool readBoolean(uint32_t) override;
};

}  // namespace IGL

#endif
