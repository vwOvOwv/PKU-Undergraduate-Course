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

#include "sim/engine.hh"

#include "simplessd/sim/trace.hh"

Engine::Engine()
    : SimpleSSD::Simulator(),
      simTick(0),
      counter(0),
      forceStop(false),
      eventHandled(0) {
  watch.start();
}

Engine::~Engine() {}

bool Engine::insertEvent(SimpleSSD::Event eid, uint64_t tick,
                         uint64_t *pOldTick) {
  bool found = false;
  bool flag = false;
  auto old = eventQueue.begin();
  auto insert = eventQueue.end();

  for (auto iter = eventQueue.begin(); iter != eventQueue.end(); iter++) {
    if (iter->first == eid) {
      found = true;
      old = iter;

      if (pOldTick) {
        *pOldTick = iter->second;
      }
    }

    if (iter->second > tick && !flag) {
      insert = iter;
      flag = true;
    }
  }

  if (found && pOldTick) {
    if (*pOldTick == tick) {
      // Rescheduling to same tick. Ignore.
      return false;
    }
  }

  // Iterator will not invalidated on insert
  // Do insert first
  eventQueue.insert(insert, {eid, tick});

  if (found) {
    eventQueue.erase(old);
  }

  return found;
}

bool Engine::removeEvent(SimpleSSD::Event eid) {
  bool found = false;

  for (auto iter = eventQueue.begin(); iter != eventQueue.end(); iter++) {
    if (iter->first == eid) {
      eventQueue.erase(iter);
      found = true;

      break;
    }
  }

  return found;
}

bool Engine::isEventExist(SimpleSSD::Event eid, uint64_t *pTick) {
  for (auto &iter : eventQueue) {
    if (iter.first == eid) {
      if (pTick) {
        *pTick = iter.second;
      }

      return true;
    }
  }

  return false;
}

uint64_t Engine::getCurrentTick() {
  std::lock_guard<std::mutex> guard(mTick);

  return simTick;
}

SimpleSSD::Event Engine::allocateEvent(SimpleSSD::EventFunction func) {
  auto iter = eventList.insert({++counter, func});

  if (!iter.second) {
    SimpleSSD::panic("Fail to allocate event");
  }

  return counter;
}

void Engine::scheduleEvent(SimpleSSD::Event eid, uint64_t tick) {
  auto iter = eventList.find(eid);

  if (iter != eventList.end()) {
    uint64_t tickCopy;

    {
      std::lock_guard<std::mutex> guard(mTick);
      tickCopy = simTick;
    }

    if (tick < tickCopy) {
      SimpleSSD::warn("Tried to schedule %" PRIu64
                      " < simTick to event %" PRIu64 ". Set tick as simTick.",
                      tick, eid);

      tick = tickCopy;
    }

    uint64_t oldTick;

    if (insertEvent(eid, tick, &oldTick)) {
      SimpleSSD::warn("Event %" PRIu64 " rescheduled from %" PRIu64
                      " to %" PRIu64,
                      eid, oldTick, tick);
    }
  }
  else {
    SimpleSSD::panic("Event %" PRIu64 " does not exists", eid);
  }
}

void Engine::descheduleEvent(SimpleSSD::Event eid) {
  auto iter = eventList.find(eid);

  if (iter != eventList.end()) {
    removeEvent(eid);
  }
  else {
    SimpleSSD::panic("Event %" PRIu64 " does not exists", eid);
  }
}

bool Engine::isScheduled(SimpleSSD::Event eid, uint64_t *pTick) {
  bool ret = false;
  auto iter = eventList.find(eid);

  if (iter != eventList.end()) {
    ret = isEventExist(eid, pTick);
  }
  else {
    SimpleSSD::panic("Event %" PRIu64 " does not exists", eid);
  }

  return ret;
}

void Engine::deallocateEvent(SimpleSSD::Event eid) {
  auto iter = eventList.find(eid);

  if (iter != eventList.end()) {
    removeEvent(eid);
    eventList.erase(iter);
  }
  else {
    SimpleSSD::panic("Event %" PRIu64 " does not exists", eid);
  }
}

bool Engine::doNextEvent() {
  uint64_t tickCopy;

  if (forceStop) {
    return false;
  }

  if (eventQueue.size() > 0) {
    auto &now = eventQueue.front();

    {
      std::lock_guard<std::mutex> guard(mTick);

      simTick = now.second;
      tickCopy = simTick;
    }

    auto iter = eventList.find(now.first);

    eventQueue.pop_front();

    if (iter != eventList.end()) {
      iter->second(tickCopy);
    }
    else {
      SimpleSSD::panic("Event %" PRIu64 " does not exists", now.first);
    }

    {
      std::lock_guard<std::mutex> guard(m);
      eventHandled++;
    }

    return true;
  }

  return false;
}

void Engine::stopEngine() {
  forceStop = true;
}

void Engine::printStats(std::ostream &out) {
  watch.stop();

  double duration = watch.getDuration();

  out << "*** Statistics of Event Engine ***" << std::endl;
  {
    std::lock_guard<std::mutex> guard(mTick);

    out << "Simulation Tick (ps): " << simTick << std::endl;
  }
  out << "Host time duration (sec): " << std::to_string(duration) << std::endl;
  out << "Event handled: " << eventHandled << " ("
      << std::to_string(eventHandled / duration) << " ops)" << std::endl;
  out << "*** End of statistics ***" << std::endl;
}

void Engine::getStat(uint64_t &val) {
  std::lock_guard<std::mutex> guard(m);
  val = eventHandled;
}
