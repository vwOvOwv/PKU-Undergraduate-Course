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

#ifndef __SIM_DMA_INTERFACE__
#define __SIM_DMA_INTERFACE__

#include <cinttypes>
#include <functional>

namespace SimpleSSD {

typedef std::function<void(uint64_t, void *)> DMAFunction;

typedef struct _DMAContext {
  int counter;
  DMAFunction function;
  void *context;

  _DMAContext(DMAFunction &f) : counter(0), function(f), context(nullptr) {}
  _DMAContext(DMAFunction &f, void *c) : counter(0), function(f), context(c) {}
} DMAContext;

class DMAInterface {
 public:
  DMAInterface() {}
  virtual ~DMAInterface() {}

  virtual void dmaRead(uint64_t, uint64_t, uint8_t *, DMAFunction &,
                       void * = nullptr) = 0;
  virtual void dmaWrite(uint64_t, uint64_t, uint8_t *, DMAFunction &,
                        void * = nullptr) = 0;
};

}  // namespace SimpleSSD

#endif
