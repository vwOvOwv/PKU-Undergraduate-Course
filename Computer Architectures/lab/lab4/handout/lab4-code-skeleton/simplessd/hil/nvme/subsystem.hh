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

#ifndef __HIL_NVME_SUBSYSTEM__
#define __HIL_NVME_SUBSYSTEM__

#include "hil/hil.hh"
#include "hil/nvme/abstract_subsystem.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

class Subsystem : public AbstractSubsystem {
 protected:
  HIL *pHIL;

  std::list<Namespace *> lNamespaces;
  uint32_t queueAllocated;

  HealthInfo globalHealth;
  uint32_t logicalPageSize;
  uint64_t totalLogicalPages;
  uint64_t allocatedLogicalPages;

  // Stats
  uint64_t commandCount;

  void convertUnit(Namespace *, uint64_t, uint64_t, Request &);
  bool createNamespace(uint32_t, Namespace::Information *);
  bool destroyNamespace(uint32_t);
  void fillIdentifyNamespace(uint8_t *, Namespace::Information *);

  // Admin commands
  bool deleteSQueue(SQEntryWrapper &, RequestFunction &);
  bool createSQueue(SQEntryWrapper &, RequestFunction &);
  bool getLogPage(SQEntryWrapper &, RequestFunction &);
  bool deleteCQueue(SQEntryWrapper &, RequestFunction &);
  bool createCQueue(SQEntryWrapper &, RequestFunction &);
  bool identify(SQEntryWrapper &, RequestFunction &);
  bool abort(SQEntryWrapper &, RequestFunction &);
  bool setFeatures(SQEntryWrapper &, RequestFunction &);
  bool getFeatures(SQEntryWrapper &, RequestFunction &);
  bool asyncEventReq(SQEntryWrapper &, RequestFunction &);
  bool namespaceManagement(SQEntryWrapper &, RequestFunction &);
  bool namespaceAttachment(SQEntryWrapper &, RequestFunction &);
  bool formatNVM(SQEntryWrapper &, RequestFunction &);

 public:
  Subsystem(Controller *, ConfigData &);
  ~Subsystem();

  void init() override;

  void submitCommand(SQEntryWrapper &, RequestFunction) override;
  void getNVMCapacity(uint64_t &, uint64_t &) override;
  uint32_t validNamespaceCount() override;

  void read(Namespace *, uint64_t, uint64_t, DMAFunction &, void *);
  void write(Namespace *, uint64_t, uint64_t, DMAFunction &, void *);
  void flush(Namespace *, DMAFunction &, void *);
  void trim(Namespace *, uint64_t, uint64_t, DMAFunction &, void *);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD

#endif
