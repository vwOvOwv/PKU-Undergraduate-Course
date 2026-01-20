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

#include "pal/pal_old.hh"

#include <sstream>

#include "pal/old/Latency.h"
#include "pal/old/LatencyMLC.h"
#include "pal/old/LatencySLC.h"
#include "pal/old/LatencyTLC.h"
#include "pal/old/PAL2.h"
#include "pal/old/PALStatistics.h"
#include "util/algorithm.hh"

#define FLUSH_PERIOD 100000000000ull  // 0.1sec
#define FLUSH_RANGE 10000000000ull    // 0.01sec

namespace SimpleSSD {

namespace PAL {

PALOLD::PALOLD(Parameter &p, ConfigReader &c)
    : AbstractPAL(p, c), lastResetTick(0) {
  Config::NANDTiming *pTiming = c.getNANDTiming();
  Config::NANDPower *pPower = c.getNANDPower();

  memset(&stat, 0, sizeof(stat));

  switch (conf.readInt(CONFIG_PAL, NAND_FLASH_TYPE)) {
    case NAND_SLC:
      lat = new LatencySLC(*pTiming, *pPower);
      break;
    case NAND_MLC:
      lat = new LatencyMLC(*pTiming, *pPower);
      break;
    case NAND_TLC:
      lat = new LatencyTLC(*pTiming, *pPower);
      break;
  }

  debugprint(LOG_PAL_OLD, "NAND timing:");
  debugprint(LOG_PAL_OLD, "Operation |     LSB    |     CSB    |     MSB    |  "
                          "  DMA 0   |    DMA ");
  debugprint(LOG_PAL_OLD,
             "   READ   | %10" PRIu64 " | %10" PRIu64 " | %10" PRIu64
             " | %10" PRIu64 " | %10" PRIu64,
             pTiming->lsb.read, pTiming->csb.read, pTiming->msb.read,
             pTiming->dma0.read, pTiming->dma1.read);
  debugprint(LOG_PAL_OLD,
             "   WRITE  | %10" PRIu64 " | %10" PRIu64 " | %10" PRIu64
             " | %10" PRIu64 " | %10" PRIu64,
             pTiming->lsb.write, pTiming->csb.write, pTiming->msb.write,
             pTiming->dma0.write, pTiming->dma1.write);
  debugprint(LOG_PAL_OLD,
             "   ERASE  |                           %10" PRIu64 " | %10" PRIu64
             " | %10" PRIu64,
             pTiming->erase, pTiming->dma0.erase, pTiming->dma1.erase);

  stats = new PALStatistics(&conf, lat);
  pal = new PAL2(stats, &param, &conf, lat);

  // We will periodically flush timeslot for saving memory
  flushFunction = [this](uint64_t tick) {
    pal->FlushFreeSlots(tick - FLUSH_RANGE);
    pal->FlushTimeSlots(tick - FLUSH_RANGE);

    schedule(flushEvent, tick + FLUSH_PERIOD);
  };
  flushEvent = allocate(flushFunction);
  schedule(flushEvent, getTick() + FLUSH_PERIOD);
}

PALOLD::~PALOLD() {
  delete pal;
  delete stats;
  delete lat;
}

void PALOLD::read(Request &req, uint64_t &tick) {
  uint64_t finishedAt = tick;
  ::Command cmd(tick, 0, OPER_READ, param.superPageSize);
  std::vector<::CPDPBP> list;

  printPPN(req, "READ");

  convertCPDPBP(req, list);

  for (auto &iter : list) {
    printCPDPBP(iter, "READ");

    pal->submit(cmd, iter);
    stat.readCount++;

    finishedAt = MAX(finishedAt, cmd.finished);
  }

  tick = finishedAt;
}

void PALOLD::write(Request &req, uint64_t &tick) {
  uint64_t finishedAt = tick;
  ::Command cmd(tick, 0, OPER_WRITE, param.superPageSize);
  std::vector<::CPDPBP> list;

  printPPN(req, "WRITE");

  convertCPDPBP(req, list);

  for (auto &iter : list) {
    printCPDPBP(iter, "WRITE");

    pal->submit(cmd, iter);
    stat.writeCount++;

    finishedAt = MAX(finishedAt, cmd.finished);
  }

  tick = finishedAt;
}

void PALOLD::erase(Request &req, uint64_t &tick) {
  uint64_t finishedAt = tick;
  ::Command cmd(tick, 0, OPER_ERASE, param.superPageSize * param.page);
  std::vector<::CPDPBP> list;

  printPPN(req, "ERASE");

  convertCPDPBP(req, list);

  for (auto &iter : list) {
    printCPDPBP(iter, "ERASE");

    pal->submit(cmd, iter);
    stat.eraseCount++;

    finishedAt = MAX(finishedAt, cmd.finished);
  }

  tick = finishedAt;
}

void PALOLD::convertCPDPBP(Request &req, std::vector<::CPDPBP> &list) {
  ::CPDPBP addr;
  static uint32_t pageAllocation = conf.getPageAllocationConfig();
  static uint8_t superblock = conf.getSuperblockConfig();
  static bool useMultiplaneOP =
      conf.readBoolean(CONFIG_PAL, NAND_USE_MULTI_PLANE_OP);
  static uint32_t pageInSuperPage = param.pageInSuperPage;
  static bool bRandomTweak =
      conf.readBoolean(CONFIG_FTL, FTL::FTL_USE_RANDOM_IO_TWEAK);
  uint32_t value[4];
  uint32_t *ptr[4];
  uint64_t tmp = req.blockIndex;
  int count = 0;

  if (bRandomTweak && req.ioFlag.size() != pageInSuperPage) {
    panic("Invalid size of I/O flag");
  }

  if (!bRandomTweak && req.ioFlag.size() != pageInSuperPage) {
    req.ioFlag = Bitset(pageInSuperPage);
    req.ioFlag.set();
  }

  list.clear();

  addr.Plane = 0;

  for (int i = 0; i < 4; i++) {
    uint8_t idx = (pageAllocation >> (i * 8)) & 0xFF;

    switch (idx) {
      case INDEX_CHANNEL:
        if (superblock & INDEX_CHANNEL) {
          value[count] = param.channel;
          ptr[count++] = &addr.Channel;
        }
        else {
          addr.Channel = tmp % param.channel;
          tmp /= param.channel;
        }

        break;
      case INDEX_PACKAGE:
        if (superblock & INDEX_PACKAGE) {
          value[count] = param.package;
          ptr[count++] = &addr.Package;
        }
        else {
          addr.Package = tmp % param.package;
          tmp /= param.package;
        }

        break;
      case INDEX_DIE:
        if (superblock & INDEX_DIE) {
          value[count] = param.die;
          ptr[count++] = &addr.Die;
        }
        else {
          addr.Die = tmp % param.die;
          tmp /= param.die;
        }

        break;
      case INDEX_PLANE:
        if (!useMultiplaneOP) {
          if (superblock & INDEX_PLANE) {
            value[count] = param.plane;
            ptr[count++] = &addr.Plane;
          }
          else {
            addr.Plane = tmp % param.plane;
            tmp /= param.plane;
          }
        }

        break;
      default:
        break;
    }
  }

  addr.Block = tmp;
  addr.Page = req.pageIndex;

  // Index of ioFlag
  tmp = 0;

  if (count == 4) {
    list.reserve(value[0] * value[1] * value[2] * value[3]);

    for (uint32_t i = 0; i < value[3]; i++) {
      for (uint32_t j = 0; j < value[2]; j++) {
        for (uint32_t k = 0; k < value[1]; k++) {
          for (uint32_t l = 0; l < value[0]; l++) {
            if (req.ioFlag.test(tmp++)) {
              *ptr[0] = l;
              *ptr[1] = k;
              *ptr[2] = j;
              *ptr[3] = i;

              list.push_back(addr);
            }
          }
        }
      }
    }
  }
  else if (count == 3) {
    list.reserve(value[0] * value[1] * value[2]);

    for (uint32_t j = 0; j < value[2]; j++) {
      for (uint32_t k = 0; k < value[1]; k++) {
        for (uint32_t l = 0; l < value[0]; l++) {
          if (req.ioFlag.test(tmp++)) {
            *ptr[0] = l;
            *ptr[1] = k;
            *ptr[2] = j;

            list.push_back(addr);
          }
        }
      }
    }
  }
  else if (count == 2) {
    list.reserve(value[0] * value[1]);

    for (uint32_t k = 0; k < value[1]; k++) {
      for (uint32_t l = 0; l < value[0]; l++) {
        if (req.ioFlag.test(tmp++)) {
          *ptr[0] = l;
          *ptr[1] = k;

          list.push_back(addr);
        }
      }
    }
  }
  else if (count == 1) {
    list.reserve(value[0]);

    for (uint32_t l = 0; l < value[0]; l++) {
      if (req.ioFlag.test(tmp++)) {
        *ptr[0] = l;

        list.push_back(addr);
      }
    }
  }
  else {
    if (req.ioFlag.test(tmp++)) {
      list.push_back(addr);
    }
  }

  if (tmp != pageInSuperPage) {
    panic("I/O flag size != # pages in super page");
  }
}

void PALOLD::printCPDPBP(::CPDPBP &addr, const char *prefix) {
  debugprint(LOG_PAL_OLD,
             "%-5s | C %5u | W %5u | D %5u | P %5u | B %5u | P %5u", prefix,
             addr.Channel, addr.Package, addr.Die, addr.Plane, addr.Block,
             addr.Page);
}

void PALOLD::printPPN(Request &req, const char *prefix) {
  debugprint(LOG_PAL_OLD, "%-5s | Block %u | Page %u", prefix, req.blockIndex,
             req.pageIndex);
}

void PALOLD::getStatList(std::vector<Stats> &list, std::string prefix) {
  Stats temp;

  temp.name = prefix + "energy.read";
  temp.desc = "Consumed energy by NAND read operation (uJ)";
  list.push_back(temp);

  temp.name = prefix + "energy.program";
  temp.desc = "Consumed energy by NAND program operation (uJ)";
  list.push_back(temp);

  temp.name = prefix + "energy.erase";
  temp.desc = "Consumed energy by NAND erase operation (uJ)";
  list.push_back(temp);

  temp.name = prefix + "energy.total";
  temp.desc = "Total consumed energy by NAND (uJ)";
  list.push_back(temp);

  temp.name = prefix + "power";
  temp.desc = "Average power consumed by NAND (uW)";
  list.push_back(temp);

  temp.name = prefix + "read.count";
  temp.desc = "Total read operation count";
  list.push_back(temp);

  temp.name = prefix + "program.count";
  temp.desc = "Total program operation count";
  list.push_back(temp);

  temp.name = prefix + "erase.count";
  temp.desc = "Total erase operation count";
  list.push_back(temp);

  temp.name = prefix + "read.bytes";
  temp.desc = "Total read operation bytes";
  list.push_back(temp);

  temp.name = prefix + "program.bytes";
  temp.desc = "Total program operation bytes";
  list.push_back(temp);

  temp.name = prefix + "erase.bytes";
  temp.desc = "Total erase operation bytes";
  list.push_back(temp);

  temp.name = prefix + "read.time.dma0.wait";
  temp.desc = "Average dma0 wait time of read";
  list.push_back(temp);

  temp.name = prefix + "read.time.dma0";
  temp.desc = "Average dma0 time of read";
  list.push_back(temp);

  temp.name = prefix + "read.time.mem";
  temp.desc = "Average memory operation time of read";
  list.push_back(temp);

  temp.name = prefix + "read.time.dma1.wait";
  temp.desc = "Average dma1 wait time of read";
  list.push_back(temp);

  temp.name = prefix + "read.time.dma1";
  temp.desc = "Average dma1 time of read";
  list.push_back(temp);

  temp.name = prefix + "read.time.total";
  temp.desc = "Average time of read";
  list.push_back(temp);

  temp.name = prefix + "program.time.dma0.wait";
  temp.desc = "Average dma0 wait time of program";
  list.push_back(temp);

  temp.name = prefix + "program.time.dma0";
  temp.desc = "Average dma0 time of program";
  list.push_back(temp);

  temp.name = prefix + "program.time.mem";
  temp.desc = "Average memory operation time of program";
  list.push_back(temp);

  temp.name = prefix + "program.time.dma1.wait";
  temp.desc = "Average dma1 wait time of program";
  list.push_back(temp);

  temp.name = prefix + "program.time.dma1";
  temp.desc = "Average dma1 time of program";
  list.push_back(temp);

  temp.name = prefix + "program.time.total";
  temp.desc = "Average time of program";
  list.push_back(temp);

  temp.name = prefix + "erase.time.dma0.wait";
  temp.desc = "Average dma0 wait time of erase";
  list.push_back(temp);

  temp.name = prefix + "erase.time.dma0";
  temp.desc = "Average dma0 time of erase";
  list.push_back(temp);

  temp.name = prefix + "erase.time.mem";
  temp.desc = "Average memory operation time of erase";
  list.push_back(temp);

  temp.name = prefix + "erase.time.dma1.wait";
  temp.desc = "Average dma1 wait time of erase";
  list.push_back(temp);

  temp.name = prefix + "erase.time.dma1";
  temp.desc = "Average dma1 time of erase";
  list.push_back(temp);

  temp.name = prefix + "erase.time.total";
  temp.desc = "Average time of erase";
  list.push_back(temp);

  temp.name = prefix + "channel.time.active";
  temp.desc = "Average active time of all channels";
  list.push_back(temp);

  temp.name = prefix + "die.time.active";
  temp.desc = "Average active time of all dies";
  list.push_back(temp);
}

void PALOLD::getStatValues(std::vector<double> &values) {
  PALStatistics::OperStats energy;
  PALStatistics::OperStats ticks;
  PALStatistics::ActiveTime active;
  PALStatistics::Breakdown breakdown;
  double elapsedTick = (double)(getTick() - lastResetTick);

  stats->getEnergyStat(energy);
  stats->getTickStat(ticks);

  values.push_back(energy.read);
  values.push_back(energy.write);
  values.push_back(energy.erase);
  values.push_back(energy.total);

  // uW = uJ / ps * 1e+12
  energy.total /= elapsedTick / 1e+12;
  values.push_back(energy.total);

  values.push_back(stat.readCount);
  values.push_back(stat.writeCount);
  values.push_back(stat.eraseCount);

  values.push_back(stat.readCount * param.pageSize);
  values.push_back(stat.writeCount * param.pageSize);
  values.push_back(stat.eraseCount * param.pageSize * param.page);

  stats->getReadBreakdown(breakdown);
  values.push_back(breakdown.dma0wait);
  values.push_back(breakdown.dma0);
  values.push_back(breakdown.mem);
  values.push_back(breakdown.dma1wait);
  values.push_back(breakdown.dma1);
  values.push_back(ticks.read);

  stats->getWriteBreakdown(breakdown);
  values.push_back(breakdown.dma0wait);
  values.push_back(breakdown.dma0);
  values.push_back(breakdown.mem);
  values.push_back(breakdown.dma1wait);
  values.push_back(breakdown.dma1);
  values.push_back(ticks.write);

  stats->getEraseBreakdown(breakdown);
  values.push_back(breakdown.dma0wait);
  values.push_back(breakdown.dma0);
  values.push_back(breakdown.mem);
  values.push_back(breakdown.dma1wait);
  values.push_back(breakdown.dma1);
  values.push_back(ticks.erase);

  stats->getChannelActiveTimeAll(active);
  values.push_back(active.average);

  stats->getDieActiveTimeAll(active);
  values.push_back(active.average);
}

void PALOLD::resetStatValues() {
  stats->ResetStats();
  lastResetTick = getTick();

  memset(&stat, 0, sizeof(stat));
}

void PALOLD::read(::CPDPBP &addr, uint64_t &tick) {
  ::Command cmd(tick, 0, OPER_READ, param.superPageSize);

  printCPDPBP(addr, "READ");
  pal->submit(cmd, addr);
  stat.readCount++;

  tick = cmd.finished;
}

void PALOLD::write(::CPDPBP &addr, uint64_t &tick) {
  ::Command cmd(tick, 0, OPER_WRITE, param.superPageSize);

  printCPDPBP(addr, "WRITE");
  pal->submit(cmd, addr);
  stat.writeCount++;

  tick = cmd.finished;
}

void PALOLD::erase(::CPDPBP &addr, uint64_t &tick) {
  ::Command cmd(tick, 0, OPER_ERASE, param.superPageSize * param.page);

  printCPDPBP(addr, "ERASE");
  pal->submit(cmd, addr);
  stat.eraseCount++;

  tick = cmd.finished;
}

}  // namespace PAL

}  // namespace SimpleSSD
