/**
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
 *
 * Authors: Jie Zhang <jie@camelab.org>
 */

#include "PAL2.h"

#include "util/algorithm.hh"

PAL2::PAL2(PALStatistics *statistics, SimpleSSD::PAL::Parameter *p,
           SimpleSSD::ConfigReader *c, Latency *l)
    : pParam(p), lat(l), stats(statistics) {
  uint32_t OriginalSizes[7];

  uint32_t SPDIV =
      c->readUint(SimpleSSD::CONFIG_PAL, SimpleSSD::PAL::NAND_DMA_SPEED) / 50;
  uint32_t PGDIV = 16384 / c->readUint(SimpleSSD::CONFIG_PAL,
                                       SimpleSSD::PAL::NAND_PAGE_SIZE);

  if (SPDIV == 0 || PGDIV == 0) {
    SimpleSSD::panic("SPDIV or PGDIV is 0");
  }

  OriginalSizes[ADDR_CHANNEL] = pParam->channel;
  OriginalSizes[ADDR_PACKAGE] = pParam->package;
  OriginalSizes[ADDR_DIE] = pParam->die;

  if (c->readBoolean(SimpleSSD::CONFIG_PAL,
                     SimpleSSD::PAL::NAND_USE_MULTI_PLANE_OP)) {
    OriginalSizes[ADDR_PLANE] = 1;
  }
  else {
    OriginalSizes[ADDR_PLANE] = pParam->plane;
  }

  OriginalSizes[ADDR_BLOCK] = pParam->block;
  OriginalSizes[ADDR_PAGE] = pParam->page;
  OriginalSizes[6] = 0;  // Add remaining bits

  AddrRemap[0] = ADDR_PAGE;
  AddrRemap[1] = ADDR_BLOCK;
  AddrRemap[2] = ADDR_PLANE;
  AddrRemap[3] = ADDR_DIE;
  AddrRemap[4] = ADDR_PACKAGE;
  AddrRemap[5] = ADDR_CHANNEL;

  RearrangedSizes[6] = OriginalSizes[6];
  for (int i = 0; i < 6; i++) {
    RearrangedSizes[i] = OriginalSizes[AddrRemap[i]];

    //  DPRINTF(PAL,"PAL: [%d] ORI(%s): %u --> REARR(%s): %u\n", i,
    //  ADDR_STRINFO[ i ], OriginalSizes[ i ], ADDR_STRINFO[ gconf->AddrSeq[i]
    //  ], RearrangedSizes[i]); //Use DPRINTF here
  }

  totalDie = pParam->channel * pParam->package * pParam->die;

  ChFreeSlots =
      new std::map<uint64_t, std::map<uint64_t, uint64_t> *>[pParam->channel];
  ChStartPoint = new uint64_t[pParam->channel];
  for (unsigned i = 0; i < pParam->channel; i++)
    ChStartPoint[i] = 0;

  DieFreeSlots =
      new std::map<uint64_t, std::map<uint64_t, uint64_t> *>[totalDie];
  DieStartPoint = new uint64_t[totalDie];
  for (unsigned i = 0; i < totalDie; i++)
    DieStartPoint[i] = 0;

  // currently, hard code pre-dma, mem-op and post-dma values
  for (unsigned i = 0; i < pParam->channel; i++) {
    std::map<uint64_t, uint64_t> *tmp;
    switch (
        c->readUint(SimpleSSD::CONFIG_PAL, SimpleSSD::PAL::NAND_FLASH_TYPE)) {
      case SimpleSSD::PAL::NAND_SLC:
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][100000 / SPDIV + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][185000000 / (PGDIV * SPDIV)] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][185000000 / (PGDIV * SPDIV) + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][1500000 / SPDIV] = tmp;
        break;
      case SimpleSSD::PAL::NAND_MLC:
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][100000 / SPDIV + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][185000000 / (PGDIV * SPDIV)] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][185000000 / (PGDIV * SPDIV) + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][1500000 / SPDIV] = tmp;
        break;
      case SimpleSSD::PAL::NAND_TLC:
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][100000 / SPDIV + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][185000000 / (PGDIV * SPDIV)] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][185000000 / (PGDIV * SPDIV) + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        ChFreeSlots[i][1500000 / SPDIV] = tmp;
        break;
      default:
        printf("unsupported NAND types!\n");
        std::terminate();
        break;
    }
  }

  for (unsigned i = 0; i < totalDie; i++) {
    std::map<uint64_t, uint64_t> *tmp;
    switch (
        c->readUint(SimpleSSD::CONFIG_PAL, SimpleSSD::PAL::NAND_FLASH_TYPE)) {
      case SimpleSSD::PAL::NAND_SLC:
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][25000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][300000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][2000000000 + 100000 / SPDIV] = tmp;
        break;
      case SimpleSSD::PAL::NAND_MLC:
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][40000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][90000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][500000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][1300000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][3500000000 + 100000 / SPDIV] = tmp;
        break;
      case SimpleSSD::PAL::NAND_TLC:
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][58000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][78000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][107000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][558000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][2201000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][5001000000 + 100000 / SPDIV] = tmp;
        tmp = new std::map<uint64_t, uint64_t>;
        DieFreeSlots[i][2274000000 + 100000 / SPDIV] = tmp;
        break;
      default:
        printf("unsupported NAND types!\n");
        std::terminate();
        break;
    }
  }
}

PAL2::~PAL2() {
  FlushTimeSlots(MAX64);

  for (uint i = 0; i < pParam->channel; i++) {
    for (auto &iter : ChFreeSlots[i]) {
      {
        delete iter.second;
        iter.second = NULL;
      }
    }
  }
  delete[] ChFreeSlots;

  for (uint i = 0; i < totalDie; i++) {
    for (auto &iter : DieFreeSlots[i]) {
      {
        delete iter.second;
        iter.second = NULL;
      }
    }
  }
  delete[] DieFreeSlots;

  delete[] ChStartPoint;
  delete[] DieStartPoint;
}

void PAL2::TimelineScheduling(Command &req, CPDPBP &reqCPD) {
  // ensure we can erase multiple blocks from single request
  unsigned erase_block = 1;
  /*=========== CONFLICT data gather ============*/
  for (unsigned cur_command = 0; cur_command < erase_block; cur_command++) {
#if GATHER_RESOURCE_CONFLICT
    uint8_t confType = CONFLICT_NONE;
#endif
    req.ppn = req.ppn - (req.ppn & (erase_block - 1)) + cur_command;
    uint32_t reqCh = reqCPD.Channel;
    uint32_t reqDieIdx = CPDPBPtoDieIdx(&reqCPD);
    TimeSlot tsDMA0, tsMEM, tsDMA1;
    uint64_t tickDMA0 = 0, tickMEM = 0,
             tickDMA1 = 0;  // start tick of the free slot
    uint64_t latDMA0, latMEM, latDMA1, totalLat;
    uint64_t DMA0tickFrom, MEMtickFrom, DMA1tickFrom;  // starting point
    uint64_t latANTI;                                  // anticipate time slot
    bool conflicts;  // check conflict when scheduling
    latDMA0 = lat->GetLatency(reqCPD.Page, req.operation, BUSY_DMA0);
    latMEM = lat->GetLatency(reqCPD.Page, req.operation, BUSY_MEM);
    latDMA1 = lat->GetLatency(reqCPD.Page, req.operation, BUSY_DMA1);
    latANTI = lat->GetLatency(reqCPD.Page, OPER_READ, BUSY_DMA0);
    // Start Finding available Slot
    DMA0tickFrom = req.arrived;  // get Current System Time
    while (1)                    // LOOP0
    {
      while (1)  // LOOP1
      {
        // 1a) LOOP1 - Find DMA0 available slot in ChTimeSlots
        if (!FindFreeTime(ChFreeSlots[reqCh], latDMA0, DMA0tickFrom, tickDMA0,
                          conflicts)) {
          if (DMA0tickFrom < ChStartPoint[reqCh]) {
            DMA0tickFrom = ChStartPoint[reqCh];
            conflicts = true;
          }
          else
            conflicts = false;
          tickDMA0 = ChStartPoint[reqCh];
        }
        else {
          if (conflicts)
            DMA0tickFrom = tickDMA0;
        }
        /*=========== CONFLICT check - DMA0 ============*/
#if GATHER_RESOURCE_CONFLICT
        if (conflicts && !(confType & CONFLICT_MEM)) {
          confType |= CONFLICT_DMA0;
        }
#endif

        // 2b) LOOP1 - Find MEM avaiable slot in DieTimeSlots
        MEMtickFrom = DMA0tickFrom;
        if (!FindFreeTime(DieFreeSlots[reqDieIdx], (latDMA0 + latMEM),
                          MEMtickFrom, tickMEM, conflicts)) {
          if (MEMtickFrom < DieStartPoint[reqDieIdx]) {
            MEMtickFrom = DieStartPoint[reqDieIdx];
            conflicts = true;
          }
          else {
            conflicts = false;
          }
          tickMEM = DieStartPoint[reqDieIdx];
        }
        else {
          if (conflicts)
            MEMtickFrom = tickMEM;
        }
        if (tickMEM == tickDMA0)
          break;
        DMA0tickFrom = MEMtickFrom;

        uint64_t tickDMA0_vrfy;
        if (!FindFreeTime(ChFreeSlots[reqCh], latDMA0, DMA0tickFrom,
                          tickDMA0_vrfy, conflicts)) {
          tickDMA0_vrfy = ChStartPoint[reqCh];
        }
        if (tickDMA0_vrfy == tickDMA0)
          break;
          /*=========== CONFLICT check - MEM ============*/
#if GATHER_RESOURCE_CONFLICT
        if (conflicts && !(confType & CONFLICT_DMA0)) {
          confType |= CONFLICT_MEM;
        }
#endif

      }  // LOOP1

      // 3) Find DMA1 available slot
      DMA1tickFrom = DMA0tickFrom + (latDMA0 + latMEM);
      if (!FindFreeTime(ChFreeSlots[reqCh], latDMA1 + latANTI, DMA1tickFrom,
                        tickDMA1, conflicts)) {
        if (DMA1tickFrom < ChStartPoint[reqCh]) {
          DMA1tickFrom = ChStartPoint[reqCh];
          conflicts = true;
        }
        else
          conflicts = false;
        tickDMA1 = ChStartPoint[reqCh];
      }
      else {
        if (conflicts)
          DMA1tickFrom = tickDMA1;
      }

      // 4) Re-verify MEM slot
      // The target die should be free during (DMA0_start ~ DMA1_end)
      totalLat = (DMA1tickFrom + latDMA1 + latANTI) - DMA0tickFrom;
      uint64_t tickMEM_vrfy;
      if (!FindFreeTime(DieFreeSlots[reqDieIdx], totalLat, DMA0tickFrom,
                        tickMEM_vrfy, conflicts)) {
        tickMEM_vrfy = DieStartPoint[reqDieIdx];
      }
      if (tickMEM_vrfy == tickMEM)
        break;
        /*=========== CONFLICT check - DMA1 ============*/
#if GATHER_RESOURCE_CONFLICT
      { confType |= CONFLICT_DMA1; }
#endif

      DMA0tickFrom = tickMEM_vrfy;  // or re-search for next available resource!
    }                               // LOOP0

    // 5) Assign dma0, dma1, mem
    {
      InsertFreeSlot(ChFreeSlots[reqCh], latDMA0, DMA0tickFrom, tickDMA0,
                     ChStartPoint[reqCh], 0);

      if (!FindFreeTime(ChFreeSlots[reqCh], latDMA1 + latANTI, DMA1tickFrom,
                        tickDMA1, conflicts)) {
        if (DMA1tickFrom < ChStartPoint[reqCh]) {
          DMA1tickFrom = ChStartPoint[reqCh];
          conflicts = true;
        }
        else
          conflicts = false;
        tickDMA1 = ChStartPoint[reqCh];
      }
      else {
        if (conflicts)
          DMA1tickFrom = tickDMA1;
      }
      if (DMA1tickFrom > tickDMA1)
        InsertFreeSlot(ChFreeSlots[reqCh], latDMA1, DMA1tickFrom + latANTI,
                       tickDMA1, ChStartPoint[reqCh], 0);
      else
        InsertFreeSlot(ChFreeSlots[reqCh], latDMA1, tickDMA1 + latANTI,
                       tickDMA1, ChStartPoint[reqCh], 0);

      // temporarily use previous MergedTimeSlots design
      InsertFreeSlot(DieFreeSlots[reqDieIdx], totalLat, DMA0tickFrom, tickMEM,
                     DieStartPoint[reqDieIdx], 0);

      if (DMA0tickFrom < tickDMA0)
        tsDMA0 = TimeSlot(tickDMA0, latDMA0);
      else
        tsDMA0 = TimeSlot(DMA0tickFrom, latDMA0);

      if (DMA1tickFrom < tickDMA1)
        tsDMA1 = TimeSlot(tickDMA1 + latANTI, latDMA1);
      else
        tsDMA1 = TimeSlot(DMA1tickFrom + latANTI, latDMA1);

      if (DMA0tickFrom < tickMEM)
        tsMEM = TimeSlot(tickMEM, totalLat);
      else
        tsMEM = TimeSlot(DMA0tickFrom, totalLat);

      //******************************************************************//
      if (DMA0tickFrom > tickDMA0)
        DMA0tickFrom = DMA0tickFrom + latDMA0;
      else
        DMA0tickFrom = tickDMA0 + latDMA0;
      uint64_t tmpTick = DMA0tickFrom;
      if (!FindFreeTime(ChFreeSlots[reqCh], latANTI * 2, DMA0tickFrom, tickDMA0,
                        conflicts)) {
        if (DMA0tickFrom < ChStartPoint[reqCh]) {
          DMA0tickFrom = ChStartPoint[reqCh];
          conflicts = true;
        }
        else
          conflicts = false;
        tickDMA0 = ChStartPoint[reqCh];
      }
      else {
        if (conflicts)
          DMA0tickFrom = tickDMA0;
      }
      if (DMA0tickFrom == tmpTick)
        InsertFreeSlot(ChFreeSlots[reqCh], latANTI * 2, DMA0tickFrom, tickDMA0,
                       ChStartPoint[reqCh], 1);
        //******************************************************************//
#if 1
      // Manage MergedTimeSlots
      if (MergedTimeSlots.size() == 0) {
        MergedTimeSlots.push_back(
            TimeSlot(tsMEM.StartTick, tsMEM.EndTick - tsMEM.StartTick + 1));
      }
      else {
        uint64_t s = tsMEM.StartTick;
        uint64_t e = tsMEM.EndTick;
        int spnt = 0, epnt = 0;  // inside(0), rightside(1)
        std::list<TimeSlot>::iterator cur;
        std::list<TimeSlot>::iterator spos = MergedTimeSlots.end();
        std::list<TimeSlot>::iterator epos = MergedTimeSlots.end();

        // find s position
        cur = MergedTimeSlots.begin();

        while (cur != MergedTimeSlots.end()) {
          if (cur->StartTick <= s && s <= cur->EndTick) {
            spos = cur;
            spnt = 0;  // inside
            break;
          }

          auto next = cur;

          next++;

          if ((next == MergedTimeSlots.end()) ||
              (next != MergedTimeSlots.end() && (s < next->StartTick))) {
            spos = cur;
            spnt = 1;  // rightside
            break;
          }

          cur = next;
        }

        // find e position
        cur = MergedTimeSlots.begin();

        while (cur != MergedTimeSlots.end()) {
          if (cur->StartTick <= e && e <= cur->EndTick) {
            epos = cur;
            epnt = 0;  // inside
            break;
          }

          auto next = cur;

          next++;

          if ((next == MergedTimeSlots.end()) ||
              (next != MergedTimeSlots.end() && (e < next->StartTick))) {
            epos = cur;
            epnt = 1;  // rightside
            break;
          }

          cur = next;
        }

        // merge
        // if both side is in a merged slot, skip
        if (!(spos != MergedTimeSlots.end() && epos != MergedTimeSlots.end() &&
              (spos == epos && spnt == 0 && epnt == 0))) {
          if (spos != MergedTimeSlots.end() &&
              spnt == 1) {  // right side of spos
            bool update = false;

            if (spos == epos) {
              update = true;
            }

            // duration will be updated later
            // Insert tmp after spos
            auto tmp = MergedTimeSlots.insert(
                ++spos,
                TimeSlot(tsMEM.StartTick, tsMEM.EndTick - tsMEM.StartTick + 1));

            if (update) {
              epos = tmp;
            }

            spos = --tmp;  // update spos
          }
          else {
            if (epos == MergedTimeSlots.end())  // both new
            {
              auto tmp =
                  TimeSlot(tsMEM.StartTick,
                           tsMEM.EndTick - tsMEM.StartTick + 1);  // copy one

              MergedTimeSlots.insert(MergedTimeSlots.begin(), tmp);
            }
            else {
              auto tmp = TimeSlot(tsMEM.StartTick,
                                  999);  // duration will be updated later
              spos = MergedTimeSlots.insert(MergedTimeSlots.begin(), tmp);
            }
          }

          if (epos != MergedTimeSlots.end()) {
            if (epnt == 0) {
              spos->EndTick = epos->EndTick;
            }
            else if (epnt == 1) {
              spos->EndTick = tsMEM.EndTick;
            }

            // remove [ spos->Next ~ epos ]
            auto iter = spos;

            for (iter++; iter != epos;) {
              iter = MergedTimeSlots.erase(iter);
            }

            // We need to erase epos
            MergedTimeSlots.erase(epos);
          }
        }
      }
#endif  // END MERGE TIMESLOT
    }

    // print Log
#if 1  // DBG_PRINT_BUSY
    if (1) {
#if DBG_PRINT_REQUEST
      // DPRINTF(PAL,
      //         "PAL: %s PPN 0x%" PRIX64 " ch%02d die%05d : REQTime  %" PRIu64
      //         "\n",
      //         OPER_STRINFO[req.operation], req.ppn, reqCPD.Channel,
      //         reqDieIdx, req.arrived);  // Use DPRINTF here
      printCPDPBP(&reqCPD);
      // DPRINTF(PAL,
      //         "PAL: %s PPN 0x%" PRIX64 " ch%02d die%05d : DMA0 %" PRIu64
      //         " ~ %" PRIu64 " (%" PRIu64 ") , MEM  %" PRIu64 " ~ %" PRIu64
      //         " (%" PRIu64 ") , DMA1 %" PRIu64 " ~ %" PRIu64 " (%" PRIu64
      //         ")\n", OPER_STRINFO[req.operation], req.ppn, reqCPD.Channel,
      //         reqDieIdx, tsDMA0->StartTick, tsDMA0->EndTick, (tsDMA0->EndTick
      //         - tsDMA0->StartTick + 1), tsMEM->StartTick, tsMEM->EndTick,
      //         (tsMEM->EndTick - tsMEM->StartTick + 1) -
      //             (tsDMA0->EndTick - tsDMA0->StartTick + 1) -
      //             (tsDMA1->EndTick - tsDMA1->StartTick + 1),
      //         tsDMA1->StartTick, tsDMA1->EndTick,
      //         (tsDMA1->EndTick - tsDMA1->StartTick + 1));  // Use DPRINTF
      //         here

      // DPRINTF(PAL,
      //         "PAL: %s PPN 0x%" PRIX64
      //         " ch%02d die%05d : REQ~DMA0start(%" PRIu64
      //         "), DMA0~DMA1end(%" PRIu64 ")\n",
      //         OPER_STRINFO[req.operation], req.ppn, reqCPD.Channel,
      //         reqDieIdx, (tsDMA0->StartTick - 1 - req.arrived + 1),
      //         (tsDMA1->EndTick - tsDMA0->StartTick + 1));  // Use DPRINTF
      //         here
#endif
    }
#endif

    // 6) Write-back latency on RequestLL
    req.finished = tsDMA1.EndTick;

    // categorize the time spent for read/write operation
    std::map<uint64_t, uint64_t>::iterator e;
    e = OpTimeStamp[req.operation].find(tsDMA0.StartTick);
    if (e != OpTimeStamp[req.operation].end()) {
      if (e->second < tsDMA1.EndTick)
        e->second = tsDMA1.EndTick;
    }
    else {
      OpTimeStamp[req.operation][tsDMA0.StartTick] = tsDMA1.EndTick;
    }
    FlushOpTimeStamp();

    // Update stats
#if 1
    stats->UpdateLastTick(tsDMA1.EndTick);
#if GATHER_RESOURCE_CONFLICT
    stats->AddLatency(req, &reqCPD, reqDieIdx, tsDMA0, tsMEM, tsDMA1, confType);
#else
    stats->AddLatency(req, &reqCPD, reqDieIdx, tsDMA0, tsMEM, tsDMA1);
#endif
#endif

    if (req.operation == OPER_ERASE || req.mergeSnapshot) {
      stats->MergeSnapshot();
    }
  }
}

void PAL2::submit(Command &cmd, CPDPBP &addr) {
  TimelineScheduling(cmd, addr);
}

void PAL2::FlushATimeSlotBusyTime(std::list<TimeSlot> &tgtTimeSlot,
                                  uint64_t currentTick, uint64_t *TimeSum) {
  auto cur = tgtTimeSlot.begin();

  while (cur != tgtTimeSlot.end()) {
    if (cur->EndTick < currentTick) {
      *TimeSum += (cur->EndTick - cur->StartTick + 1);

      cur = tgtTimeSlot.erase(cur);

      continue;
    }

    break;
  }
}

void PAL2::FlushOpTimeStamp()  // currently only used during garbage collection
{
  // flush OpTimeStamp
  std::map<uint64_t, uint64_t>::iterator e;
  for (unsigned Oper = 0; Oper < 3; Oper++) {
    uint64_t start_tick = (uint64_t)-1, end_tick = (uint64_t)-1;
    for (e = OpTimeStamp[Oper].begin(); e != OpTimeStamp[Oper].end();) {
      if (start_tick == (uint64_t)-1 && end_tick == (uint64_t)-1) {
        start_tick = e->first;
        end_tick = e->second;
      }
      else if (e->first < end_tick && e->second <= end_tick) {
      }
      else if (e->first < end_tick && e->second > end_tick) {
        end_tick = e->second;
      }
      else if (e->first >= end_tick) {
        stats->OpBusyTime[Oper] += end_tick - start_tick + 1;
        start_tick = e->first;
        end_tick = e->second;
      }
      OpTimeStamp[Oper].erase(e);
      e = OpTimeStamp[Oper].begin();
    }
    stats->OpBusyTime[Oper] += end_tick - start_tick + 1;
  }
}

void PAL2::FlushTimeSlots(uint64_t currentTick) {
  FlushATimeSlotBusyTime(MergedTimeSlots, currentTick, &(stats->ExactBusyTime));

  stats->Access_Capacity.update();
  stats->Ticks_Total.update();
}

void PAL2::FlushFreeSlots(uint64_t currentTick) {
  for (uint32_t i = 0; i < pParam->channel; i++) {
    FlushAFreeSlot(ChFreeSlots[i], currentTick);
  }
  for (uint32_t i = 0; i < totalDie; i++) {
    FlushAFreeSlot(DieFreeSlots[i], currentTick);
  }

  FlushATimeSlotBusyTime(MergedTimeSlots, currentTick, &(stats->ExactBusyTime));

  stats->Access_Capacity.update();
  stats->Ticks_Total.update();
}

void PAL2::FlushAFreeSlot(
    std::map<uint64_t, std::map<uint64_t, uint64_t> *> &tgtFreeSlot,
    uint64_t currentTick) {
  for (std::map<uint64_t, std::map<uint64_t, uint64_t> *>::iterator e =
           tgtFreeSlot.begin();
       e != tgtFreeSlot.end(); e++) {
    std::map<uint64_t, uint64_t>::iterator f = (e->second)->begin();
    std::map<uint64_t, uint64_t>::iterator g = (e->second)->end();

    for (; f != g;) {
      // count++;
      if (f->second < currentTick)
        (e->second)->erase(f++);
      else
        break;
    }
  }
}

std::list<TimeSlot>::iterator PAL2::FindFreeTime(
    std::list<TimeSlot> &tgtTimeSlot, uint64_t tickLen, uint64_t fromTick) {
  auto cur = tgtTimeSlot.begin();
  auto next = cur;

  while (cur != tgtTimeSlot.end()) {
    // printf("FindFreeTime.FIND : cur->ST=%llu, cur->ET=%llu, next=0x%X\n",
    // cur->StartTick, cur->EndTick, (unsigned int)next); //DBG
    next++;

    if (next == tgtTimeSlot.end()) {
      break;
    }

    if (cur->EndTick < fromTick && fromTick < next->StartTick) {
      if ((next->StartTick - fromTick) >= tickLen) {
        // printf("FindFreeTime.RET A: cur->ET=%llu, next->ST=%llu, ft=%llu,
        // tickLen=%llu\n", cur->EndTick, next->StartTick, fromTick, tickLen);
        // //DBG
        break;
      }
    }
    else if (fromTick <= cur->EndTick) {
      if ((next->StartTick - (cur->EndTick + 1)) >= tickLen) {
        break;
      }
    }

    cur++;
  }

  // v-- this condition looks strange? but [minus of uint64_t values] do not
  // work correctly without this!
  if ((tgtTimeSlot.begin()->StartTick > fromTick) &&
      ((tgtTimeSlot.begin()->StartTick - fromTick) >= tickLen)) {
    cur = tgtTimeSlot.end();  // if there is an available time slot before first
                              // cmd, return NULL!
  }

  return cur;
}

bool PAL2::FindFreeTime(
    std::map<uint64_t, std::map<uint64_t, uint64_t> *> &tgtFreeSlot,
    uint64_t tickLen, uint64_t tickFrom, uint64_t &startTick, bool &conflicts) {
  auto e = tgtFreeSlot.upper_bound(tickLen);
  if (e == tgtFreeSlot.end()) {
    e--;  // Jie: tgtFreeSlot is not empty
    auto f = (e->second)->upper_bound(tickFrom);
    if (f != (e->second)->begin()) {
      f--;
      if (f->second >= tickLen + tickFrom - (uint64_t)1) {
        startTick = f->first;
        conflicts = false;
        return true;
      }
      f++;
    }
    while (f != (e->second)->end()) {
      if (f->second >= tickLen + f->first - 1) {
        startTick = f->first;
        conflicts = true;
        return true;
      }
      f++;
    }
    // reach this means no FreeSlot satisfy the requirement; allocate unused
    // slots  startTick will be updated in upper function
    conflicts = false;
    return false;
  }
  if (e != tgtFreeSlot.begin())
    e--;
  uint64_t minTick = (uint64_t)-1;
  while (e != tgtFreeSlot.end()) {
    std::map<uint64_t, uint64_t>::iterator f =
        (e->second)->upper_bound(tickFrom);
    if (f != (e->second)->begin()) {
      f--;
      if (f->second >=
          tickLen + tickFrom - (uint64_t)1) {  // this free slot is
                                               // best fit one, skip
                                               // checking others
        startTick = f->first;
        conflicts = false;
        return true;
      }
      f++;
    }
    while (f != (e->second)->end()) {
      if (f->second >= tickLen + f->first - (uint64_t)1) {
        if (minTick == (uint64_t)-1 || minTick > f->first) {
          conflicts = true;
          minTick = f->first;
        }
        break;
      }
      f++;
    }
    e++;
  }
  if (minTick == (uint64_t)-1) {
    // startTick will be updated in upper function
    conflicts = false;
    return false;
  }
  else {
    startTick = minTick;
    return true;
  }
}

void PAL2::InsertFreeSlot(
    std::map<uint64_t, std::map<uint64_t, uint64_t> *> &tgtFreeSlot,
    uint64_t tickLen, uint64_t tickFrom, uint64_t startTick,
    uint64_t &startPoint, bool split) {
  if (startTick == startPoint) {
    if (tickFrom == startTick) {
      if (split)
        AddFreeSlot(tgtFreeSlot, tickLen, startPoint);
      startPoint = startPoint + tickLen;  // Jie: just need to shift startPoint
    }
    else {
      assert(tickFrom > startTick);
      if (split)
        AddFreeSlot(tgtFreeSlot, tickLen, tickFrom);
      startPoint = tickFrom + tickLen;
      AddFreeSlot(tgtFreeSlot, tickFrom - startTick, startTick);
    }
  }
  else {
    std::pair<std::map<uint64_t, std::map<uint64_t, uint64_t> *>::iterator,
              std::map<uint64_t, std::map<uint64_t, uint64_t> *>::iterator>
        ePair;
    ePair = tgtFreeSlot.equal_range(tickLen);
    std::map<uint64_t, uint64_t>::iterator f;
    std::map<uint64_t, std::map<uint64_t, uint64_t> *>::iterator e =
        tgtFreeSlot.upper_bound(tickLen);
    if (e != tgtFreeSlot.begin())
      e--;
    while (e != tgtFreeSlot.end()) {
      f = e->second->find(startTick);
      if (f != e->second->end()) {
        uint64_t tmpStartTick = f->first;
        uint64_t tmpEndTick = f->second;
        e->second->erase(f);
        if (tmpStartTick < tickFrom) {
          AddFreeSlot(tgtFreeSlot, tickFrom - tmpStartTick, tmpStartTick);
          if (split)
            AddFreeSlot(tgtFreeSlot, tickLen, tickFrom);
          assert(tmpEndTick - tickFrom + 1 >= tickLen);
          if (tmpEndTick > tickLen + tickFrom - (uint64_t)1) {
            AddFreeSlot(tgtFreeSlot, tmpEndTick - (tickFrom + tickLen - 1),
                        tickFrom + tickLen);
          }
        }
        else {
          assert(tmpStartTick == tickFrom);
          assert(tmpEndTick - tickFrom + 1 >= tickLen);
          if (split)
            AddFreeSlot(tgtFreeSlot, tickLen, tmpStartTick);
          if (tmpEndTick > tickLen + tickFrom - (uint64_t)1) {
            AddFreeSlot(tgtFreeSlot, tmpEndTick - (tickFrom + tickLen - 1),
                        tmpStartTick + tickLen);
          }
        }
        break;
      }
      e++;
    }
  }
}

void PAL2::AddFreeSlot(
    std::map<uint64_t, std::map<uint64_t, uint64_t> *> &tgtFreeSlot,
    uint64_t tickLen, uint64_t tickFrom) {
  std::map<uint64_t, std::map<uint64_t, uint64_t> *>::iterator e;
  e = tgtFreeSlot.upper_bound(tickLen);
  if (e != tgtFreeSlot.begin()) {
    e--;
    (e->second)->insert(std::pair<uint64_t, uint64_t>(
        tickFrom, tickFrom + tickLen - (uint64_t)1));
  }
}
// PPN number conversion
uint32_t PAL2::CPDPBPtoDieIdx(CPDPBP *pCPDPBP) {
  //[Channel][Package][Die];
  uint32_t ret = 0;
  ret += pCPDPBP->Die;
  ret += pCPDPBP->Package * pParam->die;
  ret += pCPDPBP->Channel * pParam->die * pParam->package;

  return ret;
}

void PAL2::printCPDPBP(CPDPBP *) {}
// void PAL2::printCPDPBP(CPDPBP *pCPDPBP) {
//   uint32_t *pCPDPBP_IDX = ((uint32_t *)pCPDPBP);
//
//   DPRINTF(PAL, "PAL:    %7s | %7s | %7s | %7s | %7s | %7s\n",
//           ADDR_STRINFO[gconf->AddrSeq[0]], ADDR_STRINFO[gconf->AddrSeq[1]],
//           ADDR_STRINFO[gconf->AddrSeq[2]], ADDR_STRINFO[gconf->AddrSeq[3]],
//           ADDR_STRINFO[gconf->AddrSeq[4]],
//           ADDR_STRINFO[gconf->AddrSeq[5]]);  // Use DPRINTF here
//   DPRINTF(PAL, "PAL:    %7u | %7u | %7u | %7u | %7u | %7u\n",
//           pCPDPBP_IDX[gconf->AddrSeq[0]], pCPDPBP_IDX[gconf->AddrSeq[1]],
//           pCPDPBP_IDX[gconf->AddrSeq[2]], pCPDPBP_IDX[gconf->AddrSeq[3]],
//           pCPDPBP_IDX[gconf->AddrSeq[4]],
//           pCPDPBP_IDX[gconf->AddrSeq[5]]);  // Use DPRINTF here
// }

void PAL2::PPNdisassemble(uint64_t *pPPN, CPDPBP *pCPDPBP) {
  uint32_t *pCPDPBP_IDX = ((uint32_t *)pCPDPBP);
  uint64_t tmp_MOD = *pPPN;
  uint8_t *AS = AddrRemap;
  uint32_t *RS = RearrangedSizes;
  for (uint32_t i = 0; i < 6; i++) {
    pCPDPBP_IDX[i] = 0;
  }
  if (RS[6] == 0)  // there is no misalignment
  {
    pCPDPBP_IDX[AS[0]] = tmp_MOD / (RS[5] * RS[4] * RS[3] * RS[2] * RS[1]);
    tmp_MOD = tmp_MOD % (RS[5] * RS[4] * RS[3] * RS[2] * RS[1]);

    pCPDPBP_IDX[AS[1]] = tmp_MOD / (RS[5] * RS[4] * RS[3] * RS[2]);
    tmp_MOD = tmp_MOD % (RS[5] * RS[4] * RS[3] * RS[2]);

    pCPDPBP_IDX[AS[2]] = tmp_MOD / (RS[5] * RS[4] * RS[3]);
    tmp_MOD = tmp_MOD % (RS[5] * RS[4] * RS[3]);

    pCPDPBP_IDX[AS[3]] = tmp_MOD / (RS[5] * RS[4]);
    tmp_MOD = tmp_MOD % (RS[5] * RS[4]);

    pCPDPBP_IDX[AS[4]] = tmp_MOD / (RS[5]);
    tmp_MOD = tmp_MOD % (RS[5]);

    pCPDPBP_IDX[AS[5]] = tmp_MOD;
  }
  else {
    uint32_t tmp_size = RS[6] * RS[5] * RS[4] * RS[3] * RS[2] * RS[1] * RS[0];
    for (int i = 0; i < (4 - AS[6]); i++) {
      tmp_size /= RS[i];
      pCPDPBP_IDX[AS[i]] = tmp_MOD / tmp_size;
      tmp_MOD = tmp_MOD % tmp_size;
    }
    tmp_size /= RS[6];
    uint32_t tmp = tmp_MOD / tmp_size;
    tmp_MOD = tmp_MOD % tmp_size;
    for (int i = (4 - AS[6]); i < 6; i++) {
      tmp_size /= RS[i];
      pCPDPBP_IDX[AS[i]] = tmp_MOD / tmp_size;
      tmp_MOD = tmp_MOD % tmp_size;
    }
    pCPDPBP_IDX[AS[5 - AS[6]]] *= tmp;
  }

#if DBG_PRINT_PPN
  DPRINTF(PAL, "PAL:     0x%llX (%llu) ==>\n", *pPPN,
          *pPPN);  // Use DPRINTF here
  printCPDPBP(pCPDPBP);
#endif
}

void PAL2::AssemblePPN(CPDPBP *pCPDPBP, uint64_t *pPPN) {
  uint64_t AddrPPN = 0;

  // with re-arrange ... I hate current structure! too dirty even made with
  // FOR-LOOP! (less-readability)
  uint32_t *pCPDPBP_IDX = ((uint32_t *)pCPDPBP);
  uint8_t *AS = AddrRemap;
  uint32_t *RS = RearrangedSizes;
  AddrPPN += pCPDPBP_IDX[AS[5]];
  AddrPPN += pCPDPBP_IDX[AS[4]] * (RS[5]);
  AddrPPN += pCPDPBP_IDX[AS[3]] * (RS[5] * RS[4]);
  AddrPPN += pCPDPBP_IDX[AS[2]] * (RS[5] * RS[4] * RS[3]);
  AddrPPN += pCPDPBP_IDX[AS[1]] * (RS[5] * RS[4] * RS[3] * RS[2]);
  AddrPPN += pCPDPBP_IDX[AS[0]] * (RS[5] * RS[4] * RS[3] * RS[2] * RS[1]);

  *pPPN = AddrPPN;

#if DBG_PRINT_PPN
  printCPDPBP(pCPDPBP);
  DPRINTF(PAL, "PAL    ==> 0x%llx (%llu)\n", *pPPN, *pPPN);  // Use DPRINTF here
#endif
}
