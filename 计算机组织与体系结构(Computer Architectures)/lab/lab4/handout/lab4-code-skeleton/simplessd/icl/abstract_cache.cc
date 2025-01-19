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

#include "icl/abstract_cache.hh"

namespace SimpleSSD {

namespace ICL {

Line::_Line()
    : tag(0), lastAccessed(0), insertedAt(0), dirty(false), valid(false) {}

Line::_Line(uint64_t t, bool d)
    : tag(t), lastAccessed(0), insertedAt(0), dirty(d), valid(true) {}

AbstractCache::AbstractCache(ConfigReader &c, FTL::FTL *f,
                             DRAM::AbstractDRAM *d)
    : conf(c), pFTL(f), pDRAM(d) {}

AbstractCache::~AbstractCache() {}

}  // namespace ICL

}  // namespace SimpleSSD
