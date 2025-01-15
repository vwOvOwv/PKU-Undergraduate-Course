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

#include "hil/nvme/queue.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

SQEntry::_SQEntry() {
  memset(data, 0, 64);
}

CQEntry::_CQEntry() {
  memset(data, 0, 16);
}

SQEntryWrapper::_SQEntryWrapper(SQEntry &sqdata, uint16_t sqid, uint16_t cqid,
                                uint16_t sqhead, uint16_t squid)
    : entry(sqdata),
      sqID(sqid),
      cqID(cqid),
      sqHead(sqhead),
      sqUID(squid),
      useSGL(true) {
  if ((sqdata.dword0.fuse >> 6) == 0x00) {
    useSGL = false;
  }
}

CQEntryWrapper::_CQEntryWrapper(SQEntryWrapper &sqew) {
  cqID = sqew.cqID;
  sqUID = sqew.sqUID;
  entry.dword2.sqHead = sqew.sqHead;
  entry.dword2.sqID = sqew.sqID;
  entry.dword3.commandID = sqew.entry.dword0.commandID;
}

void CQEntryWrapper::makeStatus(bool dnr, bool more, STATUS_CODE_TYPE sct,
                                int sc) {
  entry.dword3.status = 0x0000;

  entry.dword3.status = ((dnr ? 1 : 0) << 15);
  entry.dword3.status |= ((more ? 1 : 0) << 14);
  entry.dword3.status |= ((sct & 0x07) << 9);
  entry.dword3.status |= ((sc & 0xFF) << 1);
}

Queue::Queue(uint16_t qid, uint16_t length)
    : id(qid), head(0), tail(0), size(length), stride(0), base(nullptr) {}

Queue::~Queue() {
  if (base) {
    delete base;
  }
}

uint16_t Queue::getID() {
  return id;
}

uint16_t Queue::getItemCount() {
  if (tail >= head) {
    return tail - head;
  }
  else {
    return (size - head) + tail;
  }
}

uint16_t Queue::getHead() {
  return head;
}

uint16_t Queue::getTail() {
  return tail;
}

uint16_t Queue::getSize() {
  return size;
}

void Queue::setBase(DMAInterface *p, uint64_t s) {
  base = p;
  stride = s;
}

CQueue::CQueue(uint16_t iv, bool en, uint16_t qid, uint16_t size)
    : Queue(qid, size), ien(en), phase(true), interruptVector(iv) {}

void CQueue::setData(CQEntry *entry, DMAFunction func, void *context) {
  if (entry) {
    // Set phase
    entry->dword3.status &= 0xFFFE;
    entry->dword3.status |= (phase ? 0x0001 : 0x0000);

    // Write entry
    base->write(tail * stride, 0x10, entry->data, func, context);

    // Increase tail
    tail++;

    if (tail == size) {
      tail = 0;
      phase = !phase;
    }

    if (head == tail) {
      panic("Completion queue overflow");
    }
  }
}

uint16_t CQueue::incHead() {
  head++;

  if (head == size) {
    head = 0;
  }

  return head;
}

void CQueue::setHead(uint16_t newHead) {
  head = newHead;
}

bool CQueue::interruptEnabled() {
  return ien;
}

uint16_t CQueue::getInterruptVector() {
  return interruptVector;
}

SQueue::SQueue(uint16_t cqid, uint8_t pri, uint16_t qid, uint16_t size)
    : Queue(qid, size), cqID(cqid), priority(pri) {}

uint16_t SQueue::getCQID() {
  return cqID;
}

void SQueue::setTail(uint16_t newTail) {
  tail = newTail;
}

void SQueue::getData(SQEntry *entry, DMAFunction func, void *context) {
  if (entry && head != tail) {
    // Read entry
    base->read(head * stride, 0x40, entry->data, func, context);

    // Increase head
    head++;

    if (head == size) {
      head = 0;
    }
  }
}

uint8_t SQueue::getPriority() {
  return priority;
}

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD
