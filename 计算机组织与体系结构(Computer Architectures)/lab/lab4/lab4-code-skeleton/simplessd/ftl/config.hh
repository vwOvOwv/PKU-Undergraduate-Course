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

#ifndef __FTL_CONFIG__
#define __FTL_CONFIG__

#include "sim/base_config.hh"

namespace SimpleSSD {

namespace FTL {

typedef enum {
  /* Common FTL configuration */
  FTL_MAPPING_MODE,
  FTL_OVERPROVISION_RATIO,
  FTL_GC_THRESHOLD_RATIO,
  FTL_BAD_BLOCK_THRESHOLD,
  FTL_FILLING_MODE,
  FTL_FILL_RATIO,
  FTL_INVALID_PAGE_RATIO,
  FTL_GC_MODE,
  FTL_GC_RECLAIM_BLOCK,
  FTL_GC_RECLAIM_THRESHOLD,
  FTL_GC_EVICT_POLICY,
  FTL_GC_D_CHOICE_PARAM,
  FTL_USE_RANDOM_IO_TWEAK,

  /* N+K Mapping configuration*/
  FTL_NKMAP_N,
  FTL_NKMAP_K,
} FTL_CONFIG;

typedef enum {
  PAGE_MAPPING,
  FAST_MAPPING,
} MAPPING;

typedef enum {
  GC_MODE_0,  // Reclaim fixed number of blocks
  GC_MODE_1,  // Reclaim blocks until threshold
} GC_MODE;

typedef enum {
  FILLING_MODE_0,
  FILLING_MODE_1,
  FILLING_MODE_2,
} FILLING_MODE;

typedef enum {
  POLICY_GREEDY,  // Select the block with the least valid pages
  POLICY_COST_BENEFIT,
  POLICY_RANDOM,  // Select the block randomly
  POLICY_DCHOICE,
} EVICT_POLICY;

class Config : public BaseConfig {
 private:
  MAPPING mapping;             //!< Default: PAGE_MAPPING
  float overProvision;         //!< Default: 0.25 (25%)
  float gcThreshold;           //!< Default: 0.05 (5%)
  uint64_t badBlockThreshold;  //!< Default: 100000
  FILLING_MODE fillingMode;    //!< Default: FILLING_MODE_0
  float fillingRatio;          //!< Default: 0.0 (0%)
  float invalidRatio;          //!< Default: 0.0 (0%)
  uint64_t reclaimBlock;       //!< Default: 1
  float reclaimThreshold;      //!< Default: 0.1 (10%)
  GC_MODE gcMode;              //!< Default: FTL_GC_MODE_0
  EVICT_POLICY evictPolicy;    //!< Default: POLICY_GREEDY
  uint64_t dChoiceParam;       //!< Default: 3
  bool randomIOTweak;          //!< Default: true

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  int64_t readInt(uint32_t) override;
  uint64_t readUint(uint32_t) override;
  float readFloat(uint32_t) override;
  bool readBoolean(uint32_t) override;
};

}  // namespace FTL

}  // namespace SimpleSSD

#endif
