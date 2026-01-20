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

#ifndef __HIL_NVME_OCSSD__
#define __HIL_NVME_OCSSD__

#include "hil/nvme/subsystem.hh"
#include "pal/pal.hh"
#include "pal/pal_old.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

#define LBA_SIZE 4096  // Always OCSSD's logical block size is 4K

typedef struct _BlockData {
  uint32_t index;
  uint8_t value;

  _BlockData();
  _BlockData(uint32_t, uint8_t);
} BlockData;

typedef union _ChunkDescriptor {
  uint8_t data[32];
  struct {
    uint8_t chunkState;
    uint8_t chunkType;
    uint8_t wearLevelIndex;
    uint8_t reserved1;
    uint32_t reserved2;
    uint64_t slba;
    uint64_t nlb;
    uint64_t writePointer;
  };
} ChunkDescriptor;

typedef enum {
  FREE = 0x01,     //!< Erased (WP = 0)
  CLOSED = 0x02,   //!< Fulled (WP = max)
  OPEN = 0x04,     //!< Writing (WP < max)
  OFFLINE = 0x08,  //!< Dead
} CHUNK_STATE;

typedef struct _ChunkUpdateEntry {
  ChunkDescriptor *pDesc;
  uint32_t pageIdx;

  _ChunkUpdateEntry(ChunkDescriptor *, uint32_t);
} ChunkUpdateEntry;

typedef struct {
  uint32_t group;         // Channel
  uint32_t parallelUnit;  // Way
  uint32_t chunk;         // Blocks
  uint32_t chunkSize;     // Block Size / LBA_SIZE
  uint32_t writeSize;     // Page Size / LBA_SIZE
} Geometry;

typedef struct {
  uint64_t padding;
  uint64_t channelMask;
  uint32_t channelShift;
  uint64_t wayMask;
  uint32_t wayShift;
  uint64_t dieMask;
  uint32_t dieShift;
  uint64_t planeMask;
  uint32_t planeShift;
  uint64_t blockMask;
  uint32_t blockShift;
  uint64_t pageMask;
  uint32_t pageShift;
  uint64_t sectorMask;
} Mask;

class VectorContext : public IOContext {
 public:
  std::vector<uint64_t> lbaList;

  VectorContext(RequestFunction &f, CQEntryWrapper &r) : IOContext(f, r) {}
};

class OpenChannelSSD12 : public Subsystem {
 protected:
  PAL::Parameter param;
  PAL::PALOLD *pPALOLD;

  Disk *pDisk;

  Geometry structure;
  Mask ppaMask;

  uint64_t lastScheduled;
  Event completionEvent;
  std::priority_queue<Request, std::vector<Request>, Request> completionQueue;

  // Stats
  uint64_t eraseCount;
  uint64_t readCount;
  uint64_t writeCount;

  void updateCompletion();
  void completion();

 private:
  std::unordered_map<uint64_t, BlockData> badBlocks;

  bool parsePPA(uint64_t, ::CPDPBP &);
  void convertUnit(::CPDPBP &);
  void mergeList(std::vector<uint64_t> &, std::vector<::CPDPBP> &,
                 bool = false);

  // Commands
  bool deviceIdentification(SQEntryWrapper &, RequestFunction &);
  bool setBadBlockTable(SQEntryWrapper &, RequestFunction &);
  bool getBadBlockTable(SQEntryWrapper &, RequestFunction &);
  void physicalBlockErase(SQEntryWrapper &, RequestFunction &);
  void physicalPageWrite(SQEntryWrapper &, RequestFunction &);
  void physicalPageRead(SQEntryWrapper &, RequestFunction &);

 public:
  OpenChannelSSD12(Controller *, ConfigData &);
  ~OpenChannelSSD12();

  void init() override;

  void submitCommand(SQEntryWrapper &, RequestFunction) override;
  void getNVMCapacity(uint64_t &, uint64_t &) override;
  uint32_t validNamespaceCount() override;

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

class OpenChannelSSD20 : public OpenChannelSSD12 {
 private:
  ChunkDescriptor *pDescriptor;
  uint64_t descriptorLength;

  uint64_t vectorEraseCount;
  uint64_t vectorReadCount;
  uint64_t vectorWriteCount;

  ChunkDescriptor *getChunkDescriptor(uint32_t, uint32_t, uint32_t);
  uint64_t makeLBA(uint32_t, uint32_t, uint32_t, uint32_t);
  void parseLBA(uint64_t, uint32_t &, uint32_t &, uint32_t &, uint32_t &);
  void convertUnit(std::vector<uint64_t> &, std::vector<::CPDPBP> &,
                   std::vector<ChunkUpdateEntry> &, bool = false, bool = false);
  void readInternal(std::vector<uint64_t> &, DMAFunction &, void *,
                    bool = false);
  void writeInternal(std::vector<uint64_t> &, DMAFunction &, void *,
                     bool = false);
  void eraseInternal(std::vector<uint64_t> &, DMAFunction &, void *,
                     bool = false);

  // Commands
  bool getLogPage(SQEntryWrapper &, RequestFunction &);
  bool geometry(SQEntryWrapper &, RequestFunction &);
  void read(SQEntryWrapper &, RequestFunction &);
  void write(SQEntryWrapper &, RequestFunction &);
  void datasetManagement(SQEntryWrapper &, RequestFunction &);
  void vectorChunkRead(SQEntryWrapper &, RequestFunction &);
  void vectorChunkWrite(SQEntryWrapper &, RequestFunction &);
  void vectorChunkReset(SQEntryWrapper &, RequestFunction &);

 public:
  OpenChannelSSD20(Controller *, ConfigData &);
  ~OpenChannelSSD20();

  void init() override;

  void submitCommand(SQEntryWrapper &, RequestFunction) override;

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD

#endif
