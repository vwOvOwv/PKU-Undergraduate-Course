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

#include "hil/nvme/abstract_subsystem.hh"

#include "hil/nvme/controller.hh"

namespace SimpleSSD {

namespace HIL {

namespace NVMe {

AbstractSubsystem::AbstractSubsystem(Controller *c, ConfigData &cfg)
    : pParent(c), cfgdata(cfg), conf(*cfg.pConfigReader) {}

AbstractSubsystem::~AbstractSubsystem() {}

}  // namespace NVMe

}  // namespace HIL

}  // namespace SimpleSSD
