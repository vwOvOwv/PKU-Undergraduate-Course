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

#ifndef __SIM_GLOBAL_CONFIG__
#define __SIM_GLOBAL_CONFIG__

#include "simplessd/sim/base_config.hh"

typedef enum {
  GLOBAL_SIM_MODE,
  GLOBAL_LOG_PERIOD,
  GLOBAL_LOG_FILE,
  GLOBAL_DEBUG_LOG_FILE,
  GLOBAL_LATENCY_LOG_FILE,
  GLOBAL_PROGRESS_PERIOD,
  GLOBAL_INTERFACE,
  GLOBAL_SCHEDULER,
  GLOBAL_SUBMISSION_LATENCY,
  GLOBAL_COMPLETION_LATENCY,
} GLOBAL_CONFIG;

typedef enum {
  MODE_REQUEST_GENERATOR,
  MODE_TRACE_REPLAYER,
  MODE_NUM,
} SIM_MODE;

typedef enum {
  INTERFACE_NONE,
  INTERFACE_NVME,
  INTERFACE_SATA,
  INTERFACE_UFS,
  INTERFACE_NUM,
} INTERFACE;

typedef enum {
  SCHEDULER_NOOP,
  SCHEDULER_NUM,
} SCHEDULER;

class Config : public SimpleSSD::BaseConfig {
 private:
  SIM_MODE mode;
  uint64_t logPeriod;
  std::string logFile;
  std::string logDebugFile;
  std::string latencyFile;
  uint64_t progressPeriod;
  INTERFACE interface;
  SCHEDULER scheduler;
  uint64_t submissionLatency;
  uint64_t completionLatency;

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  uint64_t readUint(uint32_t) override;
  std::string readString(uint32_t) override;
};

#endif
