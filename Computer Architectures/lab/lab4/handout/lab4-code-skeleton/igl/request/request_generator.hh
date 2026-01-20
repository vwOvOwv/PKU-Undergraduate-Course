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

#ifndef __IGL_REQUEST_GENERATOR__
#define __IGL_REQUEST_GENERATOR__

#include <list>
#include <mutex>
#include <random>
#include <thread>

#include "bil/entry.hh"
#include "igl/io_gen.hh"
#include "sim/cfg_reader.hh"
#include "sim/engine.hh"

namespace IGL {

class RequestGenerator : public IOGenerator {
 private:
  std::mutex m;

  uint64_t io_size;
  uint64_t io_submitted;

  IO_TYPE type;

  IO_MODE mode;
  uint64_t iodepth;

  uint64_t io_count;
  uint64_t read_count;
  float rwmixread;

  uint64_t offset;
  uint64_t size;

  uint64_t thinktime;

  uint64_t blocksize;
  uint64_t blockalign;

  uint64_t randseed;
  std::mt19937_64 randengine;
  std::uniform_int_distribution<uint64_t> randgen;

  bool time_based;
  uint64_t runtime;

  uint64_t submissionLatency;
  uint64_t completionLatency;

  uint64_t io_depth;

  uint64_t initTime;
  bool reserveTermination;

  void generateAddress(uint64_t &, uint64_t &);
  bool nextIOIsRead();
  void rescheduleSubmit(uint64_t);

  SimpleSSD::Event submitEvent;
  SimpleSSD::EventFunction submitIO;
  SimpleSSD::EventFunction iocallback;

  void _submitIO(uint64_t);
  void _iocallback(uint64_t);

 public:
  RequestGenerator(Engine &, BIL::BlockIOEntry &, std::function<void()> &,
                   ConfigReader &);
  ~RequestGenerator();

  void init(uint64_t, uint32_t) override;
  void begin() override;
  void printStats(std::ostream &) override;
  void getProgress(float &) override;
};

}  // namespace IGL

#endif
