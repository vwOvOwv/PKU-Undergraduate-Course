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

#ifndef __HIL_NVME_DMA__
#define __HIL_NVME_DMA__

#include <vector>

#include "hil/nvme/config.hh"
#include "hil/nvme/def.hh"
#include "sim/dma_interface.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

class Controller;

typedef struct {
  ConfigReader *pConfigReader;
  DMAInterface *pInterface;
  uint64_t memoryPageSize;
  uint8_t memoryPageSizeOrder;
  uint16_t maxQueueEntry;
} ConfigData;

class DMAInterface {
 protected:
  SimpleSSD::DMAInterface *pInterface;
  DMAFunction initFunction;
  uint64_t callCounter;
  void *context;

  Event immediateEvent;

  DMAFunction dmaHandler;
  static void commonDMAHandler(uint64_t, void *);

 public:
  DMAInterface(ConfigData &, DMAFunction &, void *);
  virtual ~DMAInterface();

  virtual void read(uint64_t, uint64_t, uint8_t *, DMAFunction &,
                    void * = nullptr) = 0;
  virtual void write(uint64_t, uint64_t, uint8_t *, DMAFunction &,
                     void * = nullptr) = 0;
};

struct DMAInitContext {
  DMAInterface *pThis;
  uint64_t totalSize;
  uint64_t currentSize;
  uint8_t *buffer;
};

struct PRP {
  uint64_t addr;
  uint64_t size;

  PRP();
  PRP(uint64_t, uint64_t);
};

class PRPList : public DMAInterface {
 private:
  std::vector<PRP> prpList;
  uint64_t totalSize;
  uint64_t pagesize;

  void getPRPListFromPRP(uint64_t, uint64_t);
  uint64_t getPRPSize(uint64_t);

 public:
  PRPList(ConfigData &, DMAFunction &, void *, uint64_t, uint64_t, uint64_t);
  PRPList(ConfigData &, DMAFunction &, void *, uint64_t, uint64_t, bool);
  ~PRPList();

  void read(uint64_t, uint64_t, uint8_t *, DMAFunction &,
            void * = nullptr) override;
  void write(uint64_t, uint64_t, uint8_t *, DMAFunction &,
             void * = nullptr) override;
};

union SGLDescriptor {
  uint8_t data[16];
  struct {
    uint64_t address;
    uint32_t length;
    uint8_t reserved[3];
    uint8_t id;
  };

  SGLDescriptor();
};

struct Chunk {
  uint64_t addr;
  uint32_t length;

  bool ignore;

  Chunk();
  Chunk(uint64_t, uint32_t, bool);
};

class SGL : public DMAInterface {
 private:
  std::vector<Chunk> chunkList;
  uint64_t totalSize;

  void parseSGLDescriptor(SGLDescriptor &);
  void parseSGLSegment(uint64_t, uint32_t);

 public:
  SGL(ConfigData &, DMAFunction &, void *, uint64_t, uint64_t);
  ~SGL();

  void read(uint64_t, uint64_t, uint8_t *, DMAFunction &,
            void * = nullptr) override;
  void write(uint64_t, uint64_t, uint8_t *, DMAFunction &,
             void * = nullptr) override;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD

#endif
