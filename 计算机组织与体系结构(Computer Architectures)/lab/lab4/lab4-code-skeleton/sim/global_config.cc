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

#include "sim/global_config.hh"

#include "simplessd/sim/trace.hh"
#include "util/convert.hh"

const char NAME_MODE[] = "Mode";
const char NAME_LOG_PERIOD[] = "LogPeriod";
const char NAME_LOG_FILE[] = "LogFile";
const char NAME_DEBUG_LOG_FILE[] = "DebugLogFile";
const char NAME_LATENCY_LOG_FILE[] = "LatencyLogFile";
const char NAME_PROGRESS_PERIOD[] = "ProgressPeriod";
const char NAME_INTERFACE[] = "Interface";
const char NAME_SCHEDULER[] = "Scheduler";
const char NAME_SUBMISSION_LATENCY[] = "SubmissionLatency";
const char NAME_COMPLETION_LATENCY[] = "CompletionLatency";

Config::Config() {
  mode = MODE_REQUEST_GENERATOR;
  logPeriod = 0;
  progressPeriod = 0;
  interface = INTERFACE_NVME;
  scheduler = SCHEDULER_NOOP;
}

bool Config::setConfig(const char *name, const char *value) {
  bool ret = true;

  if (MATCH_NAME(NAME_MODE)) {
    mode = (SIM_MODE)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_LOG_PERIOD)) {
    logPeriod = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_LOG_FILE)) {
    logFile = value;
  }
  else if (MATCH_NAME(NAME_DEBUG_LOG_FILE)) {
    logDebugFile = value;
  }
  else if (MATCH_NAME(NAME_LATENCY_LOG_FILE)) {
    latencyFile = value;
  }
  else if (MATCH_NAME(NAME_PROGRESS_PERIOD)) {
    progressPeriod = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_INTERFACE)) {
    interface = (INTERFACE)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_SCHEDULER)) {
    scheduler = (SCHEDULER)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_SUBMISSION_LATENCY)) {
    submissionLatency = convertTime(value);
  }
  else if (MATCH_NAME(NAME_COMPLETION_LATENCY)) {
    completionLatency = convertTime(value);
  }
  else {
    ret = false;
  }

  return ret;
}

void Config::update() {
  if (mode >= MODE_NUM) {
    SimpleSSD::panic("Invalid simulation mode");
  }
  if (interface >= INTERFACE_NUM) {
    SimpleSSD::panic("Invalid interface");
  }
}

uint64_t Config::readUint(uint32_t idx) {
  uint64_t ret = 0;

  switch (idx) {
    case GLOBAL_SIM_MODE:
      ret = mode;
      break;
    case GLOBAL_LOG_PERIOD:
      ret = logPeriod;
      break;
    case GLOBAL_PROGRESS_PERIOD:
      ret = progressPeriod;
      break;
    case GLOBAL_INTERFACE:
      ret = interface;
      break;
    case GLOBAL_SCHEDULER:
      ret = scheduler;
      break;
    case GLOBAL_SUBMISSION_LATENCY:
      ret = submissionLatency;
      break;
    case GLOBAL_COMPLETION_LATENCY:
      ret = completionLatency;
      break;
  }

  return ret;
}

std::string Config::readString(uint32_t idx) {
  std::string ret = "";

  switch (idx) {
    case GLOBAL_LOG_FILE:
      ret = logFile;
      break;
    case GLOBAL_DEBUG_LOG_FILE:
      ret = logDebugFile;
      break;
    case GLOBAL_LATENCY_LOG_FILE:
      ret = latencyFile;
      break;
  }

  return ret;
}
