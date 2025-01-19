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

#include "bil/entry.hh"

#include <cmath>

#include "bil/interface.hh"
#include "bil/noop_scheduler.hh"
#include "simplessd/sim/trace.hh"

namespace BIL {

BlockIOEntry::BlockIOEntry(ConfigReader &c, Engine &e, DriverInterface *i,
                           std::ostream *o)
    : conf(c),
      engine(e),
      pLatencyFile(o),
      pScheduler(nullptr),
      pDriver(i),
      lastProgress(0),
      io_progress(0),
      io_count(0),
      minLatency(std::numeric_limits<uint64_t>::max()),
      maxLatency(0),
      sumLatency(0),
      squareSumLatency(0),
      callback([this](uint64_t id) { completion(id); }) {
  switch (c.readUint(CONFIG_GLOBAL, GLOBAL_SCHEDULER)) {
    case SCHEDULER_NOOP:
      pScheduler = new NoopScheduler(e, i);

      break;
    default:
      SimpleSSD::panic("Invalid I/O scheduler specified");

      break;
  }

  pScheduler->init();
}

BlockIOEntry::~BlockIOEntry() {
  delete pScheduler;
}

void BlockIOEntry::submitIO(BIO &bio) {
  BIO copy(bio);

  io_count++;
  bio.submittedAt = engine.getCurrentTick();

  ioQueue.push_back(bio);
  copy.callback = callback;

  pScheduler->submitIO(copy);
}

void BlockIOEntry::completion(uint64_t id) {
  uint64_t tick = engine.getCurrentTick();

  for (auto iter = ioQueue.begin(); iter != ioQueue.end(); iter++) {
    if (iter->id == id) {
      tick = tick - iter->submittedAt;

      {
        std::lock_guard<std::mutex> guard(m);

        io_progress++;

        progress.latency += tick;
        progress.iops++;
        progress.bandwidth += iter->length;
      }

      if (pLatencyFile) {
        *pLatencyFile << std::to_string(iter->id) << ", "
                      << std::to_string(iter->offset) << ", "
                      << std::to_string(iter->length) << ", "
                      << std::to_string(tick) << std::endl;
      }

      iter->callback(id);

      ioQueue.erase(iter);

      break;
    }
  }

  if (minLatency > tick) {
    minLatency = tick;
  }
  if (maxLatency < tick) {
    maxLatency = tick;
  }

  sumLatency += tick;
  squareSumLatency += tick * tick;
}

void BlockIOEntry::printStats(std::ostream &out) {
  double avgLatency = (double)sumLatency / io_count;
  double stdevLatency = sqrt((double)squareSumLatency / io_count - avgLatency);
  double digit = log10(avgLatency);

  out << "*** Statistics of Block I/O Entry ***" << std::endl;

  if (digit < 6.0) {
    out << "Latency (ps): min=" << std::to_string(minLatency)
        << ", max=" << std::to_string(maxLatency)
        << ", avg=" << std::to_string(avgLatency)
        << ", stdev=" << std::to_string(stdevLatency) << std::endl;
  }
  else if (digit < 9.0) {
    out << "Latency (ns): min=" << std::to_string(minLatency / 1000.0)
        << ", max=" << std::to_string(maxLatency / 1000.0)
        << ", avg=" << std::to_string(avgLatency / 1000.0)
        << ", stdev=" << std::to_string(stdevLatency / 31.62277660168)
        << std::endl;
  }
  else if (digit < 12.0) {
    out << "Latency (us): min=" << std::to_string(minLatency / 1000000.0)
        << ", max=" << std::to_string(maxLatency / 1000000.0)
        << ", avg=" << std::to_string(avgLatency / 1000000.0)
        << ", stdev=" << std::to_string(stdevLatency / 1000.0) << std::endl;
  }
  else {
    out << "Latency (ms): min=" << std::to_string(minLatency / 1000000000.0)
        << ", max=" << std::to_string(maxLatency / 1000000000.0)
        << ", avg=" << std::to_string(avgLatency / 1000000000.0)
        << ", stdev=" << std::to_string(stdevLatency / 31622.77660168379)
        << std::endl;
  }

  out << "*** End of statistics ***" << std::endl;
}

void BlockIOEntry::getProgress(Progress &data) {
  uint64_t tick = engine.getCurrentTick();
  uint64_t diff = tick - lastProgress;

  if (diff == 0) {
    data.iops = 0;
    data.bandwidth = 0;
    data.latency = 0;

    return;
  }

  double ratio = 1000000000000.0 / diff;

  {
    std::lock_guard<std::mutex> guard(m);

    data.iops = progress.iops * ratio;
    data.bandwidth = progress.bandwidth * ratio;

    if (io_progress == 0) {
      data.latency = 0;
    }
    else {
      data.latency = progress.latency / io_progress;
    }

    io_progress = 0;
    progress.iops = 0;
    progress.bandwidth = 0;
    progress.latency = 0;
  }

  lastProgress = tick;
}

}  // namespace BIL
