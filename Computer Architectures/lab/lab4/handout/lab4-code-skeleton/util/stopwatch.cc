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

#include "util/stopwatch.hh"

Stopwatch::Stopwatch() {}

Stopwatch::~Stopwatch() {}

double Stopwatch::getTime() {
  return std::chrono::duration_cast<std::chrono::duration<double>>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

void Stopwatch::start() {
  begin = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop() {
  end = std::chrono::high_resolution_clock::now();
}

double Stopwatch::getDuration() {
  return std::chrono::duration_cast<std::chrono::duration<double>>(end - begin)
      .count();
}
