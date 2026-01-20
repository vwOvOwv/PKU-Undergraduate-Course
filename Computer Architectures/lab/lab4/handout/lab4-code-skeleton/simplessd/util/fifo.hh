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

#ifndef __UTIL_FIFO__
#define __UTIL_FIFO__

#include <list>
#include <queue>

#include "sim/dma_interface.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

typedef std::function<uint64_t(uint64_t)> LatencyFunction;

struct FIFOParam {
  uint64_t rqSize;          //!< Read Queue size
  uint64_t wqSize;          //!< Write Queue size
  uint64_t transferUnit;    //!< Transfer unit used to interleave DMA
  LatencyFunction latency;  //!< Downstream latency function
};

struct FIFOEntry {
  bool last;  //!< This is last entry of splitted request
  uint64_t id;

  uint64_t addr;
  uint64_t size;
  uint8_t *buffer;

  uint64_t arrivedAt;      //!< Request arrived to FIFO
  uint64_t insertBeginAt;  //!< Request is now begin to written on FIFO
  uint64_t insertEndAt;    //!< Request is fully written on FIFO

  DMAFunction func;
  void *context;

  FIFOEntry();
  FIFOEntry(uint64_t, uint64_t, uint8_t *, DMAFunction &, void *);
};

struct ReadEntry {
  uint64_t id;
  uint64_t insertEndAt;
  uint64_t dmaEndAt;
  uint64_t latency;

  ReadEntry();
  ReadEntry(uint64_t, uint64_t, uint64_t, uint64_t);
};

class FIFO : public DMAInterface {
 protected:
  class Queue {
   public:
    uint64_t capacity;
    uint64_t usage;

    std::list<FIFOEntry> waitQueue;
    std::queue<FIFOEntry> transferQueue;

    Event insertDone;
    Event beginTransfer;
    Event submitCompletion;
    DMAFunction transferDone;

    bool insertPending;
    bool transferPending;

    Queue(uint64_t);
  };

  DMAInterface *upstream;
  FIFOParam param;
  uint64_t unitLatency;

  Queue readQueue;
  Queue writeQueue;

  uint64_t counter;
  std::list<ReadEntry> readCompletion;

  std::list<ReadEntry>::iterator find(uint64_t);

  uint64_t calcSize(uint64_t, bool &);

  // Write
  void insertWrite();
  void insertWriteDone();
  void transferWrite();
  void transferWriteDone();
  void transferWriteDoneNext();

  // Read
  void transferRead();
  void transferReadDone();
  void insertRead();
  void insertReadDone();
  void insertReadDoneMerge(std::list<ReadEntry>::iterator);
  void insertReadDoneNext();

 public:
  FIFO() = delete;
  FIFO(DMAInterface *, FIFOParam &);
  FIFO(const FIFO &) = delete;
  ~FIFO();

  void dmaRead(uint64_t, uint64_t, uint8_t *, DMAFunction &,
               void * = nullptr) override;
  void dmaWrite(uint64_t, uint64_t, uint8_t *, DMAFunction &,
                void * = nullptr) override;
};

}  // namespace SimpleSSD

#endif
