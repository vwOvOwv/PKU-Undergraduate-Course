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

#ifndef __CPU_CPU__
#define __CPU_CPU__

#include <cinttypes>
#include <queue>
#include <unordered_map>
#include <vector>

#include "cpu/def.hh"
#include "lib/mcpat/mcpat.h"
#include "sim/config_reader.hh"
#include "sim/dma_interface.hh"
#include "sim/simulator.hh"
#include "sim/statistics.hh"

namespace SimpleSSD {

namespace CPU {

typedef struct _InstStat {
  // Instruction count
  uint64_t branch;
  uint64_t load;
  uint64_t store;
  uint64_t arithmetic;
  uint64_t floatingPoint;
  uint64_t otherInsts;

  // Total times in ps to execute this insturcion group
  uint64_t latency;

  _InstStat();
  _InstStat(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
            uint64_t);
  _InstStat &operator+=(const _InstStat &);
  uint64_t sum();
} InstStat;

typedef struct _JobEntry {
  DMAFunction func;
  void *context;
  InstStat *inst;
  uint64_t submitAt;
  uint64_t delay;

  _JobEntry(DMAFunction &, void *, InstStat *);
} JobEntry;

class CPU : public StatObject {
 private:
  struct CoreStat {
    InstStat instStat;
    uint64_t busy;

    CoreStat();
  };

  class Core {
   private:
    bool busy;

    Event jobEvent;
    std::queue<JobEntry> jobs;

    CoreStat stat;

    void handleJob();
    void jobDone();

   public:
    Core();
    ~Core();

    void submitJob(JobEntry, uint64_t = 0);

    void addStat(InstStat &);

    bool isBusy();
    uint64_t getJobListSize();
    CoreStat &getStat();
  };

  ConfigReader &conf;
  uint64_t lastResetStat;

  uint64_t clockSpeed;
  uint64_t clockPeriod;

  // Cores
  std::vector<Core> hilCore;
  std::vector<Core> iclCore;
  std::vector<Core> ftlCore;

  // CPIs
  std::unordered_map<uint16_t, std::unordered_map<uint16_t, InstStat>> cpi;

  uint32_t leastBusyCPU(std::vector<Core> &);
  void calculatePower(Power &);

 public:
  CPU(ConfigReader &);
  ~CPU();

  void execute(NAMESPACE, FUNCTION, DMAFunction &, void * = nullptr,
               uint64_t = 0);
  uint64_t applyLatency(NAMESPACE, FUNCTION);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;

  void printLastStat();
};

}  // namespace CPU

}  // namespace SimpleSSD

#endif
