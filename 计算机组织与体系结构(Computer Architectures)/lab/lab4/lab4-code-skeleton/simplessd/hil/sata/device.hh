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

#ifndef __HIL_SATA_DEVICE__
#define __HIL_SATA_DEVICE__

#include "hil/hil.hh"
#include "hil/sata/def.hh"
#include "hil/sata/hba.hh"
#include "util/disk.hh"

namespace SimpleSSD {

namespace HIL {

namespace SATA {

struct CommandContext {
  FIS request;
  uint8_t *prdt;
  uint16_t prdtLength;

  uint32_t slotIndex;
  bool reqDone;
  bool prdtDone;

  CommandContext()
      : prdt(nullptr),
        prdtLength(0),
        slotIndex(0),
        reqDone(false),
        prdtDone(false) {}

  void release() {
    if (prdtLength > 0) {
      free(prdt);
    }

    delete this;
  }
};

struct IOContext {
  uint64_t beginAt;
  uint64_t tick;
  uint64_t slba;
  uint64_t nlb;
  uint8_t *buffer;

  CommandContext *cmd;

  IOContext(CommandContext *c)
      : beginAt(getTick()), tick(0), slba(0), nlb(0), buffer(nullptr), cmd(c) {}
};

struct NCQContext : public IOContext {
  uint8_t tag;

  // Copy of PRDT
  uint8_t *prdt;
  uint16_t prdtLength;

  NCQContext(CommandContext *c) : IOContext(c), tag(0) {
    prdt = c->prdt;
    prdtLength = c->prdtLength;

    // Prevent prdt being freed
    c->prdtLength = 0;
  }

  void release() {
    if (prdtLength > 0) {
      free(prdt);
    }

    delete this;
  }
};

class Device : public StatObject {
 private:
  HBA *pParent;
  DMAInterface *pDMA;
  HIL *pHIL;
  Disk *pDisk;

  uint64_t totalLogicalPages;
  uint32_t logicalPageSize;
  uint32_t lbaSize;

  ConfigReader &conf;

  // Handlers
  DMAFunction dmaHandler;

  // PRDT
  void prdtRead(uint8_t *, uint32_t, uint32_t, uint8_t *, DMAFunction &,
                void *);
  void prdtWrite(uint8_t *, uint32_t, uint32_t, uint8_t *, DMAFunction &,
                 void *);

  // SimpleSSD
  void read(uint64_t, uint64_t, uint8_t *, DMAFunction &, void *);
  void write(uint64_t, uint64_t, uint8_t *, DMAFunction &, void *);
  void flush(DMAFunction &, void *);
  void convertUnit(uint64_t, uint64_t, SimpleSSD::HIL::Request &);

  // FPDMA
  DMAFunction writeDMASetup;
  void _writeDMASetup(uint64_t, void *);
  DMAFunction writeDMADone;
  void _writeDMADone(uint64_t, void *);
  DMAFunction readDMASetup;
  void _readDMASetup(uint64_t, void *);
  DMAFunction readDMADone;
  void _readDMADone(uint64_t, void *);

  // SATA / ATA8-ACS commands
  void identifyDevice(CommandContext *);
  void setMode(CommandContext *);
  void readVerify(CommandContext *);
  void readDMA(CommandContext *, bool = false);
  void readNCQ(CommandContext *);
  void writeDMA(CommandContext *, bool = false);
  void writeNCQ(CommandContext *);
  void flushCache(CommandContext *);

 public:
  Device(HBA *, DMAInterface *, ConfigReader &);
  ~Device();

  // For AHCI
  void init();
  void submitCommand(RequestContext *);
};

}  // namespace SATA

}  // namespace HIL

}  // namespace SimpleSSD

#endif
