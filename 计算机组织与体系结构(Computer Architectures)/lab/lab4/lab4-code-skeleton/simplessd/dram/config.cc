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

#include "dram/config.hh"

#include "util/algorithm.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace DRAM {

const char NAME_DRAM_MODEL[] = "Model";
const char NAME_DRAM_STRUCTURE_CHANNEL[] = "Channel";
const char NAME_DRAM_STRUCTURE_RANK[] = "Rank";
const char NAME_DRAM_STRUCTURE_BANK[] = "Bank";
const char NAME_DRAM_STRUCTURE_CHIP[] = "Chip";
const char NAME_DRAM_STRUCTURE_BUS_WIDTH[] = "BusWidth";
const char NAME_DRAM_STRUCTURE_BURST_LENGTH[] = "BurstLength";
const char NAME_DRAM_STRUCTURE_CHIP_SIZE[] = "ChipSize";
const char NAME_DRAM_STRUCTURE_PAGE_SIZE[] = "PageSize";
const char NAME_DRAM_TIMING_CK[] = "tCK";
const char NAME_DRAM_TIMING_RCD[] = "tRCD";
const char NAME_DRAM_TIMING_CL[] = "tCL";
const char NAME_DRAM_TIMING_RP[] = "tRP";
const char NAME_DRAM_TIMING_RAS[] = "tRAS";
const char NAME_DRAM_TIMING_WR[] = "tWR";
const char NAME_DRAM_TIMING_RTP[] = "tRTP";
const char NAME_DRAM_TIMING_BURST[] = "tBURST";
const char NAME_DRAM_TIMING_CCD_L[] = "tCCD_L";
const char NAME_DRAM_TIMING_RFC[] = "tRFC";
const char NAME_DRAM_TIMING_REFI[] = "tREFI";
const char NAME_DRAM_TIMING_WTR[] = "tWTR";
const char NAME_DRAM_TIMING_RTW[] = "tRTW";
const char NAME_DRAM_TIMING_CS[] = "tCS";
const char NAME_DRAM_TIMING_RRD[] = "tRRD";
const char NAME_DRAM_TIMING_RRD_L[] = "tRRD_L";
const char NAME_DRAM_TIMING_XAW[] = "tXAW";
const char NAME_DRAM_TIMING_XP[] = "tXP";
const char NAME_DRAM_TIMING_XPDLL[] = "tXPDLL";
const char NAME_DRAM_TIMING_XS[] = "tXS";
const char NAME_DRAM_TIMING_XSDLL[] = "tXSDLL";
const char NAME_DRAM_POWER_IDD0_0[] = "IDD0_0";
const char NAME_DRAM_POWER_IDD0_1[] = "IDD0_1";
const char NAME_DRAM_POWER_IDD2P0_0[] = "IDD2P0_0";
const char NAME_DRAM_POWER_IDD2P0_1[] = "IDD2P0_1";
const char NAME_DRAM_POWER_IDD2P1_0[] = "IDD2P1_0";
const char NAME_DRAM_POWER_IDD2P1_1[] = "IDD2P1_1";
const char NAME_DRAM_POWER_IDD2N_0[] = "IDD2N_0";
const char NAME_DRAM_POWER_IDD2N_1[] = "IDD2N_1";
const char NAME_DRAM_POWER_IDD3P0_0[] = "IDD3P0_0";
const char NAME_DRAM_POWER_IDD3P0_1[] = "IDD3P0_1";
const char NAME_DRAM_POWER_IDD3P1_0[] = "IDD3P1_0";
const char NAME_DRAM_POWER_IDD3P1_1[] = "IDD3P1_1";
const char NAME_DRAM_POWER_IDD3N_0[] = "IDD3N_0";
const char NAME_DRAM_POWER_IDD3N_1[] = "IDD3N_1";
const char NAME_DRAM_POWER_IDD4R_0[] = "IDD4R_0";
const char NAME_DRAM_POWER_IDD4R_1[] = "IDD4R_1";
const char NAME_DRAM_POWER_IDD4W_0[] = "IDD4W_0";
const char NAME_DRAM_POWER_IDD4W_1[] = "IDD4W_1";
const char NAME_DRAM_POWER_IDD5_0[] = "IDD5_0";
const char NAME_DRAM_POWER_IDD5_1[] = "IDD5_1";
const char NAME_DRAM_POWER_IDD6_0[] = "IDD6_0";
const char NAME_DRAM_POWER_IDD6_1[] = "IDD6_1";
const char NAME_DRAM_POWER_VDD_0[] = "VDD_0";
const char NAME_DRAM_POWER_VDD_1[] = "VDD_1";

Config::Config() {
  model = SIMPLE_MODEL;

  /* LPDDR3-1600 4Gbit 1x32 */
  dram.channel = 1;
  dram.rank = 1;
  dram.bank = 8;
  dram.chip = 1;
  dram.chipSize = 536870912;
  dram.busWidth = 32;
  dram.burstLength = 8;
  dram.activationLimit = 4;
  dram.useDLL = false;
  dram.pageSize = 4096;

  dramTiming.tCK = 1250;
  dramTiming.tRCD = 18000;
  dramTiming.tCL = 15000;
  dramTiming.tRP = 18000;
  dramTiming.tRAS = 42000;
  dramTiming.tWR = 15000;
  dramTiming.tRTP = 7500;
  dramTiming.tBURST = 5000;
  dramTiming.tCCD_L = 0;
  dramTiming.tRFC = 130000;
  dramTiming.tREFI = 3900;
  dramTiming.tWTR = 7500;
  dramTiming.tRTW = 2500;
  dramTiming.tCS = 2500;
  dramTiming.tRRD = 10000;
  dramTiming.tRRD_L = 0;
  dramTiming.tXAW = 50000;
  dramTiming.tXP = 0;
  dramTiming.tXPDLL = 0;
  dramTiming.tXS = 0;
  dramTiming.tXSDLL = 0;

  dramPower.pIDD0[0] = 8.f;
  dramPower.pIDD0[1] = 60.f;
  dramPower.pIDD2P0[0] = 0.f;
  dramPower.pIDD2P0[1] = 0.f;
  dramPower.pIDD2P1[0] = 0.8f;
  dramPower.pIDD2P1[1] = 1.8f;
  dramPower.pIDD2N[0] = 0.8f;
  dramPower.pIDD2N[1] = 26.f;
  dramPower.pIDD3P0[0] = 0.f;
  dramPower.pIDD3P0[1] = 0.f;
  dramPower.pIDD3P1[0] = 1.4f;
  dramPower.pIDD3P1[1] = 11.f;
  dramPower.pIDD3N[0] = 2.f;
  dramPower.pIDD3N[1] = 34.f;
  dramPower.pIDD4R[0] = 2.f;
  dramPower.pIDD4R[1] = 230.f;
  dramPower.pIDD4W[0] = 2.f;
  dramPower.pIDD4W[1] = 190.f;
  dramPower.pIDD5[0] = 28.f;
  dramPower.pIDD5[1] = 150.f;
  dramPower.pIDD6[0] = 0.5f;
  dramPower.pIDD6[1] = 1.8f;
  dramPower.pVDD[0] = 1.8f;
  dramPower.pVDD[1] = 1.2f;
}

bool Config::setConfig(const char *name, const char *value) {
  bool ret = true;

  if (MATCH_NAME(NAME_DRAM_MODEL)) {
    model = (MODEL)strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_STRUCTURE_CHANNEL)) {
    dram.channel = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_STRUCTURE_RANK)) {
    dram.rank = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_STRUCTURE_BANK)) {
    dram.bank = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_STRUCTURE_CHIP)) {
    dram.chip = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_STRUCTURE_BUS_WIDTH)) {
    dram.busWidth = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_STRUCTURE_BURST_LENGTH)) {
    dram.burstLength = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_STRUCTURE_CHIP_SIZE)) {
    dram.chipSize = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_STRUCTURE_PAGE_SIZE)) {
    dram.pageSize = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_CK)) {
    dramTiming.tCK = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_RCD)) {
    dramTiming.tRCD = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_CL)) {
    dramTiming.tCL = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_RP)) {
    dramTiming.tRP = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_RAS)) {
    dramTiming.tRAS = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_WR)) {
    dramTiming.tWR = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_RTP)) {
    dramTiming.tRTP = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_BURST)) {
    dramTiming.tBURST = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_CCD_L)) {
    dramTiming.tCCD_L = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_RFC)) {
    dramTiming.tRFC = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_REFI)) {
    dramTiming.tREFI = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_WTR)) {
    dramTiming.tWTR = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_RTW)) {
    dramTiming.tRTW = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_CS)) {
    dramTiming.tCS = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_RRD)) {
    dramTiming.tRRD = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_RRD_L)) {
    dramTiming.tRRD_L = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_XAW)) {
    dramTiming.tXAW = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_XP)) {
    dramTiming.tXP = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_XPDLL)) {
    dramTiming.tXPDLL = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_XS)) {
    dramTiming.tXS = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_TIMING_XSDLL)) {
    dramTiming.tXSDLL = strtoul(value, nullptr, 10);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD0_0)) {
    dramPower.pIDD0[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD0_1)) {
    dramPower.pIDD0[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD2P0_0)) {
    dramPower.pIDD2P0[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD2P0_1)) {
    dramPower.pIDD2P0[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD2P1_0)) {
    dramPower.pIDD2P1[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD2P1_1)) {
    dramPower.pIDD2P1[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD2N_0)) {
    dramPower.pIDD2N[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD2N_1)) {
    dramPower.pIDD2N[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD3P0_0)) {
    dramPower.pIDD3P0[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD3P0_1)) {
    dramPower.pIDD3P0[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD3P1_0)) {
    dramPower.pIDD3P1[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD3P1_1)) {
    dramPower.pIDD3P1[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD3N_0)) {
    dramPower.pIDD3N[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD3N_1)) {
    dramPower.pIDD3N[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD4R_0)) {
    dramPower.pIDD4R[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD4R_1)) {
    dramPower.pIDD4R[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD4W_0)) {
    dramPower.pIDD4W[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD4W_1)) {
    dramPower.pIDD4W[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD5_0)) {
    dramPower.pIDD5[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD5_1)) {
    dramPower.pIDD5[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD6_0)) {
    dramPower.pIDD6[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_IDD6_1)) {
    dramPower.pIDD6[1] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_VDD_0)) {
    dramPower.pVDD[0] = strtof(value, nullptr);
  }
  else if (MATCH_NAME(NAME_DRAM_POWER_VDD_1)) {
    dramPower.pVDD[1] = strtof(value, nullptr);
  }
  else {
    ret = false;
  }

  return ret;
}

int64_t Config::readInt(uint32_t idx) {
  int64_t ret = 0;

  switch (idx) {
    case DRAM_MODEL:
      ret = model;
      break;
  }

  return ret;
}

Config::DRAMStructure *Config::getDRAMStructure() {
  return &dram;
}

Config::DRAMTiming *Config::getDRAMTiming() {
  return &dramTiming;
}

Config::DRAMPower *Config::getDRAMPower() {
  return &dramPower;
}

}  // namespace DRAM

}  // namespace SimpleSSD
