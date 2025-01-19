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

#ifndef __HIL_NVME_NAMESPACE__
#define __HIL_NVME_NAMESPACE__

#include <list>

#include "hil/nvme/def.hh"
#include "hil/nvme/dma.hh"
#include "hil/nvme/queue.hh"
#include "util/def.hh"
#include "util/disk.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

class Subsystem;

typedef union _DatasetManagementRange {
  uint8_t data[0x10];
  struct {
    uint32_t attr;
    uint32_t nlb;
    uint64_t slba;
  };
} DatasetManagementRange;

typedef std::function<void(CQEntryWrapper &)> RequestFunction;

class RequestContext {
 public:
  DMAInterface *dma;
  RequestFunction function;
  CQEntryWrapper resp;

  uint8_t *buffer;

  RequestContext(RequestFunction &f, CQEntryWrapper &r)
      : dma(nullptr), function(f), resp(r), buffer(nullptr) {}
};

class IOContext : public RequestContext {
 public:
  uint64_t beginAt;
  uint64_t slba;
  uint64_t nlb;
  uint64_t tick;

  IOContext(RequestFunction &f, CQEntryWrapper &r) : RequestContext(f, r) {}
};

class CompareContext : public IOContext {
 public:
  uint8_t *hostContent;

  CompareContext(RequestFunction &f, CQEntryWrapper &r)
      : IOContext(f, r), hostContent(nullptr) {}
};

class Namespace {
 public:
  typedef struct _Information {
    uint64_t size;                         //!< NSZE
    uint64_t capacity;                     //!< NCAP
    uint64_t utilization;                  //!< NUSE
    uint64_t sizeInByteL;                  //<! NVMCAPL
    uint64_t sizeInByteH;                  //<! NVMCAPH
    uint8_t lbaFormatIndex;                //!< FLBAS
    uint8_t dataProtectionSettings;        //!< DPS
    uint8_t namespaceSharingCapabilities;  //!< NMIC

    uint32_t lbaSize;
    LPNRange range;
  } Information;

 private:
  Subsystem *pParent;
  Disk *pDisk;

  ConfigData &cfgdata;
  ConfigReader &conf;

  Information info;
  uint32_t nsid;
  bool attached;
  bool allocated;

  HealthInfo health;

  uint64_t formatFinishedAt;

  // Admin commands
  void getLogPage(SQEntryWrapper &, RequestFunction &);

  // NVM commands
  void flush(SQEntryWrapper &, RequestFunction &);
  void write(SQEntryWrapper &, RequestFunction &);
  void read(SQEntryWrapper &, RequestFunction &);
  void compare(SQEntryWrapper &, RequestFunction &);
  void datasetManagement(SQEntryWrapper &, RequestFunction &);

 public:
  Namespace(Subsystem *, ConfigData &);
  ~Namespace();

  void submitCommand(SQEntryWrapper &, RequestFunction &);

  void setData(uint32_t, Information *);
  void attach(bool);
  uint32_t getNSID();
  Information *getInfo();
  bool isAttached();

  void format(uint64_t);
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD

#endif
