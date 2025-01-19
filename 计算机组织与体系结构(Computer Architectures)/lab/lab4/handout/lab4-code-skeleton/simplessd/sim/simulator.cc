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

#include "sim/simulator.hh"

namespace SimpleSSD {

// Defined in sim/simulator.hh
Simulator *sim = nullptr;

void setSimulator(Simulator *p) {
  sim = p;
}

uint64_t getTick() {
  if (sim) {
    return sim->getCurrentTick();
  }

  return 0;
}

Event allocate(EventFunction f) {
  if (sim) {
    return sim->allocateEvent(f);
  }

  return 0;
}

void schedule(Event e, uint64_t t) {
  if (sim) {
    sim->scheduleEvent(e, t);
  }
}

void deschedule(Event e) {
  if (sim) {
    sim->descheduleEvent(e);
  }
}

bool scheduled(Event e, uint64_t *p) {
  if (sim) {
    return sim->isScheduled(e, p);
  }

  return false;
}

void deallocate(Event e) {
  if (sim) {
    sim->deallocateEvent(e);
  }
}

}  // namespace SimpleSSD
