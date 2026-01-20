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

#ifndef __HIL_UFS_DEVICE__
#define __HIL_UFS_DEVICE__

#include <cinttypes>

#include "hil/hil.hh"
#include "hil/ufs/config.hh"
#include "hil/ufs/def.hh"
#include "hil/ufs/host.hh"
#include "util/disk.hh"

namespace SimpleSSD {

namespace HIL {

namespace UFS {

typedef struct _LUN {
  bool bWellknown;
  uint8_t id;

  uint8_t unitDescriptor[IDN_UNIT_LENGTH];
  uint8_t inquiry[SCSI_INQUIRY_LENGTH];

  _LUN(bool, uint8_t, ConfigReader &);
} LUN;

class Device : public StatObject {
 private:
  DMAInterface *pDMA;
  HIL *pHIL;
  Disk *pDisk;

  uint64_t totalLogicalPages;
  uint32_t logicalPageSize;

  ConfigReader &conf;

  // Cache info
  bool bReadCache;
  bool bWriteCache;

  // Well-known LUNs
  LUN lunReportLuns;
  LUN lunUFSDevice;
  LUN lunBoot;
  LUN lunRPMB;

  // LUN
  LUN lun;

  // Descriptors
  uint8_t deviceDescriptor[IDN_DEVICE_LENGTH];
  uint8_t powerDescriptor[IDN_POWER_LENGTH];

  uint8_t strManufacturer[IDN_STRING_LENGTH - 2];
  uint8_t strProductName[IDN_STRING_LENGTH - 2];
  uint8_t strSerialNumber[IDN_STRING_LENGTH - 2];
  uint8_t strOEMID[IDN_STRING_LENGTH - 2];
  uint8_t strProductRevision[IDN_STRING_LENGTH - 2];

  // PRDT
  DMAFunction dmaHandler;

  void prdtRead(uint8_t *, uint32_t, uint32_t, uint8_t *, DMAFunction &,
                void * = nullptr);
  void prdtWrite(uint8_t *, uint32_t, uint32_t, uint8_t *, DMAFunction &,
                 void * = nullptr);

  // IO
  void convertUnit(uint64_t, uint64_t, Request &);
  void read(uint64_t, uint64_t, uint8_t *, DMAFunction &, void *);
  void write(uint64_t, uint64_t, uint8_t *, DMAFunction &, void *);
  void flush(DMAFunction &, void *);

 public:
  Device(DMAInterface *, ConfigReader &);
  ~Device();

  void processQueryCommand(UPIUQueryReq *, UPIUQueryResp *);
  void processCommand(UTP_TRANSFER_CMD, UPIUCommand *, UPIUResponse *,
                      uint8_t *, uint32_t, DMAFunction &, void * = nullptr);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace UFS

}  // namespace HIL

}  // namespace SimpleSSD

#endif
