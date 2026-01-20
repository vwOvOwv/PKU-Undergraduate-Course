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

#include "igl/request/request_generator.hh"

#include <iostream>

#include "simplessd/sim/trace.hh"
#include "simplessd/util/algorithm.hh"

namespace IGL {

RequestGenerator::RequestGenerator(Engine &e, BIL::BlockIOEntry &b,
                                   std::function<void()> &f, ConfigReader &c)
    : IOGenerator(e, b, f),
      io_submitted(0),
      io_count(0),
      read_count(0),
      io_depth(0),
      reserveTermination(false) {
  // Read config
  io_size = c.readUint(CONFIG_REQ_GEN, REQUEST_IO_SIZE);
  type = (IO_TYPE)c.readUint(CONFIG_REQ_GEN, REQUEST_IO_TYPE);
  mode = (IO_MODE)c.readUint(CONFIG_REQ_GEN, REQUEST_IO_MODE);
  iodepth = c.readUint(CONFIG_REQ_GEN, REQUEST_IO_DEPTH);
  rwmixread = c.readFloat(CONFIG_REQ_GEN, REQUEST_IO_MIX_RATIO);
  offset = c.readUint(CONFIG_REQ_GEN, REQUEST_OFFSET);
  size = c.readUint(CONFIG_REQ_GEN, REQUEST_SIZE);
  thinktime = c.readUint(CONFIG_REQ_GEN, REQUEST_THINKTIME);
  blocksize = c.readUint(CONFIG_REQ_GEN, REQUEST_BLOCK_SIZE);
  blockalign = c.readUint(CONFIG_REQ_GEN, REQUEST_BLOCK_ALIGN);
  randseed = c.readUint(CONFIG_REQ_GEN, REQUEST_RANDOM_SEED);
  time_based = c.readBoolean(CONFIG_REQ_GEN, REQUEST_TIME_BASED);
  runtime = c.readUint(CONFIG_REQ_GEN, REQUEST_RUN_TIME);

  if (blockalign == 0) {
    blockalign = blocksize;
  }

  if (mode == IO_SYNC) {
    iodepth = 1;
    mode = IO_ASYNC;
  }

  submissionLatency = c.readUint(CONFIG_GLOBAL, GLOBAL_SUBMISSION_LATENCY);
  completionLatency = c.readUint(CONFIG_GLOBAL, GLOBAL_COMPLETION_LATENCY);

  // Set random engine
  randengine.seed(randseed);

  submitIO = [this](uint64_t tick) { _submitIO(tick); };
  iocallback = [this](uint64_t id) { _iocallback(id); };

  submitEvent = engine.allocateEvent(submitIO);
}

RequestGenerator::~RequestGenerator() {}

void RequestGenerator::init(uint64_t bytesize, uint32_t bs) {
  if (offset > bytesize) {
    SimpleSSD::panic("offset is larger than SSD size");
  }
  if (blocksize < bs) {
    SimpleSSD::panic("blocksize is smaller than SSD's logical block");
  }
  if (blockalign < bs) {
    SimpleSSD::panic("blockalign is smaller than SSD's logical block");
  }
  if (blocksize % bs != 0) {
    SimpleSSD::warn("blocksize is not aligned to SSD's logical block");

    blocksize /= bs;
    blocksize *= bs;
  }
  if (blockalign % bs != 0) {
    SimpleSSD::warn("blockalign is not aligned to SSD's logical block");

    blockalign /= bs;
    blockalign *= bs;
  }
  if (offset % blockalign != 0) {
    SimpleSSD::warn("offset is not aligned to blockalign");

    offset /= blockalign;
    offset *= blockalign;
  }
  if (size == 0 || offset + size > bytesize) {
    size = bytesize - offset;
  }
  if (size == 0) {
    SimpleSSD::panic("Invalid offset and size provided");
  }

  randgen = std::uniform_int_distribution<uint64_t>(offset, offset + size);
}

void RequestGenerator::begin() {
  initTime = engine.getCurrentTick();

  _submitIO(initTime);
}

void RequestGenerator::printStats(std::ostream &out) {
  uint64_t tick = engine.getCurrentTick();

  out << "*** Statistics of Request Generator ***" << std::endl;
  out << "Tick: " << tick << std::endl;
  out << "Time (ps): " << initTime << " - " << tick << " (" << tick - initTime
      << ")" << std::endl;
  out << "I/O (bytes): " << io_submitted << " ("
      << std::to_string((double)io_submitted / (tick - initTime) *
                        1000000000000.)
      << " B/s)" << std::endl;
  out << "I/O (counts): " << io_count << " (Read: " << read_count
      << ", Write: " << io_count - read_count << ")" << std::endl;
  out << "*** End of statistics ***" << std::endl;

  bioEntry.printStats(out);
}

void RequestGenerator::getProgress(float &val) {
  bool zero = false;

  {
    std::lock_guard<std::mutex> guard(m);

    if (io_submitted == 0) {
      zero = true;
    }
  }

  if (zero) {
    val = 0.f;

    return;
  }

  if (time_based) {                           // Read-only variable after init
    uint64_t tick = engine.getCurrentTick();  // Thread-safe

    // initTime is read-only after begin() called
    val = (float)(tick - initTime) / runtime;
  }
  else {
    std::lock_guard<std::mutex> guard(m);

    val = (float)io_submitted / io_size;
  }
}

void RequestGenerator::generateAddress(uint64_t &off, uint64_t &len) {
  // This function generates address to access
  // based on I/O type, blocksize/align and offset/size
  if (type == IO_RANDREAD || type == IO_RANDWRITE || type == IO_RANDRW) {
    off = randgen(randengine);  // randgen range: [offset, offset + size)
    off -= off % blockalign;
    len = blocksize;
  }
  else {
    off = io_count * blockalign;
    len = blocksize;

    // Limit range of address to [offset, offset + size)
    while (off + len > size) {
      if (off >= size) {
        off -= size;
      }
      else {
        // TODO: is this correct?
        off -= len;
      }
    }

    off += offset;

    // printf("off: %lu, len: %lu\n", off, len);
  }
}

bool RequestGenerator::nextIOIsRead() {
  // This function determine next I/O is read or write
  // based on rwmixread
  // io_count should not zero
  if (type == IO_READWRITE || type == IO_RANDRW) {
    if (rwmixread > (float)read_count / io_count) {
      return true;
    }
  }
  else if (type == IO_READ || type == IO_RANDREAD) {
    return true;
  }

  return false;
}

void RequestGenerator::_submitIO(uint64_t) {
  BIL::BIO bio;

  // This function uses io_count (=0 at very beginning)
  generateAddress(bio.offset, bio.length);

  bio.id = io_count++;

  // This function also uses io_count (=1 at very beginning)
  if (nextIOIsRead()) {
    bio.type = BIL::BIO_READ;
    read_count++;
  }
  else {
    bio.type = BIL::BIO_WRITE;
  }

  io_submitted += bio.length;

  bio.callback = iocallback;

  // push to queue
  io_depth++;

  // Submit to Block I/O entry
  bioEntry.submitIO(bio);

  // Check on-the-fly I/O depth
  rescheduleSubmit(submissionLatency);
}

void RequestGenerator::_iocallback(uint64_t) {
  io_depth--;

  if (reserveTermination) {
    // No I/O will be generated anymore
    // If no pending I/O call endCallback
    if (io_depth == 0) {
      endCallback();
    }
  }
  else {
    // Check on-the-fly I/O depth
    rescheduleSubmit(submissionLatency + completionLatency);
  }
}

void RequestGenerator::rescheduleSubmit(uint64_t breakTime) {
  uint64_t tick = engine.getCurrentTick();

  // We are done
  if ((!time_based && io_submitted >= io_size) ||
      (time_based && runtime <= (tick - initTime))) {
    reserveTermination = true;

    // We need to double-check this for following case:
    // _iocallback (all I/O completed) -> rescheduleSubmit
    if (io_depth == 0) {
      endCallback();
    }

    return;
  }

  if (io_depth < iodepth) {
    uint64_t scheduledTick;
    bool doSchedule = true;

    // Check conflict
    if (engine.isScheduled(submitEvent, &scheduledTick)) {
      if (scheduledTick >= tick + breakTime) {
        doSchedule = false;
      }
    }

    // We can schedule it
    if (doSchedule) {
      engine.scheduleEvent(submitEvent, tick + breakTime);
    }
  }
}

}  // namespace IGL
