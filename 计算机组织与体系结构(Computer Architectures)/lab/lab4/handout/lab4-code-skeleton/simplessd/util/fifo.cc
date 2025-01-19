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

#include "util/fifo.hh"

#include "util/algorithm.hh"

namespace SimpleSSD {

FIFOEntry::FIFOEntry()
    : last(true),
      id(0),
      addr(0),
      size(0),
      buffer(nullptr),
      arrivedAt(0),
      insertBeginAt(0),
      insertEndAt(0),
      context(nullptr) {}

FIFOEntry::FIFOEntry(uint64_t a, uint64_t s, uint8_t *b, DMAFunction &f,
                     void *c)
    : last(true),
      id(0),
      addr(a),
      size(s),
      buffer(b),
      arrivedAt(getTick()),
      insertBeginAt(0),
      insertEndAt(0),
      func(f),
      context(c) {}

ReadEntry::ReadEntry() : id(0), insertEndAt(0), dmaEndAt(0), latency(0) {}

ReadEntry::ReadEntry(uint64_t a, uint64_t i, uint64_t d, uint64_t l)
    : id(a), insertEndAt(i), dmaEndAt(d), latency(l) {}

FIFO::Queue::Queue(uint64_t c)
    : capacity(c), usage(0), insertPending(false), transferPending(false) {}

FIFO::FIFO(DMAInterface *up, FIFOParam &p)
    : upstream(up),
      param(p),
      readQueue(p.rqSize),
      writeQueue(p.wqSize),
      counter(0) {
  // Sanity check
  if (param.transferUnit > param.rqSize || param.transferUnit > param.wqSize) {
    panic("Invalid transferUnit size");
  }

  // Set Transfer Unit Latency
  unitLatency = param.latency(param.transferUnit);

  // Allocate events and functions for each queue
  writeQueue.insertDone = allocate([this](uint64_t) { insertWriteDone(); });
  writeQueue.beginTransfer = allocate([this](uint64_t) { transferWrite(); });
  writeQueue.submitCompletion =
      allocate([this](uint64_t) { transferWriteDoneNext(); });
  writeQueue.transferDone = [this](uint64_t, void *) { transferWriteDone(); };
  readQueue.insertDone = allocate([this](uint64_t) { insertReadDone(); });
  readQueue.beginTransfer = allocate([this](uint64_t) { insertRead(); });
  readQueue.submitCompletion =
      allocate([this](uint64_t) { insertReadDoneNext(); });
  readQueue.transferDone = [this](uint64_t, void *) { transferReadDone(); };
}

FIFO::~FIFO() {}

std::list<ReadEntry>::iterator FIFO::find(uint64_t id) {
  auto iter = readCompletion.begin();

  for (; iter != readCompletion.end(); iter++) {
    if (iter->id == id) {
      break;
    }
  }

  return iter;
}

uint64_t FIFO::calcSize(uint64_t size, bool &b) {
  if (size < param.transferUnit) {
    b = true;

    return size;
  }

  return DIVCEIL(size, param.transferUnit) * param.transferUnit;
}

void FIFO::insertWrite() {
  auto iter = writeQueue.waitQueue.begin();
  bool smallerThanUnit = false;
  uint64_t now = getTick();
  uint64_t size = calcSize(iter->size, smallerThanUnit);

  if ((writeQueue.insertPending ||
       writeQueue.usage + size > writeQueue.capacity) &&
      size <= param.wqSize) {
    return;
  }

  // We need to split large request
  if (size > param.wqSize) {
    FIFOEntry copy = *iter;

    // copy is next one
    copy.last = true;
    copy.addr = iter->addr + param.transferUnit;
    copy.size = iter->size - param.transferUnit;
    copy.buffer = iter->buffer ? iter->buffer + param.transferUnit : nullptr;

    // iter is current one
    iter->last = false;
    iter->size = param.transferUnit;

    // Insert copy after iter
    writeQueue.waitQueue.insert(++iter, copy);

    // iter should first one
    iter = writeQueue.waitQueue.begin();
    size = calcSize(iter->size, smallerThanUnit);
  }

  writeQueue.insertPending = true;

  // Current item will be written to FIFO
  writeQueue.usage += size;
  iter->insertBeginAt = now;
  iter->insertEndAt = now + param.latency(size);

  // When insertion done, call handler
  schedule(writeQueue.insertDone, iter->insertEndAt);

  // Current item will be transferred after one transferUnit written to FIFO
  // If we alwritey scheduled, current item will be transffered after last one
  if (!scheduled(writeQueue.beginTransfer)) {
    schedule(writeQueue.beginTransfer,
             now + (smallerThanUnit ? param.latency(size) : unitLatency));
  }

  // Push current item to transferQueue
  writeQueue.transferQueue.push(*iter);
}

void FIFO::insertWriteDone() {
  // Pop last item
  writeQueue.waitQueue.pop_front();
  writeQueue.insertPending = false;

  // Insert next waiting item
  if (writeQueue.waitQueue.size() > 0) {
    insertWrite();
  }
}

void FIFO::transferWrite() {
  auto &iter = writeQueue.transferQueue.front();

  if (writeQueue.transferPending) {
    return;
  }

  writeQueue.transferPending = true;

  // Begin transfer
  upstream->dmaWrite(iter.addr, iter.size, iter.buffer,
                     writeQueue.transferDone);
}

void FIFO::transferWriteDone() {
  auto &iter = writeQueue.transferQueue.front();
  uint64_t now = getTick();
  uint64_t latency =
      (iter.size < param.transferUnit) ? param.latency(iter.size) : unitLatency;

  if (now >= iter.insertEndAt + latency) {  // Upstream <= downstream
    // Finish immediately
    transferWriteDoneNext();
  }
  else {  // upstream is faster than downstream
    // Should finish at insertedEndAt + upstreamLatency(transferUnit)
    // TODO: Applying unitLatency is not correct (more slower result)
    schedule(writeQueue.submitCompletion, iter.insertEndAt + latency);
  }
}

void FIFO::transferWriteDoneNext() {
  auto &iter = writeQueue.transferQueue.front();
  bool unused;
  uint64_t now = getTick();

  // Call handler
  if (iter.last) {
    iter.func(now, iter.context);
  }

  // Transfer done
  writeQueue.usage -= calcSize(iter.size, unused);
  writeQueue.transferPending = false;
  writeQueue.transferQueue.pop();

  // Do next transfer if we have
  if (writeQueue.transferQueue.size() > 0) {
    if (!scheduled(writeQueue.beginTransfer)) {
      transferWrite();
    }
  }

  // Do next insertion if we have
  if (writeQueue.waitQueue.size() > 0) {
    insertWrite();
  }
}

void FIFO::transferRead() {
  auto iter = readQueue.waitQueue.begin();
  bool smallerThanUnit = false;
  uint64_t size = calcSize(iter->size, smallerThanUnit);

  if ((readQueue.transferPending ||
       readQueue.usage + size > readQueue.capacity) &&
      size <= param.rqSize) {
    return;
  }

  // We need to split large request
  if (size > param.rqSize) {
    FIFOEntry copy = *iter;

    // copy is next one
    copy.last = true;
    copy.addr = iter->addr + param.transferUnit;
    copy.size = iter->size - param.transferUnit;
    copy.buffer = iter->buffer ? iter->buffer + param.transferUnit : nullptr;

    // iter is current one
    iter->last = false;
    iter->size = param.transferUnit;

    // Insert copy after iter
    readQueue.waitQueue.insert(++iter, copy);

    // iter should first one
    iter = readQueue.waitQueue.begin();
    size = calcSize(iter->size, smallerThanUnit);
  }

  readQueue.transferPending = true;

  // Set ID
  iter->id = counter++;

  // Current item will be requested to be read
  readQueue.usage += size;

  // When all data is read to FIFO, call handler
  upstream->dmaRead(iter->addr, iter->size, iter->buffer,
                    readQueue.transferDone);

  if (!scheduled(readQueue.beginTransfer)) {
    schedule(readQueue.beginTransfer,
             getTick() + (smallerThanUnit ? param.latency(size) : unitLatency));
  }

  // Push current item to transferQueue
  readQueue.transferQueue.push(*iter);
}

void FIFO::transferReadDone() {
  auto &iter = readQueue.waitQueue.front();
  uint64_t latency =
      (iter.size < param.transferUnit) ? param.latency(iter.size) : unitLatency;

  // Save DMA end time
  auto comp = find(iter.id);

  if (comp == readCompletion.end()) {
    readCompletion.push_back(ReadEntry(iter.id, 0, getTick(), latency));
  }
  else {
    comp->dmaEndAt = getTick();

    insertReadDoneMerge(comp);
  }

  // Pop last item
  readQueue.waitQueue.pop_front();
  readQueue.transferPending = false;

  // Transfer next waiting item
  if (readQueue.waitQueue.size() > 0) {
    transferRead();
  }
}

void FIFO::insertRead() {
  auto &iter = readQueue.transferQueue.front();
  bool unused;
  uint64_t now = getTick();

  if (readQueue.insertPending) {
    return;
  }

  readQueue.insertPending = true;

  // Begin insertion
  iter.insertBeginAt = now;
  iter.insertEndAt = now + param.latency(calcSize(iter.size, unused));

  // When insertion done, call handler
  if (!scheduled(readQueue.insertDone)) {
    schedule(readQueue.insertDone, iter.insertEndAt);
  }
}

void FIFO::insertReadDone() {
  auto &iter = readQueue.transferQueue.front();
  uint64_t latency =
      (iter.size < param.transferUnit) ? param.latency(iter.size) : unitLatency;

  auto comp = find(iter.id);

  if (comp == readCompletion.end()) {
    readCompletion.push_back(ReadEntry(iter.id, getTick(), 0, latency));
  }
  else {
    insertReadDoneMerge(comp);
  }
}

void FIFO::insertReadDoneMerge(std::list<ReadEntry>::iterator comp) {
  uint64_t now = getTick();

  if (now >= comp->dmaEndAt + comp->latency) {  // Upstream <= downstream
    // Finish immediately
    insertReadDoneNext();
  }
  else {  // upstream is faster than downstream
    // Should finish at insertedEndAt + upstreamLatency(transferUnit)
    // TODO: Applying unitLatency is not correct (more slower result)
    schedule(readQueue.submitCompletion, comp->dmaEndAt + comp->latency);
  }

  readCompletion.erase(comp);
}

void FIFO::insertReadDoneNext() {
  auto &iter = readQueue.transferQueue.front();
  bool unused;
  uint64_t now = getTick();

  // Call handler
  if (iter.last) {
    iter.func(now, iter.context);
  }

  // Insert done
  readQueue.usage -= calcSize(iter.size, unused);
  readQueue.insertPending = false;
  readQueue.transferQueue.pop();

  // Do next insert if we have
  if (readQueue.transferQueue.size() > 0) {
    if (!scheduled(readQueue.beginTransfer)) {
      insertRead();
    }
  }

  // Do next transfer if we have
  if (readQueue.waitQueue.size() > 0) {
    transferRead();
  }
}

void FIFO::dmaRead(uint64_t addr, uint64_t size, uint8_t *buffer,
                   DMAFunction &func, void *context) {
  if (size == 0) {
    warn("FIFO: zero-size DMA read request. Ignore.");

    return;
  }

  readQueue.waitQueue.push_back(FIFOEntry(addr, size, buffer, func, context));

  transferRead();
}

void FIFO::dmaWrite(uint64_t addr, uint64_t size, uint8_t *buffer,
                    DMAFunction &func, void *context) {
  if (size == 0) {
    warn("FIFO: zero-size DMA write request. Ignore.");

    return;
  }

  writeQueue.waitQueue.push_back(FIFOEntry(addr, size, buffer, func, context));

  insertWrite();
}

}  // namespace SimpleSSD
