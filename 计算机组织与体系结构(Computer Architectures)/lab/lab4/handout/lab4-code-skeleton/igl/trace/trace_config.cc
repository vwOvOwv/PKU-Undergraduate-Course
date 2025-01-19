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

#include "igl/trace/trace_config.hh"

#include "simplessd/sim/trace.hh"

namespace IGL {

const char NAME_FILE[] = "File";
const char NAME_TIMING_MODE[] = "TimingMode";
const char NAME_QUEUE_DEPTH[] = "QueueDepth";
const char NAME_IO_LIMIT[] = "IOLimit";
const char NAME_LINE_REGEX[] = "Regex";
const char NAME_GROUP_OPERATION[] = "Operation";
const char NAME_GROUP_BYTE_OFFSET[] = "ByteOffset";
const char NAME_GROUP_BYTE_LENGTH[] = "ByteLength";
const char NAME_GROUP_LBA_OFFSET[] = "LBAOffset";
const char NAME_GROUP_LBA_LENGTH[] = "LBALength";
const char NAME_GROUP_SEC[] = "Second";
const char NAME_GROUP_MILI_SEC[] = "Millisecond";
const char NAME_GROUP_MICRO_SEC[] = "Microsecond";
const char NAME_GROUP_NANO_SEC[] = "Nanosecond";
const char NAME_GROUP_PICO_SEC[] = "Picosecond";
const char NAME_LBA_SIZE[] = "LBASize";
const char NAME_USE_HEX[] = "UseHexadecimal";

TraceConfig::TraceConfig() {
  mode = MODE_SYNC;
  queueDepth = 1;
  iolimit = 0;
  groupOperation = 0;
  groupByteOffset = 0;
  groupByteLength = 0;
  groupLBAOffset = 0;
  groupLBALength = 0;
  groupSecond = 0;
  groupMiliSecond = 0;
  groupMicroSecond = 0;
  groupNanoSecond = 0;
  groupPicoSecond = 0;
  lbaSize = 512;
  useHexadecimal = false;
}

bool TraceConfig::setConfig(const char *name, const char *value) {
  bool ret = true;

  if (MATCH_NAME(NAME_FILE)) {
    file = value;
  }
  else if (MATCH_NAME(NAME_TIMING_MODE)) {
    mode = (TIMING_MODE)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_QUEUE_DEPTH)) {
    queueDepth = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_IO_LIMIT)) {
    iolimit = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_LINE_REGEX)) {
    regex = value;
  }
  else if (MATCH_NAME(NAME_GROUP_OPERATION)) {
    groupOperation = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_BYTE_OFFSET)) {
    groupByteOffset = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_BYTE_LENGTH)) {
    groupByteLength = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_LBA_OFFSET)) {
    groupLBAOffset = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_LBA_LENGTH)) {
    groupLBALength = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_SEC)) {
    groupSecond = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_MILI_SEC)) {
    groupMiliSecond = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_MICRO_SEC)) {
    groupMicroSecond = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_NANO_SEC)) {
    groupNanoSecond = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_GROUP_PICO_SEC)) {
    groupPicoSecond = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_LBA_SIZE)) {
    lbaSize = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_USE_HEX)) {
    useHexadecimal = convertBool(value);
  }
  else {
    ret = false;
  }

  return ret;
}

void TraceConfig::update() {
  uint64_t idx = 0;

  // Remove leading spaces
  for (auto c : regex) {
    if (c != ' ') {
      break;
    }

    idx++;
  }

  // Check double quotes
  if (regex[0] == '"') {
    regex = regex.substr(idx + 1);
  }
  else {
    regex = regex.substr(idx);
  }

  // Remove trailing spaces
  while (regex.back() == ' ') {
    regex.pop_back();
  }

  if (regex.back() == '"') {
    regex.pop_back();
  }

  if (mode >= MODE_NUM) {
    SimpleSSD::panic("Invalid timing mode specified");
  }
}

uint64_t TraceConfig::readUint(uint32_t idx) {
  uint64_t ret = 0;

  switch (idx) {
    case TRACE_TIMING_MODE:
      ret = mode;
      break;
    case TRACE_QUEUE_DEPTH:
      ret = queueDepth;
      break;
    case TRACE_IO_LIMIT:
      ret = iolimit;
      break;
    case TRACE_GROUP_OPERATION:
      ret = groupOperation;
      break;
    case TRACE_GROUP_BYTE_OFFSET:
      ret = groupByteOffset;
      break;
    case TRACE_GROUP_BYTE_LENGTH:
      ret = groupByteLength;
      break;
    case TRACE_GROUP_LBA_OFFSET:
      ret = groupLBAOffset;
      break;
    case TRACE_GROUP_LBA_LENGTH:
      ret = groupLBALength;
      break;
    case TRACE_GROUP_SEC:
      ret = groupSecond;
      break;
    case TRACE_GROUP_MILI_SEC:
      ret = groupMiliSecond;
      break;
    case TRACE_GROUP_MICRO_SEC:
      ret = groupMicroSecond;
      break;
    case TRACE_GROUP_NANO_SEC:
      ret = groupNanoSecond;
      break;
    case TRACE_GROUP_PICO_SEC:
      ret = groupPicoSecond;
      break;
    case TRACE_LBA_SIZE:
      ret = lbaSize;
      break;
  }

  return ret;
}

std::string TraceConfig::readString(uint32_t idx) {
  std::string ret = "";

  switch (idx) {
    case TRACE_FILE:
      ret = file;
      break;
    case TRACE_LINE_REGEX:
      ret = regex;
      break;
  }

  return ret;
}

bool TraceConfig::readBoolean(uint32_t idx) {
  bool ret = false;

  switch (idx) {
    case TRACE_USE_HEX:
      ret = useHexadecimal;
      break;
  }

  return ret;
}

}  // namespace IGL
