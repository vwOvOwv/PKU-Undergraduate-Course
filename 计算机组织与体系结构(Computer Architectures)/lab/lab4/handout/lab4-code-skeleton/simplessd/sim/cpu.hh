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

#ifndef __SIM_CPU__
#define __SIM_CPU__

#include <cinttypes>
#include <functional>

#include "cpu/cpu.hh"

namespace SimpleSSD {

typedef struct _CPUContext {
  DMAFunction func;
  void *context;

  CPU::NAMESPACE ns;
  CPU::FUNCTION fct;
  uint64_t delay;

  _CPUContext(DMAFunction &, void *);
  _CPUContext(DMAFunction &, void *, CPU::NAMESPACE, CPU::FUNCTION);
  _CPUContext(DMAFunction &, void *, CPU::NAMESPACE, CPU::FUNCTION, uint64_t);
} CPUContext;

void initCPU(ConfigReader &);
void deInitCPU();
void getCPUStatList(std::vector<Stats> &, std::string);
void getCPUStatValues(std::vector<double> &);
void resetCPUStatValues();
void printCPULastStat();

void execute(CPU::NAMESPACE, CPU::FUNCTION, DMAFunction &, void * = nullptr,
             uint64_t = 0);
uint64_t applyLatency(CPU::NAMESPACE, CPU::FUNCTION);

void commonCPUHandler(uint64_t, void *);

// Defined at sim/cpu.cc
extern DMAFunction cpuHandler;

}  // namespace SimpleSSD

#endif
