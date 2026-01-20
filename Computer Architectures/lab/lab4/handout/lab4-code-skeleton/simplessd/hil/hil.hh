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

#ifndef __HIL_HIL__
#define __HIL_HIL__

#include <queue>

#include "icl/icl.hh"
#include "sim/dma_interface.hh"
#include "util/simplessd.hh"

namespace SimpleSSD {

namespace HIL {

class HIL : public StatObject {
 private:
  ConfigReader &conf;
  ICL::ICL *pICL;

  uint64_t reqCount;

  uint64_t lastScheduled;
  Event completionEvent;
  std::priority_queue<Request, std::vector<Request>, Request> completionQueue;

  struct {
    uint64_t request[2];
    uint64_t busy[3];
    uint64_t iosize[2];
    uint64_t lastBusyAt[3];
  } stat;

  void updateBusyTime(int, uint64_t, uint64_t);
  void updateCompletion();
  void completion();

 public:
  HIL(ConfigReader &);
  ~HIL();

  void read(Request &);
  void write(Request &);
  void flush(Request &);
  void trim(Request &);

  void format(Request &, bool);

  void getLPNInfo(uint64_t &, uint32_t &);
  uint64_t getUsedPageCount(uint64_t, uint64_t);

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace HIL

}  // namespace SimpleSSD

#endif
