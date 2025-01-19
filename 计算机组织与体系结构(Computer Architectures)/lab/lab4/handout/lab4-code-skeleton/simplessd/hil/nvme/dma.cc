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

#include "hil/nvme/dma.hh"

#include "hil/nvme/controller.hh"
#include "util/algorithm.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

DMAInterface::DMAInterface(ConfigData &cfg, DMAFunction &f, void *c)
    : pInterface(cfg.pInterface),
      initFunction(f),
      callCounter(0),
      context(c),
      dmaHandler(commonDMAHandler) {
  immediateEvent =
      allocate([this](uint64_t now) { initFunction(now, context); });
}

DMAInterface::~DMAInterface() {}

void DMAInterface::commonDMAHandler(uint64_t now, void *context) {
  DMAContext *pContext = (DMAContext *)context;

  pContext->counter--;

  if (pContext->counter == 0) {
    pContext->function(now, pContext->context);
    delete pContext;
  }
}

PRP::PRP() : addr(0), size(0) {}

PRP::PRP(uint64_t address, uint64_t s) : addr(address), size(s) {}

PRPList::PRPList(ConfigData &cfg, DMAFunction &f, void *c, uint64_t prp1,
                 uint64_t prp2, uint64_t size)
    : DMAInterface(cfg, f, c), totalSize(size), pagesize(cfg.memoryPageSize) {
  bool immediate = true;
  uint64_t prp1Size = getPRPSize(prp1);
  uint64_t prp2Size = getPRPSize(prp2);

  // Determine PRP1 and PRP2
  if (totalSize <= pagesize) {
    if (totalSize <= prp1Size) {
      // PRP1 is PRP pointer, PRP2 is not used
      prpList.push_back(PRP(prp1, totalSize));
    }
    else {
      // PRP1 is PRP pointer, PRP2 is PRP pointer
      prpList.push_back(PRP(prp1, prp1Size));
      prpList.push_back(PRP(prp2, prp2Size));

      if (prp1Size + prp2Size < totalSize) {
        panic("prp_list: Invalid DPTR size");
      }
    }
  }
  else if (totalSize <= pagesize * 2) {
    if (prp1Size == pagesize) {
      // PRP1 is PRP pointer, PRP2 is PRP pointer
      prpList.push_back(PRP(prp1, prp1Size));
      prpList.push_back(PRP(prp2, prp2Size));

      if (prp1Size + prp2Size < totalSize) {
        panic("prp_list: Invalid DPTR size");
      }
    }
    else {
      immediate = false;

      // PRP1 is PRP pointer, PRP2 is PRP list
      prpList.push_back(PRP(prp1, prp1Size));
      getPRPListFromPRP(prp2, totalSize - prp1Size);
    }
  }
  else {
    immediate = false;

    // PRP1 is PRP pointer, PRP2 is PRP list
    prpList.push_back(PRP(prp1, prp1Size));
    getPRPListFromPRP(prp2, totalSize - prp1Size);
  }

  if (immediate) {
    schedule(immediateEvent, getTick());
  }
}

PRPList::PRPList(ConfigData &cfg, DMAFunction &f, void *c, uint64_t base,
                 uint64_t size, bool cont)
    : DMAInterface(cfg, f, c), totalSize(size), pagesize(cfg.memoryPageSize) {
  if (cont) {
    prpList.push_back(PRP(base, size));

    schedule(immediateEvent, getTick());
  }
  else {
    getPRPListFromPRP(base, size);
  }
}

PRPList::~PRPList() {}

void PRPList::getPRPListFromPRP(uint64_t base, uint64_t size) {
  static DMAFunction doRead = [](uint64_t now, void *context) {
    DMAInitContext *pContext = (DMAInitContext *)context;
    PRPList *pThis = (PRPList *)pContext->pThis;
    uint64_t listPRP;
    uint64_t listPRPSize;
    uint64_t currentSize = 0;

    pThis->callCounter--;

    for (size_t i = 0; i < pContext->currentSize; i += 8) {
      listPRP = *((uint64_t *)(pContext->buffer + i));
      listPRPSize = pThis->getPRPSize(listPRP);
      currentSize += listPRPSize;

      if (listPRP == 0) {
        panic("prp_list: Invalid PRP in PRP List");
      }

      pThis->prpList.push_back(PRP(listPRP, listPRPSize));

      if (currentSize >= pContext->totalSize) {
        break;
      }
    }

    free(pContext->buffer);

    if (currentSize < pContext->totalSize) {
      // PRP list ends but size is not full
      // Last item of PRP list is pointer of another PRP
      // list
      listPRP = pThis->prpList.back().addr;
      pThis->prpList.pop_back();

      pThis->getPRPListFromPRP(listPRP, pContext->totalSize - currentSize);
    }

    if (pThis->callCounter == 0) {
      // Everything is done
      pThis->initFunction(now, pThis->context);
    }

    delete pContext;
  };

  DMAInitContext *pContext = new DMAInitContext();

  callCounter++;

  pContext->pThis = this;
  pContext->totalSize = size;

  // Get PRP size
  pContext->currentSize = getPRPSize(base);

  // Allocate buffer
  pContext->buffer = (uint8_t *)malloc(pContext->currentSize);

  if (pContext->buffer) {
    // Read PRP
    CPUContext *pCPU = new CPUContext(doRead, pContext, CPU::NVME__PRPLIST,
                                      CPU::GET_PRPLIST_FROM_PRP);
    pInterface->dmaRead(base, pContext->currentSize, pContext->buffer,
                        cpuHandler, pCPU);
  }
  else {
    delete pContext;

    panic("prp_list: ENOMEM");
  }
}

uint64_t PRPList::getPRPSize(uint64_t addr) {
  return pagesize - (addr & (pagesize - 1));
}

void PRPList::read(uint64_t offset, uint64_t length, uint8_t *buffer,
                   DMAFunction &func, void *context) {
  DMAFunction doRead = [this, offset, length, buffer](uint64_t, void *context) {
    uint64_t totalRead = 0;
    uint64_t currentOffset = 0;
    uint64_t read;
    bool begin = false;

    DMAContext *readContext = (DMAContext *)context;

    for (auto &iter : prpList) {
      if (begin) {
        read = MIN(iter.size, length - totalRead);
        readContext->counter++;
        pInterface->dmaRead(iter.addr, read, buffer ? buffer + totalRead : NULL,
                            dmaHandler, readContext);
        totalRead += read;

        if (totalRead == length) {
          break;
        }
      }

      if (!begin && currentOffset + iter.size > offset) {
        begin = true;
        totalRead = offset - currentOffset;
        read = MIN(iter.size - totalRead, length);
        readContext->counter++;
        pInterface->dmaRead(iter.addr + totalRead, read, buffer, dmaHandler,
                            readContext);
        totalRead = read;
      }

      currentOffset += iter.size;
    }

    if (readContext->counter == 0) {
      readContext->counter = 1;

      dmaHandler(getTick(), readContext);
    }
  };

  DMAContext *readContext = new DMAContext(func, context);

  execute(CPU::NVME__PRPLIST, CPU::READ, doRead, readContext);
}

void PRPList::write(uint64_t offset, uint64_t length, uint8_t *buffer,
                    DMAFunction &func, void *context) {
  DMAFunction doWrite = [this, offset, length, buffer](uint64_t,
                                                       void *context) {
    uint64_t totalWritten = 0;
    uint64_t currentOffset = 0;
    uint64_t written;
    bool begin = false;

    DMAContext *writeContext = (DMAContext *)context;

    for (auto &iter : prpList) {
      if (begin) {
        written = MIN(iter.size, length - totalWritten);
        writeContext->counter++;
        pInterface->dmaWrite(iter.addr, written,
                             buffer ? buffer + totalWritten : NULL, dmaHandler,
                             writeContext);
        totalWritten += written;

        if (totalWritten == length) {
          break;
        }
      }

      if (!begin && currentOffset + iter.size > offset) {
        begin = true;
        totalWritten = offset - currentOffset;
        written = MIN(iter.size - totalWritten, length);
        writeContext->counter++;
        pInterface->dmaWrite(iter.addr + totalWritten, written, buffer,
                             dmaHandler, writeContext);
        totalWritten = written;
      }

      currentOffset += iter.size;
    }

    if (writeContext->counter == 0) {
      writeContext->counter = 1;

      dmaHandler(getTick(), writeContext);
    }
  };

  DMAContext *writeContext = new DMAContext(func, context);

  execute(CPU::NVME__PRPLIST, CPU::WRITE, doWrite, writeContext);
}

SGLDescriptor::SGLDescriptor() {
  memset(data, 0, 16);
}

Chunk::Chunk() : addr(0), length(0), ignore(true) {}

Chunk::Chunk(uint64_t a, uint32_t l, bool i) : addr(a), length(l), ignore(i) {}

SGL::SGL(ConfigData &cfg, DMAFunction &f, void *c, uint64_t prp1, uint64_t prp2)
    : DMAInterface(cfg, f, c), totalSize(0) {
  SGLDescriptor desc;

  // Create first SGL descriptor from PRP pointers
  memcpy(desc.data, &prp1, 8);
  memcpy(desc.data + 8, &prp2, 8);

  // Check type
  if (SGL_TYPE(desc.id) == TYPE_DATA_BLOCK_DESCRIPTOR ||
      SGL_TYPE(desc.id) == TYPE_KEYED_DATA_BLOCK_DESCRIPTOR) {
    // This is entire buffer
    parseSGLDescriptor(desc);

    schedule(immediateEvent, getTick());
  }
  else if (SGL_TYPE(desc.id) == TYPE_SEGMENT_DESCRIPTOR ||
           SGL_TYPE(desc.id) == TYPE_LAST_SEGMENT_DESCRIPTOR) {
    // This points segment
    parseSGLSegment(desc.address, desc.length);
  }
}

SGL::~SGL() {}

void SGL::parseSGLDescriptor(SGLDescriptor &desc) {
  switch (SGL_TYPE(desc.id)) {
    case TYPE_DATA_BLOCK_DESCRIPTOR:
    case TYPE_KEYED_DATA_BLOCK_DESCRIPTOR:
      chunkList.push_back(Chunk(desc.address, desc.length, false));
      totalSize += desc.length;

      break;
    case TYPE_BIT_BUCKET_DESCRIPTOR:
      chunkList.push_back(Chunk(desc.address, desc.length, true));
      totalSize += desc.length;

      break;
    default:
      panic("Invalid SGL descriptor");
      break;
  }

  if (SGL_SUBTYPE(desc.id) != SUBTYPE_ADDRESS) {
    panic("Unexpected SGL subtype");
  }
}

void SGL::parseSGLSegment(uint64_t address, uint32_t length) {
  static DMAFunction doRead = [](uint64_t now, void *context) {
    SGLDescriptor desc;
    DMAInitContext *pContext = (DMAInitContext *)context;
    SGL *pThis = (SGL *)pContext->pThis;
    bool next = false;

    pThis->callCounter--;

    for (uint32_t i = 0; i < pContext->currentSize; i += 16) {
      memcpy(desc.data, pContext->buffer + i, 16);

      switch (SGL_TYPE(desc.id)) {
        case TYPE_DATA_BLOCK_DESCRIPTOR:
        case TYPE_KEYED_DATA_BLOCK_DESCRIPTOR:
        case TYPE_BIT_BUCKET_DESCRIPTOR:
          pThis->parseSGLDescriptor(desc);

          break;
        case TYPE_SEGMENT_DESCRIPTOR:
        case TYPE_LAST_SEGMENT_DESCRIPTOR:
          if (i != pContext->currentSize - 16) {
            panic("Invalid SGL segment");
          }
          next = true;

          break;
      }
    }

    free(pContext->buffer);

    // Go to next
    if (next) {
      pThis->parseSGLSegment(desc.address, desc.length);
    }

    if (pThis->callCounter == 0) {
      pThis->initFunction(now, pThis->context);
    }

    delete pContext;
  };

  DMAInitContext *pContext = new DMAInitContext();

  callCounter++;

  // Allocate buffer
  pContext->pThis = this;
  pContext->currentSize = length;
  pContext->buffer = (uint8_t *)calloc(length, 1);

  if (pContext->buffer) {
    // Read segment
    CPUContext *pCPU = new CPUContext(doRead, pContext, CPU::NVME__SGL,
                                      CPU::PARSE_SGL_SEGMENT);
    pInterface->dmaRead(address, length, pContext->buffer, cpuHandler, pCPU);
  }
  else {
    delete pContext;

    panic("sgl: ENOMEM");
  }
}

void SGL::read(uint64_t offset, uint64_t length, uint8_t *buffer,
               DMAFunction &func, void *context) {
  DMAFunction doRead = [this, offset, length, buffer](uint64_t, void *context) {
    uint64_t totalRead = 0;
    uint64_t currentOffset = 0;
    uint64_t read;
    bool begin = false;

    DMAContext *readContext = (DMAContext *)context;

    for (auto &iter : chunkList) {
      if (begin) {
        read = MIN(iter.length, length - totalRead);

        if (!iter.ignore) {
          readContext->counter++;
          pInterface->dmaRead(iter.addr, read,
                              buffer ? buffer + totalRead : NULL, dmaHandler,
                              readContext);
        }

        totalRead += read;

        if (totalRead == length) {
          break;
        }
      }

      if (!begin && currentOffset + iter.length > offset) {
        begin = true;
        totalRead = offset - currentOffset;
        read = MIN(iter.length - totalRead, length);

        if (!iter.ignore) {
          readContext->counter++;
          pInterface->dmaRead(iter.addr + totalRead, read, buffer, dmaHandler,
                              readContext);
        }

        totalRead = read;
      }

      currentOffset += iter.length;
    }

    if (readContext->counter == 0) {
      readContext->counter = 1;

      dmaHandler(getTick(), readContext);
    }
  };

  DMAContext *readContext = new DMAContext(func, context);

  execute(CPU::NVME__SGL, CPU::READ, doRead, readContext);
}

void SGL::write(uint64_t offset, uint64_t length, uint8_t *buffer,
                DMAFunction &func, void *context) {
  DMAFunction doWrite = [this, offset, length, buffer](uint64_t,
                                                       void *context) {
    uint64_t totalWritten = 0;
    uint64_t currentOffset = 0;
    uint64_t written;
    bool begin = false;

    DMAContext *writeContext = (DMAContext *)context;

    for (auto &iter : chunkList) {
      if (begin) {
        written = MIN(iter.length, length - totalWritten);

        if (!iter.ignore) {
          writeContext->counter++;
          pInterface->dmaWrite(iter.addr, written,
                               buffer ? buffer + totalWritten : NULL,
                               dmaHandler, writeContext);
        }

        totalWritten += written;

        if (totalWritten == length) {
          break;
        }
      }

      if (!begin && currentOffset + iter.length > offset) {
        begin = true;
        totalWritten = offset - currentOffset;
        written = MIN(iter.length - totalWritten, length);

        if (!iter.ignore) {
          writeContext->counter++;
          pInterface->dmaWrite(iter.addr + totalWritten, written, buffer,
                               dmaHandler, writeContext);
        }

        totalWritten = written;
      }

      currentOffset += iter.length;
    }

    if (writeContext->counter == 0) {
      writeContext->counter = 1;

      dmaHandler(getTick(), writeContext);
    }
  };

  DMAContext *writeContext = new DMAContext(func, context);

  execute(CPU::NVME__SGL, CPU::WRITE, doWrite, writeContext);
}

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD
