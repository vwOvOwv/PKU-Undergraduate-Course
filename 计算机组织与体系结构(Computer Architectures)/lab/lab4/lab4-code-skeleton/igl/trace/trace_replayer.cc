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

#include "igl/trace/trace_replayer.hh"

#include "simplessd/sim/trace.hh"
#include "simplessd/util/algorithm.hh"

namespace IGL {

TraceReplayer::TraceReplayer(Engine &e, BIL::BlockIOEntry &b,
                             std::function<void()> &f, ConfigReader &c)
    : IOGenerator(e, b, f),
      useLBAOffset(false),
      useLBALength(false),
      nextIOIsSync(false),
      reserveTermination(false),
      io_submitted(0),
      io_count(0),
      read_count(0),
      write_count(0),
      io_depth(0) {
  // Check file
  auto filename = c.readString(CONFIG_TRACE, TRACE_FILE);
  file.open(filename);

  if (!file.is_open()) {
    SimpleSSD::panic("Failed to open trace file %s!", filename.c_str());
  }

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // Create regex
  try {
    regex = std::regex(c.readString(CONFIG_TRACE, TRACE_LINE_REGEX));
  }
  catch (std::regex_error &e) {
    SimpleSSD::panic("Invalid regular expression!");
  }

  // Fill flags
  mode = (TIMING_MODE)c.readUint(CONFIG_TRACE, TRACE_TIMING_MODE);
  submissionLatency = c.readUint(CONFIG_GLOBAL, GLOBAL_SUBMISSION_LATENCY);
  completionLatency = c.readUint(CONFIG_GLOBAL, GLOBAL_COMPLETION_LATENCY);
  maxQueueDepth = c.readUint(CONFIG_TRACE, TRACE_QUEUE_DEPTH);
  max_io = c.readUint(CONFIG_TRACE, TRACE_IO_LIMIT);
  groupID[ID_OPERATION] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_OPERATION);
  groupID[ID_BYTE_OFFSET] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_BYTE_OFFSET);
  groupID[ID_BYTE_LENGTH] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_BYTE_LENGTH);
  groupID[ID_LBA_OFFSET] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_LBA_OFFSET);
  groupID[ID_LBA_LENGTH] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_LBA_LENGTH);
  groupID[ID_TIME_SEC] = (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_SEC);
  groupID[ID_TIME_MS] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_MILI_SEC);
  groupID[ID_TIME_US] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_MICRO_SEC);
  groupID[ID_TIME_NS] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_NANO_SEC);
  groupID[ID_TIME_PS] =
      (uint32_t)c.readUint(CONFIG_TRACE, TRACE_GROUP_PICO_SEC);
  useHex = c.readBoolean(CONFIG_TRACE, TRACE_USE_HEX);

  if (groupID[ID_OPERATION] == 0) {
    SimpleSSD::panic("Operation group ID cannot be 0");
  }

  if (groupID[ID_LBA_OFFSET] > 0) {
    useLBAOffset = true;
  }
  if (groupID[ID_LBA_LENGTH] > 0) {
    useLBALength = true;
  }

  if (useLBALength || useLBAOffset) {
    lbaSize = (uint32_t)c.readUint(CONFIG_TRACE, TRACE_LBA_SIZE);

    if (SimpleSSD::popcount(lbaSize) != 1) {
      SimpleSSD::panic("LBA size should be power of 2");
    }
  }

  if (!useLBAOffset && groupID[ID_BYTE_OFFSET] == 0) {
    SimpleSSD::panic("Both LBA Offset and Byte Offset group ID cannot be 0");
  }
  if (!useLBALength && groupID[ID_BYTE_LENGTH] == 0) {
    SimpleSSD::panic("Both LBA Length and Byte Length group ID cannot be 0");
  }

  timeValids[0] = groupID[ID_TIME_SEC] > 0 ? true : false;
  timeValids[1] = groupID[ID_TIME_MS] > 0 ? true : false;
  timeValids[2] = groupID[ID_TIME_US] > 0 ? true : false;
  timeValids[3] = groupID[ID_TIME_NS] > 0 ? true : false;
  timeValids[4] = groupID[ID_TIME_PS] > 0 ? true : false;

  if (!(timeValids[0] || timeValids[1] || timeValids[2] || timeValids[3] ||
        timeValids[4])) {
    if (mode == MODE_STRICT) {
      SimpleSSD::panic("No valid time field specified");
    }
  }

  firstTick = std::numeric_limits<uint64_t>::max();

  completionEvent = [this](uint64_t id) { iocallback(id); };

  submitEvent = engine.allocateEvent([this](uint64_t) { submitIO(); });
}

TraceReplayer::~TraceReplayer() {
  file.close();
}

void TraceReplayer::init(uint64_t bytesize, uint32_t bs) {
  ssdSize = bytesize;
  blocksize = bs;

  if ((useLBALength || useLBAOffset) && lbaSize < bs) {
    SimpleSSD::warn("LBA size of trace file is smaller than SSD's LBA size");
  }
}

void TraceReplayer::begin() {
  initTime = engine.getCurrentTick();

  parseLine();

  if (mode == MODE_STRICT) {
    firstTick = linedata.tick;
  }
  else {
    firstTick = initTime;
  }

  if (reserveTermination) {
    SimpleSSD::warn("No I/O submitted. Check regular expression.");

    endCallback();
  }
  else {
    submitIO();
  }
}

void TraceReplayer::printStats(std::ostream &out) {
  uint64_t tick = engine.getCurrentTick();

  out << "*** Statistics of Trace Replayer ***" << std::endl;
  out << "Tick: " << tick << std::endl;
  out << "Time (ps): " << firstTick - initTime << " - " << tick << " ("
      << tick + firstTick - initTime << ")" << std::endl;
  out << "I/O (bytes): " << io_submitted << " ("
      << std::to_string((double)io_submitted / tick * 1000000000000.) << " B/s)"
      << std::endl;
  out << "I/O (counts): " << io_count << " (Read: " << read_count
      << ", Write: " << write_count << ")" << std::endl;
  out << "*** End of statistics ***" << std::endl;

  bioEntry.printStats(out);
}

void TraceReplayer::getProgress(float &val) {
  if (max_io == 0) {
    // If I/O count is unlimited, use file pointer for fast progress calculation
    uint64_t ptr;

    {
      std::lock_guard<std::mutex> guard(m);
      ptr = file.tellg();
    }

    val = (float)ptr / fileSize;
  }
  else {
    // Use submitted I/O count in progress calculation
    // If trace file contains I/O requests smaller than max_io, progress value
    // cannot reach 1.0 (100%)
    val = (float)io_count / max_io;
  }
}

uint64_t TraceReplayer::mergeTime(std::smatch &match) {
  uint64_t tick = 0;
  bool valid = true;

  if (timeValids[0] && match.size() > groupID[ID_TIME_SEC]) {
    tick += strtoul(match[groupID[ID_TIME_SEC]].str().c_str(), nullptr, 10) *
            1000000000000ULL;
  }
  else if (timeValids[0]) {
    valid = false;
  }

  if (timeValids[1] && match.size() > groupID[ID_TIME_MS]) {
    tick += strtoul(match[groupID[ID_TIME_MS]].str().c_str(), nullptr, 10) *
            1000000000ULL;
  }
  else if (timeValids[1]) {
    valid = false;
  }

  if (timeValids[2] && match.size() > groupID[ID_TIME_US]) {
    tick += strtoul(match[groupID[ID_TIME_US]].str().c_str(), nullptr, 10) *
            1000000ULL;
  }
  else if (timeValids[2]) {
    valid = false;
  }

  if (timeValids[3] && match.size() > groupID[ID_TIME_NS]) {
    tick += strtoul(match[groupID[ID_TIME_NS]].str().c_str(), nullptr, 10) *
            1000ULL;
  }
  else if (timeValids[3]) {
    valid = false;
  }

  if (timeValids[4] && match.size() > groupID[ID_TIME_PS]) {
    tick += strtoul(match[groupID[ID_TIME_PS]].str().c_str(), nullptr, 10);
  }
  else if (timeValids[4]) {
    valid = false;
  }

  if (!valid) {
    SimpleSSD::panic("Time parse failed");
  }

  return tick;
}

BIL::BIO_TYPE TraceReplayer::getType(std::string type) {
  io_count++;

  switch (type[0]) {
    case 'r':
    case 'R':
      read_count++;

      return BIL::BIO_READ;
    case 'w':
    case 'W':
      write_count++;

      return BIL::BIO_WRITE;
    case 'f':
    case 'F':
      return BIL::BIO_FLUSH;
    case 't':
    case 'T':
    case 'd':
    case 'D':
      return BIL::BIO_TRIM;
  }

  return BIL::BIO_NUM;
}

void TraceReplayer::parseLine() {
  // printf("parseLine\n");
  std::string line;
  std::smatch match;

  // Read line
  while (true) {
    bool eof = false;

    {
      std::lock_guard<std::mutex> guard(m);

      eof = file.eof();
      std::getline(file, line);
    }

    if (eof) {
      reserveTermination = true;

      if (io_depth == 0) {
        // No on-the-fly I/O
        endCallback();
      }

      // printf("return from parseLine\n");

      return;
    }
    if (std::regex_match(line, match, regex)) {
      break;
    }
  }

  // Get time
  linedata.tick = mergeTime(match);

  // Fill BIO
  if (useLBAOffset) {
    linedata.offset = strtoul(match[groupID[ID_LBA_OFFSET]].str().c_str(),
                              nullptr, useHex ? 16 : 10) *
                      lbaSize;
  }
  else {
    linedata.offset = strtoul(match[groupID[ID_BYTE_OFFSET]].str().c_str(),
                              nullptr, useHex ? 16 : 10);
  }

  if (useLBALength) {
    linedata.length = strtoul(match[groupID[ID_LBA_LENGTH]].str().c_str(),
                              nullptr, useHex ? 16 : 10) *
                      lbaSize;
  }
  else {
    linedata.length = strtoul(match[groupID[ID_BYTE_LENGTH]].str().c_str(),
                              nullptr, useHex ? 16 : 10);
  }

  // printf("offset %lu, length %lu\n", linedata.offset, linedata.length);

  // This function increases I/O count
  linedata.type = getType(match[groupID[ID_OPERATION]].str());
}

void TraceReplayer::submitIO() {
  BIL::BIO bio;

  if (linedata.type == BIL::BIO_NUM) {
    SimpleSSD::panic("Unexpected request type.");
  }

  bio.callback = completionEvent;
  bio.id = io_count;
  bio.type = linedata.type;
  bio.offset = linedata.offset;
  bio.length = linedata.length;

  bioEntry.submitIO(bio);

  io_depth++;

  if ((max_io != 0 && io_count >= max_io)) {
    reserveTermination = true;

    return;
  }

  parseLine();

  if (reserveTermination) {
    return;
  }

  switch (mode) {
    case MODE_STRICT:
      engine.scheduleEvent(submitEvent, linedata.tick - firstTick + initTime);
      break;
    case MODE_ASYNC:
      rescheduleSubmit(submissionLatency);
      break;
    default:
      break;
  }
}

void TraceReplayer::iocallback(uint64_t) {
  io_depth--;

  if (reserveTermination) {
    // Everything is done
    if (io_depth == 0) {
      endCallback();
    }
  }

  if (mode == MODE_SYNC || nextIOIsSync) {
    // MODE_ASYNC submission blocked by I/O depth limitation
    // Let's submit here
    nextIOIsSync = false;

    rescheduleSubmit(submissionLatency + completionLatency);
  }
}

void TraceReplayer::rescheduleSubmit(uint64_t breakTime) {
  if (mode == MODE_ASYNC) {
    if (io_depth >= maxQueueDepth) {
      nextIOIsSync = true;

      return;
    }
  }
  else if (mode == MODE_STRICT) {
    return;
  }

  engine.scheduleEvent(submitEvent, engine.getCurrentTick() + breakTime);
}

}  // namespace IGL
