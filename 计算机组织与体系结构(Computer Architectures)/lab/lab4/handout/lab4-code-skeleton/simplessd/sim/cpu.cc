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

#include "sim/cpu.hh"

namespace SimpleSSD {

// Defined in sim/cpu.hh
CPU::CPU *cpu = nullptr;
DMAFunction cpuHandler = commonCPUHandler;

CPUContext::_CPUContext(DMAFunction &f, void *c) : func(f), context(c) {}

CPUContext::_CPUContext(DMAFunction &f, void *c, CPU::NAMESPACE n,
                        CPU::FUNCTION fc)
    : func(f), context(c), ns(n), fct(fc), delay(0) {}

CPUContext::_CPUContext(DMAFunction &f, void *c, CPU::NAMESPACE n,
                        CPU::FUNCTION fc, uint64_t d)
    : func(f), context(c), ns(n), fct(fc), delay(d) {}

void initCPU(ConfigReader &conf) {
  if (cpu) {
    delete cpu;
  }

  cpu = new CPU::CPU(conf);
}

void deInitCPU() {
  delete cpu;
  cpu = nullptr;
}

void getCPUStatList(std::vector<Stats> &list, std::string prefix) {
  if (cpu) {
    cpu->getStatList(list, prefix);
  }
}

void getCPUStatValues(std::vector<double> &values) {
  if (cpu) {
    cpu->getStatValues(values);
  }
}

void resetCPUStatValues() {
  if (cpu) {
    cpu->resetStatValues();
  }
}

void printCPULastStat() {
  if (cpu) {
    cpu->printLastStat();
  }
}

void execute(CPU::NAMESPACE ns, CPU::FUNCTION fct, DMAFunction &func,
             void *context, uint64_t delay) {
  if (cpu) {
    cpu->execute(ns, fct, func, context, delay);
  }
}

void commonCPUHandler(uint64_t, void *context) {
  CPUContext *pContext = (CPUContext *)context;

  execute(pContext->ns, pContext->fct, pContext->func, pContext->context,
          pContext->delay);

  delete pContext;
}

uint64_t applyLatency(CPU::NAMESPACE ns, CPU::FUNCTION fct) {
  if (cpu) {
    return cpu->applyLatency(ns, fct);
  }

  return 0;
}

}  // namespace SimpleSSD
