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

#ifndef __DRAM_CONFIG__
#define __DRAM_CONFIG__

#include "sim/base_config.hh"

namespace SimpleSSD {

namespace DRAM {

typedef enum {
  DRAM_MODEL,
} DRAM_CONFIG;

typedef enum {
  SIMPLE_MODEL,
} MODEL;

class Config : public BaseConfig {
 public:
  typedef struct {
    uint32_t channel;   //!< # of channel
    uint32_t rank;      //!< Rank / Channel
    uint32_t bank;      //!< Bank / Rank
    uint32_t chip;      //!< Chips / Rank
    uint32_t busWidth;  //!< Bus / Chip
    uint32_t burstLength;
    uint32_t activationLimit;
    bool useDLL;
    uint64_t chipSize;
    uint64_t pageSize;
  } DRAMStructure;

  typedef struct {    // Unit: ps
    uint32_t tCK;     //!< Clock period
    uint32_t tRCD;    //!< RAS to CAS delay
    uint32_t tCL;     //!< CAS latency
    uint32_t tRP;     //!< Row precharge time
    uint32_t tRAS;    //!< ACT to PRE delay
    uint32_t tWR;     //!< Write recovery time
    uint32_t tRTP;    //!< Read to precharge delay
    uint32_t tBURST;  //!< Burst duration
    uint32_t tCCD_L;  //!< Same bank group CAS to CAS delay
    uint32_t tRFC;    //!< Refresh cycle time
    uint32_t tREFI;   //!< Refresh command interval
    uint32_t tWTR;    //!< Write to read, same rank switching time
    uint32_t tRTW;    //!< Read to write, same rank switching time
    uint32_t tCS;     //!< Rank to rank switching time
    uint32_t tRRD;    //!< ACT to ACT delay
    uint32_t tRRD_L;  //!< Same bank group RRD
    uint32_t tXAW;    //!< X activation window
    uint32_t tXP;     //!< Power-up delay
    uint32_t tXPDLL;  //!< Power-up delay with locked DLL
    uint32_t tXS;     //!< Self-refresh exit latency
    uint32_t tXSDLL;  //!< Self-refresh exit latency DLL
  } DRAMTiming;

  typedef struct {     // Unit: mA
    float pIDD0[2];    //!< Active precharge current
    float pIDD2P0[2];  //!< Precharge powerdown slow
    float pIDD2P1[2];  //!< Precharge powerdown fast
    float pIDD2N[2];   //!< Precharge standby current
    float pIDD3P0[2];  //!< Active powerdown slow
    float pIDD3P1[2];  //!< Active powerdown fast
    float pIDD3N[2];   //!< Active standby current
    float pIDD4R[2];   //!< READ current
    float pIDD4W[2];   //!< WRITE current
    float pIDD5[2];    //!< Refresh current
    float pIDD6[2];    //!< Self-refresh current
    float pVDD[2];     //!< Main voltage (Unit: V)
  } DRAMPower;

 private:
  MODEL model;

  DRAMStructure dram;
  DRAMTiming dramTiming;
  DRAMPower dramPower;

 public:
  Config();

  bool setConfig(const char *, const char *) override;

  int64_t readInt(uint32_t) override;

  DRAMStructure *getDRAMStructure();
  DRAMTiming *getDRAMTiming();
  DRAMPower *getDRAMPower();
};

}  // namespace DRAM

}  // namespace SimpleSSD

#endif
