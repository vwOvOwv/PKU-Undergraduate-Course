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

#include "ftl/common/block.hh"

#include <algorithm>
#include <cassert>
#include <cstring>

namespace SimpleSSD {

namespace FTL {

Block::Block(uint32_t blockIdx, uint32_t count, uint32_t ioUnit)
    : idx(blockIdx),
      pageCount(count),
      ioUnitInPage(ioUnit),
      pValidBits(nullptr),
      pErasedBits(nullptr),
      pLPNs(nullptr),
      ppLPNs(nullptr),
      lastAccessed(0),
      eraseCount(0) {
  if (ioUnitInPage == 1) {
    pValidBits = new Bitset(pageCount);
    pErasedBits = new Bitset(pageCount);

    pLPNs = (uint64_t *)calloc(pageCount, sizeof(uint64_t));
  }
  else if (ioUnitInPage > 1) {
    Bitset copy(ioUnitInPage);

    validBits = std::vector<Bitset>(pageCount, copy);
    erasedBits = std::vector<Bitset>(pageCount, copy);

    ppLPNs = (uint64_t **)calloc(pageCount, sizeof(uint64_t *));

    for (uint32_t i = 0; i < pageCount; i++) {
      ppLPNs[i] = (uint64_t *)calloc(ioUnitInPage, sizeof(uint64_t));
    }
  }
  else {
    panic("Invalid I/O unit in page");
  }

  // C-style allocation
  pNextWritePageIndex = (uint32_t *)calloc(ioUnitInPage, sizeof(uint32_t));

  erase();
  eraseCount = 0;
}

Block::Block(const Block &old)
    : Block(old.idx, old.pageCount, old.ioUnitInPage) {
  if (ioUnitInPage == 1) {
    *pValidBits = *old.pValidBits;
    *pErasedBits = *old.pErasedBits;

    memcpy(pLPNs, old.pLPNs, pageCount * sizeof(uint64_t));
  }
  else {
    validBits = old.validBits;
    erasedBits = old.erasedBits;

    for (uint32_t i = 0; i < pageCount; i++) {
      memcpy(ppLPNs[i], old.ppLPNs[i], ioUnitInPage * sizeof(uint64_t));
    }
  }

  memcpy(pNextWritePageIndex, old.pNextWritePageIndex,
         ioUnitInPage * sizeof(uint32_t));

  eraseCount = old.eraseCount;
}

Block::Block(Block &&old) noexcept
    : idx(std::move(old.idx)),
      pageCount(std::move(old.pageCount)),
      ioUnitInPage(std::move(old.ioUnitInPage)),
      pNextWritePageIndex(std::move(old.pNextWritePageIndex)),
      pValidBits(std::move(old.pValidBits)),
      pErasedBits(std::move(old.pErasedBits)),
      pLPNs(std::move(old.pLPNs)),
      validBits(std::move(old.validBits)),
      erasedBits(std::move(old.erasedBits)),
      ppLPNs(std::move(old.ppLPNs)),
      lastAccessed(std::move(old.lastAccessed)),
      eraseCount(std::move(old.eraseCount)) {
  // TODO Use std::exchange to set old value to null (C++14)
  old.idx = 0;
  old.pageCount = 0;
  old.ioUnitInPage = 0;
  old.pNextWritePageIndex = nullptr;
  old.pValidBits = nullptr;
  old.pErasedBits = nullptr;
  old.pLPNs = nullptr;
  old.ppLPNs = nullptr;
  old.lastAccessed = 0;
  old.eraseCount = 0;
}

Block::~Block() {
  free(pNextWritePageIndex);
  free(pLPNs);

  delete pValidBits;
  delete pErasedBits;

  if (ppLPNs) {
    for (uint32_t i = 0; i < pageCount; i++) {
      free(ppLPNs[i]);
    }

    free(ppLPNs);
  }

  pNextWritePageIndex = nullptr;
  pLPNs = nullptr;
  pValidBits = nullptr;
  pErasedBits = nullptr;
  ppLPNs = nullptr;
}

Block &Block::operator=(const Block &rhs) {
  if (this != &rhs) {
    this->~Block();
    *this = Block(rhs);  // Call copy constructor
  }

  return *this;
}

Block &Block::operator=(Block &&rhs) {
  if (this != &rhs) {
    this->~Block();

    idx = std::move(rhs.idx);
    pageCount = std::move(rhs.pageCount);
    ioUnitInPage = std::move(rhs.ioUnitInPage);
    pNextWritePageIndex = std::move(rhs.pNextWritePageIndex);
    pValidBits = std::move(rhs.pValidBits);
    pErasedBits = std::move(rhs.pErasedBits);
    pLPNs = std::move(rhs.pLPNs);
    validBits = std::move(rhs.validBits);
    erasedBits = std::move(rhs.erasedBits);
    ppLPNs = std::move(rhs.ppLPNs);
    lastAccessed = std::move(rhs.lastAccessed);
    eraseCount = std::move(rhs.eraseCount);

    rhs.pNextWritePageIndex = nullptr;
    rhs.pValidBits = nullptr;
    rhs.pErasedBits = nullptr;
    rhs.pLPNs = nullptr;
    rhs.ppLPNs = nullptr;
    rhs.lastAccessed = 0;
    rhs.eraseCount = 0;
  }

  return *this;
}

uint32_t Block::getBlockIndex() const {
  return idx;
}

uint64_t Block::getLastAccessedTime() {
  return lastAccessed;
}

uint32_t Block::getEraseCount() {
  return eraseCount;
}

uint32_t Block::getValidPageCount() {
  uint32_t ret = 0;

  if (ioUnitInPage == 1) {
    ret = pValidBits->count();
  }
  else {
    for (auto &iter : validBits) {
      if (iter.any()) {
        ret++;
      }
    }
  }

  return ret;
}

uint32_t Block::getValidPageCountRaw() {
  uint32_t ret = 0;

  if (ioUnitInPage == 1) {
    // Same as getValidPageCount()
    ret = pValidBits->count();
  }
  else {
    for (auto &iter : validBits) {
      ret += iter.count();
    }
  }

  return ret;
}

uint32_t Block::getDirtyPageCount() {
  uint32_t ret = 0;

  if (ioUnitInPage == 1) {
    ret = (~(*pValidBits | *pErasedBits)).count();
  }
  else {
    for (uint32_t i = 0; i < pageCount; i++) {
      // Dirty: Valid(false), Erased(false)
      if ((~(validBits.at(i) | erasedBits.at(i))).any()) {
        ret++;
      }
    }
  }

  return ret;
}

uint32_t Block::getNextWritePageIndex() {
  uint32_t idx = 0;

  for (uint32_t i = 0; i < ioUnitInPage; i++) {
    if (idx < pNextWritePageIndex[i]) {
      idx = pNextWritePageIndex[i];
    }
  }

  return idx;
}

uint32_t Block::getNextWritePageIndex(uint32_t idx) {
  return pNextWritePageIndex[idx];
}

bool Block::getPageInfo(uint32_t pageIndex, std::vector<uint64_t> &lpn,
                        Bitset &map) {
  if (ioUnitInPage == 1 && map.size() == 1) {
    map.set();
    lpn = std::vector<uint64_t>(1, pLPNs[pageIndex]);
  }
  else if (map.size() == ioUnitInPage) {
    map = validBits.at(pageIndex);
    lpn = std::vector<uint64_t>(ppLPNs[pageIndex],
                                ppLPNs[pageIndex] + ioUnitInPage);
  }
  else {
    panic("I/O map size mismatch");
  }

  return map.any();
}

bool Block::read(uint32_t pageIndex, uint32_t idx, uint64_t tick) {
  bool read = false;

  if (ioUnitInPage == 1 && idx == 0) {
    read = pValidBits->test(pageIndex);
  }
  else if (idx < ioUnitInPage) {
    read = validBits.at(pageIndex).test(idx);
  }
  else {
    panic("I/O map size mismatch");
  }

  if (read) {
    lastAccessed = tick;
  }

  return read;
}

bool Block::write(uint32_t pageIndex, uint64_t lpn, uint32_t idx,
                  uint64_t tick) {
  bool write = false;

  if (ioUnitInPage == 1 && idx == 0) {
    write = pErasedBits->test(pageIndex);
  }
  else if (idx < ioUnitInPage) {
    write = erasedBits.at(pageIndex).test(idx);
  }
  else {
    panic("I/O map size mismatch");
  }

  if (write) {
    // TODO: check if this is necessary
    // if (pageIndex < pNextWritePageIndex[idx]) {
    //   panic("Write to block should sequential");
    // }

    lastAccessed = tick;

    if (ioUnitInPage == 1) {
      pErasedBits->reset(pageIndex);
      pValidBits->set(pageIndex);

      pLPNs[pageIndex] = lpn;
    }
    else {
      erasedBits.at(pageIndex).reset(idx);
      validBits.at(pageIndex).set(idx);

      ppLPNs[pageIndex][idx] = lpn;
    }

    pNextWritePageIndex[idx] = pageIndex + 1;
  }
  else {
    panic("Write to non erased page");
  }

  return write;
}

void Block::erase() {
  if (ioUnitInPage == 1) {
    pValidBits->reset();
    pErasedBits->set();
  }
  else {
    for (auto &iter : validBits) {
      iter.reset();
    }
    for (auto &iter : erasedBits) {
      iter.set();
    }
  }

  memset(pNextWritePageIndex, 0, sizeof(uint32_t) * ioUnitInPage);

  eraseCount++;
}

bool Block::checkValid(uint32_t pageIndex, uint32_t idx) {
  if (ioUnitInPage == 1 && idx == 0) {
    return pValidBits->test(pageIndex);
  }

  panic("I/O map size mismatch");

  return false;
}

bool Block::checkErase(uint32_t pageIndex, uint32_t idx) {
  if (ioUnitInPage == 1 && idx == 0) {
    return pErasedBits->test(pageIndex);
  }

  panic("I/O map size mismatch");

  return false;
}

uint64_t Block::getLPN(uint32_t pageIndex, uint32_t idx) {
  if (ioUnitInPage == 1 && idx == 0) {
    return pLPNs[pageIndex];
  }

  panic("I/O map size mismatch");

  return 0;
}

uint64_t Block::getNextOffset() {
  uint64_t lastOffset = pNextWritePageIndex[0];

  for (uint32_t i = 0; i < lastOffset; i++) {
    assert(pValidBits->test(i));
  }

  return lastOffset;
}

void Block::invalidate(uint32_t pageIndex, uint32_t idx) {
  if (ioUnitInPage == 1) {
    pValidBits->reset(pageIndex);
  }
  else {
    validBits.at(pageIndex).reset(idx);
  }
}

}  // namespace FTL

}  // namespace SimpleSSD
