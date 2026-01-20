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

#pragma once

#ifndef __CPU_CONFIG__
#define __CPU_CONFIG__

#include "sim/base_config.hh"

namespace SimpleSSD {

namespace CPU {

typedef enum {
  CPU_CLOCK,
  CPU_CORE_HIL,
  CPU_CORE_ICL,
  CPU_CORE_FTL,
} CPU_CONFIG;

class Config : public BaseConfig {
 private:
  uint64_t clock;    //!< Default: 400MHz
  uint32_t hilCore;  //!< Default: 1
  uint32_t iclCore;  //!< Default: 1
  uint32_t ftlCore;  //!< Default: 1

 public:
  Config();

  bool setConfig(const char *, const char *) override;
  void update() override;

  uint64_t readUint(uint32_t) override;
};

}  // namespace CPU

}  // namespace SimpleSSD

#endif
