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

#ifndef __HIL_NVME_QUEUE__
#define __HIL_NVME_QUEUE__

#include "hil/nvme/def.hh"
#include "hil/nvme/dma.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

typedef union _SQEntry {
  uint8_t data[64];
  struct {
    struct {
      uint8_t opcode;
      uint8_t fuse;
      uint16_t commandID;
    } dword0;
    uint32_t namespaceID;
    uint32_t reserved1;
    uint32_t reserved2;
    uint64_t metadata;
    uint64_t data1;
    uint64_t data2;
    uint32_t dword10;
    uint32_t dword11;
    uint32_t dword12;
    uint32_t dword13;
    uint32_t dword14;
    uint32_t dword15;
  };

  _SQEntry();
} SQEntry;

typedef union _CQEntry {
  uint8_t data[16];
  struct {
    uint32_t dword0;
    uint32_t reserved;
    struct {
      uint16_t sqHead;
      uint16_t sqID;
    } dword2;
    struct {
      uint16_t commandID;
      uint16_t status;
    } dword3;
  };

  _CQEntry();
} CQEntry;

typedef struct _SQEntryWrapper {
  SQEntry entry;

  uint16_t sqID;
  uint16_t cqID;
  uint16_t sqHead;
  uint16_t sqUID;  //!< HEAD - 1

  bool useSGL;

  _SQEntryWrapper(SQEntry &, uint16_t, uint16_t, uint16_t, uint16_t);
} SQEntryWrapper;

typedef struct _CQEntryWrapper {
  CQEntry entry;

  uint64_t submitAt;

  uint16_t cqID;   //!< sqID at entry.dword2
  uint16_t sqUID;  //!< Head - 1

  _CQEntryWrapper(SQEntryWrapper &);
  void makeStatus(bool, bool, STATUS_CODE_TYPE, int);
} CQEntryWrapper;

class Queue {
 protected:
  uint16_t id;

  uint16_t head;
  uint16_t tail;

  uint16_t size;
  uint64_t stride;

  DMAInterface *base;

 public:
  Queue(uint16_t, uint16_t);
  ~Queue();

  uint16_t getID();
  uint16_t getItemCount();
  uint16_t getHead();
  uint16_t getTail();
  uint16_t getSize();
  void setBase(DMAInterface *, uint64_t);
};

class CQueue : public Queue {
 protected:
  bool ien;
  bool phase;
  uint16_t interruptVector;

 public:
  CQueue(uint16_t, bool, uint16_t, uint16_t);

  void setData(CQEntry *, DMAFunction, void *);
  uint16_t incHead();
  void setHead(uint16_t);
  bool interruptEnabled();
  uint16_t getInterruptVector();
};

class SQueue : public Queue {
 protected:
  uint16_t cqID;
  uint8_t priority;

 public:
  SQueue(uint16_t, uint8_t, uint16_t, uint16_t);

  uint16_t getCQID();
  void setTail(uint16_t);
  void getData(SQEntry *, DMAFunction, void *);
  uint8_t getPriority();
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD

#endif
