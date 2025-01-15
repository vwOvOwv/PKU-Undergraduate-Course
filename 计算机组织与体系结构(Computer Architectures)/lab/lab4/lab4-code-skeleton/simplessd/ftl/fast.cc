#include "ftl/fast.hh"

#include <algorithm>
#include <limits>
#include <random>

#include "util/algorithm.hh"
#include "util/bitset.hh"

namespace SimpleSSD {

namespace FTL {

/* ======== Lab4 TODO: FUNCTIONS REQUIRE IMPLEMENTATION ======== */

void FASTMapping::invalidateOldPage(LBN lbn, POff offset) {
  // 1. Check if the page is in RW blocks
  for (auto &pbn : RWBlockList) {
    if (!PMT.find_block(pbn)) {
      panic("RWBlock is not in use");
    }
    POff poff = PMT.find_page(pbn, lbn * pagePerBlock + offset);
    if (poff != -1) {
      PMT.update_page(pbn, -1, poff);
      Block *block = usedBlocks.find(pbn);
      if (block != nullptr && block->checkValid(offset, 0)) {
        block->invalidate(poff, 0);
      }
    }
  }
  // 2. Check if the page is in SW block
  // TODO: Lab4
  // 3. Check if the page is in data blocks
  // TODO: Lab4
}

std::pair<PBN, POff> FASTMapping::findPageInRWBlocks(LPN lpn) {
  // TODO: Lab4
}
bool FASTMapping::findPageInSWBlock(LBN lbn, POff offset) {
  // TODO: Lab4
}
PBN FASTMapping::findPageInDataBlocks(LBN lbn, POff offset) {
  // TODO: Lab4
}
bool FASTMapping::pageIsFreeInDataBlock(LBN lbn, POff offset) {
  // TODO: Lab4
}
bool FASTMapping::canWriteToSWBlockAfterMerge(LBN lbn, POff offset,
                                              uint64_t &tick, bool sendToPAL) {
  // 0. Check if there is a valid SW block, if not, get a new one
  uint64_t beginAt = tick;
  uint64_t finishedAt = tick;
  if (!bSWBlockValid) {
    auto newsw = freeBlocks.pop();
    PBN newPBN = newsw.getBlockIndex();
    PMT.new_block(newPBN, pagePerBlock);
    usedBlocks.insert(newPBN, newsw);
    SWBlockPBN = newPBN;
    bSWBlockValid = true;
  }
  auto &mapping_list = PMT.get_pages(SWBlockPBN);

  // 1. offset == 0
  if (offset == 0) {
    // If exist page at 0, need merge
    if (mapping_list[0] != -1) {
      // Perform merge.
      PAL::Request palRequest(param.ioUnitInPage);
      palRequest.ioFlag.set(); // Lab4 NOTE: This is necessary!
      std::vector<PAL::Request> readRequests;
      std::vector<PAL::Request> writeRequests;
      std::vector<PAL::Request> eraseRequests;

      int64_t valid_count = 0;
      for (auto &lpn : mapping_list) {
        if (lpn != -1) {
          valid_count++;
        }
      }
      // TODO: Lab4
      // 1.0: Find the original data block
      // 1.1: If SW is full, switch it to a data block and erase the old one
        // 1.1.1: Modify the mapping table
        // 1.1.2: invalidate SW log block
        // 1.1.3: erase the original data block
        // 1.1.3 Hint: You can use the following codes to erase the block.
        /*
          palRequest.blockIndex = dataPBN;
          palRequest.pageIndex = 0;
          eraseRequests.push_back(palRequest);
        */
      // 1.2: If SW is not full, merge it with the original data block
        // 1.2.1: read all valid pages in SW block and data block
        // 1.2.3: find a new free block for the merged block
        // 1.2.4: write all valid pages to the new block
        // 1.2.4 Hint: The write codes can be similar to erase codes, but need
        // additional operations on Block class like these:
        /*
          block->write(i, lbn * pagePerBlock + offset, 0, tick);
        */
        // 1.2.5: modify the mapping table
        // 1.2.6: invalidate SW log block
        // 1.2.7: erase the original data block and SW block
      // 1.3: perform the merge PAL requests
      performPALRequestForMerge(readRequests, writeRequests, eraseRequests,
                                beginAt, sendToPAL);
      // 1.4: get a new SW block
    } 
    // If SW block is empty, we can diretly write, but need update SWBlockLBN
    else if (SWBlockLBN == -1) {
      SWBlockLBN = lbn;
    }
    // After merge, we can write
    // If mapping list exist no valid page, also can directly write
    finishedAt = MAX(finishedAt, beginAt);
    tick = finishedAt;
    return true;
  }
  // 2. offset == next free page in SW block
  // 3. not 1 or 2, can not write to SW block
  else {
    // TODO: Lab4
    return false; // Lab4: Should be modified
  }
}
std::pair<PBN, POff> FASTMapping::findFreePageInRWBlocks(uint64_t &tick,
                                                         bool sendToPAL) {
  // TODO: Lab4
  // 1. Find a not full RW block
    // 1.1: If find a not full RW block, return the first free page、

  // 2: If all RW blocks are full, check if RW block count is max
    // 2.1: If not, get a new RW block

  // 3: If RW block count is max, merge the RW block
    // 3.1: Pop front the RW block
    // 3.2: Find all corresponding LBN in the RW block
    // 3.3: Find all valid pages of these LBNs in all RW blocks,
    // read them out, write them to new data blocks, and invalidate them
    // 3.4: Find all valid pages of these LBNs in corresponding data blocks
    // read them out, write them to new data blocks, and erase these blocks
    // 3.5: Erase the RW block
    // 3.6: Perform the merge PAL requests
    // 3.7: Get a new RW block
    // 3.8: Return the first free page
  
  // Hint: the read, write, erase and perform merge codes can be similar to
  // the codes in canWriteToSWBlockAfterMerge().

  return {-1, -1}; // Lab4: Should be modified
}

/* ======== Lab4: FUNCTIONS ALREADY IMPLEMENTED ======== */

void FASTMapping::performPALRequestForMerge(std::vector<PAL::Request> &reads,
                                            std::vector<PAL::Request> &writes,
                                            std::vector<PAL::Request> &erases,
                                            uint64_t &tick, bool sendToPAL) {
  if (!sendToPAL) {
    return;
  }
  uint64_t beginAt;
  uint64_t readFinishedAt = tick;
  uint64_t writeFinishedAt = tick;
  uint64_t eraseFinishedAt = tick;

  for (auto &iter : reads) {
    beginAt = tick;
    pPAL->read(iter, beginAt);
    readFinishedAt = MAX(readFinishedAt, beginAt);
  }
  for (auto &iter : writes) {
    beginAt = readFinishedAt;
    pPAL->write(iter, beginAt);
    writeFinishedAt = MAX(writeFinishedAt, beginAt);
  }
  for (auto &iter : erases) {
    beginAt = writeFinishedAt;
    eraseInternal(iter, beginAt);
    eraseFinishedAt = MAX(eraseFinishedAt, beginAt);
  }
  tick = eraseFinishedAt;
}

void FASTMapping::readInternal(Request &req, uint64_t &tick) {
  PAL::Request palRequest(req);
  palRequest.ioFlag.set();
  uint64_t beginAt;
  uint64_t finishedAt = tick;

  LBN lbn = req.lpn / pagePerBlock;
  POff poff = req.lpn % pagePerBlock;

  bool found_mapping = false;

  // ======== check in RW log blocks ========
  auto mapping_pair = findPageInRWBlocks(req.lpn);
  if (mapping_pair.first != -1 && mapping_pair.second != -1) {
    found_mapping = true;
    palRequest.blockIndex = mapping_pair.first;
    palRequest.pageIndex = mapping_pair.second;
  }
  // ======== check in SW log block ========
  if (!found_mapping) {
    if (findPageInSWBlock(lbn, poff)) {
      found_mapping = true;
      palRequest.blockIndex = SWBlockPBN;
      palRequest.pageIndex = poff;
    }
  }
  // ======== check in data blocks ========
  if (!found_mapping) {
    auto PBN = findPageInDataBlocks(lbn, poff);
    if (PBN != -1) {
      found_mapping = true;
      palRequest.blockIndex = PBN;
      palRequest.pageIndex = poff;
    }
  }
  // ======== perform read ========
  if (found_mapping) {
    beginAt = tick;
    palRequest.ioFlag.set();

    Block *block = usedBlocks.find(palRequest.blockIndex);
    if (block == nullptr) {
      panic("Block is not in use");
    }
    block->read(palRequest.pageIndex, 0, beginAt);
    pPAL->read(palRequest, beginAt);

    finishedAt = MAX(finishedAt, beginAt);
    tick = finishedAt;
    tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::READ_INTERNAL);
  }
}

void FASTMapping::writeInternal(Request &req, uint64_t &tick, bool sendToPAL) {
  PAL::Request palRequest(req);
  palRequest.ioFlag.set();
  uint64_t beginAt = tick;
  uint64_t finishedAt = tick;

  LBN lbn = req.lpn / pagePerBlock;
  POff poff = req.lpn % pagePerBlock;

  // ======== check data block, if the page is free ========
  if (pageIsFreeInDataBlock(lbn, poff)) {
    auto PBN = BMT.find(lbn);
    assert(PBN != -1);
    palRequest.blockIndex = PBN;
    palRequest.pageIndex = poff;
  }
  // ======== check SW log block for sequential write ========
  else if (canWriteToSWBlockAfterMerge(lbn, poff, beginAt, sendToPAL)) {
    assert(bSWBlockValid);
    palRequest.blockIndex = SWBlockPBN;
    palRequest.pageIndex = poff;
    invalidateOldPage(lbn, poff);
    PMT.update_page(SWBlockPBN, req.lpn, poff);
  }
  // ======== write in RW log block for other condition ========
  else {
    auto mapping_pair = findFreePageInRWBlocks(beginAt, sendToPAL);
    assert(mapping_pair.first != -1 && mapping_pair.second != -1);
    palRequest.blockIndex = mapping_pair.first;
    palRequest.pageIndex = mapping_pair.second;
    invalidateOldPage(lbn, poff);
    PMT.update_page(palRequest.blockIndex, lbn * pagePerBlock + poff, poff);
  }
  // ======== perform write ========
  palRequest.ioFlag.set();

  Block *block = usedBlocks.find(palRequest.blockIndex);
  if (block == nullptr) {
    panic("Block is not in use");
  }
  block->write(palRequest.pageIndex, req.lpn, 0, beginAt);
  if (sendToPAL)
    pPAL->write(palRequest, beginAt);

  finishedAt = MAX(finishedAt, beginAt);
  tick = finishedAt;
  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::WRITE_INTERNAL);
}

void FASTMapping::eraseInternal(PAL::Request &req, uint64_t &tick) {
  Block *block = usedBlocks.find(req.blockIndex);
  if (block == nullptr) {
    panic("Block is not in use");
  }

  // Erase block
  block->erase();
  pPAL->erase(req, tick);

  // Insert to free block list
  freeBlocks.insert(*block);
  // Remove from used block list
  usedBlocks.erase(req.blockIndex);

  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::ERASE_INTERNAL);
}

/* ======== Lab4: HELPER CLASSES ======== */

void BlockMappingTable::update(LBN lbn, PBN pbn) {
  if (table.find(lbn) != table.end()) {
    table[lbn] = pbn;
  }
  else {
    table.insert({lbn, pbn});
  }
}

void BlockMappingTable::invalidate(LBN lbn) {
  table.erase(lbn);
}

PBN BlockMappingTable::find(LBN lbn) {
  auto iter = table.find(lbn);
  if (iter != table.end()) {
    return iter->second;
  }
  else {
    return -1;
  }
}

void PageMappingTable::new_block(PBN pbn, uint64_t pageCount) {
  if (table.find(pbn) != table.end()) {
    panic("Log block is already in use");
  }
  table.insert({pbn, std::vector<LPN>(pageCount, -1)});
}

bool PageMappingTable::find_block(PBN pbn) {
  return table.find(pbn) != table.end();
}

void PageMappingTable::invalid_block(PBN pbn) {
  table.erase(pbn);
}

void PageMappingTable::update_page(PBN pbn, LPN lpn, POff offset) {
  auto iter = table.find(pbn);
  if (iter == table.end()) {
    panic("Log block is not in use");
  }

  if ((POff)iter->second.size() <= offset) {
    panic("Invalid page offset");
  }

  iter->second[offset] = lpn;
}

void PageMappingTable::invalid_page(PBN pbn, LPN lpn) {
  auto iter = table.find(pbn);
  if (iter == table.end()) {
    panic("Log block is not in use");
  }

  for (auto &entry : iter->second) {
    if (entry == lpn) {
      entry = -1;
    }
  }
}

POff PageMappingTable::find_page(PBN pbn, LPN lpn) {
  auto iter = table.find(pbn);
  if (iter == table.end()) {
    panic("Log block is not in use");
  }

  for (POff i = 0; i < (POff)iter->second.size(); i++) {
    if (iter->second[i] == lpn) {
      return i;
    }
  }

  return -1;
}

const std::vector<LPN> &PageMappingTable::get_pages(PBN pbn) {
  auto iter = table.find(pbn);
  if (iter == table.end()) {
    panic("Log block is not in use");
  }

  return iter->second;
}

void UsedPhyBlocks::insert(PBN pbn, Block block) {
  if (blocks.find(pbn) != blocks.end()) {
    panic("Block is already in use");
  }
  blocks.insert({pbn, block});
}

void UsedPhyBlocks::erase(PBN pbn) {
  blocks.erase(pbn);
}

Block *UsedPhyBlocks::find(PBN pbn) {
  auto iter = blocks.find(pbn);
  if (iter != blocks.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

void FreeBlockList::insert(Block block) {
  freeBlocks.push_back(block);
  nFreeBlocks++;
}

Block FreeBlockList::pop() {
  if (nFreeBlocks == 0) {
    panic("No free block left");
  }

  Block block = freeBlocks.front();
  freeBlocks.pop_front();
  nFreeBlocks--;

  return block;
}

uint32_t FreeBlockList::size() {
  return nFreeBlocks;
}

/* ======== Lab4: FUNCTIONS NEED NOT CHANGE ======== */

float FASTMapping::freeBlockRatio() {
  return (float)freeBlocks.size() / param.totalPhysicalBlocks;
}

FASTMapping::FASTMapping(ConfigReader &c, Parameter &p, PAL::PAL *l,
                         DRAM::AbstractDRAM *d)
    : AbstractFTL(p, l, d), pPAL(l), conf(c) {
  for (uint32_t i = 0; i < param.totalPhysicalBlocks; i++) {
    freeBlocks.insert(Block(i, param.pagesInBlock, param.ioUnitInPage));
  }

  RWBlockSize = 6;
  SWBlockPBN = -1;
  SWBlockLBN = -1;
  bSWBlockValid = false;
  pagePerBlock = (int64_t)param.pagesInBlock;
  debugprint(LOG_FTL_PAGE_MAPPING, "pagePerBlock %" PRIu64, pagePerBlock);

  status.totalLogicalPages = param.totalLogicalBlocks * param.pagesInBlock;

  memset(&stat, 0, sizeof(stat));
}

FASTMapping::~FASTMapping() {}

void FASTMapping::read(Request &req, uint64_t &tick) {
  uint64_t begin = tick;

  if (req.ioFlag.count() > 0) {
    readInternal(req, tick);

    debugprint(LOG_FTL_PAGE_MAPPING,
               "READ  | LPN %" PRIu64 " | %" PRIu64 " - %" PRIu64 " (%" PRIu64
               ")",
               req.lpn, begin, tick, tick - begin);
  }
  else {
    warn("FTL got empty request");
  }

  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::READ);
}

void FASTMapping::write(Request &req, uint64_t &tick) {
  uint64_t begin = tick;

  if (req.ioFlag.count() > 0) {
    writeInternal(req, tick);

    debugprint(LOG_FTL_PAGE_MAPPING,
               "WRITE | LPN %" PRIu64 " | %" PRIu64 " - %" PRIu64 " (%" PRIu64
               ")",
               req.lpn, begin, tick, tick - begin);
  }
  else {
    warn("FTL got empty request");
  }

  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::WRITE);
}

void FASTMapping::trim(Request &req, uint64_t &tick) {
  uint64_t begin = tick;

  trimInternal(req, tick);

  debugprint(LOG_FTL_PAGE_MAPPING,
             "TRIM  | LPN %" PRIu64 " | %" PRIu64 " - %" PRIu64 " (%" PRIu64
             ")",
             req.lpn, begin, tick, tick - begin);

  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::TRIM);
}

void FASTMapping::format(LPNRange &, uint64_t &) {}

Status *FASTMapping::getStatus(uint64_t, uint64_t) {
  return &status;
}

bool FASTMapping::initialize() {
  uint64_t nPagesToWarmup;
  uint64_t nPagesToInvalidate;
  uint64_t nTotalLogicalPages;
  uint64_t maxPagesBeforeGC;
  uint64_t tick;
  uint64_t valid;
  uint64_t invalid;
  FILLING_MODE mode;

  Request req(param.ioUnitInPage);

  debugprint(LOG_FTL_PAGE_MAPPING, "Initialization started");

  nTotalLogicalPages = param.totalLogicalBlocks * param.pagesInBlock;
  nPagesToWarmup =
      nTotalLogicalPages * conf.readFloat(CONFIG_FTL, FTL_FILL_RATIO);
  nPagesToInvalidate =
      nTotalLogicalPages * conf.readFloat(CONFIG_FTL, FTL_INVALID_PAGE_RATIO);
  mode = (FILLING_MODE)conf.readUint(CONFIG_FTL, FTL_FILLING_MODE);
  maxPagesBeforeGC =
      param.pagesInBlock *
      (param.totalPhysicalBlocks *
           (1 - conf.readFloat(CONFIG_FTL, FTL_GC_THRESHOLD_RATIO)) -
       param.pageCountToMaxPerf);  // # free blocks to maintain

  if (nPagesToWarmup + nPagesToInvalidate > maxPagesBeforeGC) {
    warn("ftl: Too high filling ratio. Adjusting invalidPageRatio.");
    nPagesToInvalidate = maxPagesBeforeGC - nPagesToWarmup;
  }

  debugprint(LOG_FTL_PAGE_MAPPING, "Total logical pages: %" PRIu64,
             nTotalLogicalPages);
  debugprint(LOG_FTL_PAGE_MAPPING,
             "Total logical pages to fill: %" PRIu64 " (%.2f %%)",
             nPagesToWarmup, nPagesToWarmup * 100.f / nTotalLogicalPages);
  debugprint(LOG_FTL_PAGE_MAPPING,
             "Total invalidated pages to create: %" PRIu64 " (%.2f %%)",
             nPagesToInvalidate,
             nPagesToInvalidate * 100.f / nTotalLogicalPages);

  req.ioFlag.set();

  // Step 1. Filling
  if (mode == FILLING_MODE_0 || mode == FILLING_MODE_1) {
    // Sequential
    for (uint64_t i = 0; i < nPagesToWarmup; i++) {
      tick = 0;
      req.lpn = i;
      writeInternal(req, tick, false);
    }
  }
  else {
    // Random
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0, nTotalLogicalPages - 1);

    for (uint64_t i = 0; i < nPagesToWarmup; i++) {
      tick = 0;
      req.lpn = dist(gen);
      writeInternal(req, tick, false);
    }
  }

  // Step 2. Invalidating
  if (mode == FILLING_MODE_0) {
    // Sequential
    for (uint64_t i = 0; i < nPagesToInvalidate; i++) {
      tick = 0;
      req.lpn = i;
      writeInternal(req, tick, false);
    }
  }
  else if (mode == FILLING_MODE_1) {
    // Random
    // We can successfully restrict range of LPN to create exact number of
    // invalid pages because we wrote in sequential mannor in step 1.
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0, nPagesToWarmup - 1);

    for (uint64_t i = 0; i < nPagesToInvalidate; i++) {
      tick = 0;
      req.lpn = dist(gen);
      writeInternal(req, tick, false);
    }
  }
  else {
    // Random
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0, nTotalLogicalPages - 1);

    for (uint64_t i = 0; i < nPagesToInvalidate; i++) {
      tick = 0;
      req.lpn = dist(gen);
      writeInternal(req, tick, false);
    }
  }

  // Report
  calculateTotalPages(valid, invalid);
  debugprint(LOG_FTL_PAGE_MAPPING, "Filling finished. Page status:");
  debugprint(LOG_FTL_PAGE_MAPPING,
             "  Total valid physical pages: %" PRIu64
             " (%.2f %%, target: %" PRIu64 ", error: %" PRId64 ")",
             valid, valid * 100.f / nTotalLogicalPages, nPagesToWarmup,
             (int64_t)(valid - nPagesToWarmup));
  debugprint(LOG_FTL_PAGE_MAPPING,
             "  Total invalid physical pages: %" PRIu64
             " (%.2f %%, target: %" PRIu64 ", error: %" PRId64 ")",
             invalid, invalid * 100.f / nTotalLogicalPages, nPagesToInvalidate,
             (int64_t)(invalid - nPagesToInvalidate));
  debugprint(LOG_FTL_PAGE_MAPPING, "Initialization finished");

  return true;
}

void FASTMapping::trimInternal(Request &, uint64_t &tick) {
  tick += applyLatency(CPU::FTL__PAGE_MAPPING, CPU::TRIM_INTERNAL);
}

float FASTMapping::calculateWearLeveling() {
  // we donot consider the wear leveling in this lab
  return -1;
}

void FASTMapping::calculateTotalPages(uint64_t &valid, uint64_t &invalid) {
  valid = 0;
  invalid = 0;

  for (auto &iter : usedBlocks.blocks) {
    valid += iter.second.getValidPageCount();
    invalid += iter.second.getDirtyPageCount();
  }
}

void FASTMapping::getStatList(std::vector<Stats> &list, std::string prefix) {
  Stats temp;

  temp.name = prefix + "page_mapping.gc.count";
  temp.desc = "Total GC count";
  list.push_back(temp);

  temp.name = prefix + "page_mapping.gc.reclaimed_blocks";
  temp.desc = "Total reclaimed blocks in GC";
  list.push_back(temp);

  temp.name = prefix + "page_mapping.gc.superpage_copies";
  temp.desc = "Total copied valid superpages during GC";
  list.push_back(temp);

  temp.name = prefix + "page_mapping.gc.page_copies";
  temp.desc = "Total copied valid pages during GC";
  list.push_back(temp);

  // For the exact definition, see following paper:
  // Li, Yongkun, Patrick PC Lee, and John Lui.
  // "Stochastic modeling of large-scale solid-state storage systems:
  // analysis, design tradeoffs and optimization." ACM SIGMETRICS (2013)
  temp.name = prefix + "page_mapping.wear_leveling";
  temp.desc = "Wear-leveling factor";
  list.push_back(temp);
}

void FASTMapping::getStatValues(std::vector<double> &values) {
  values.push_back(stat.gcCount);
  values.push_back(stat.reclaimedBlocks);
  values.push_back(stat.validSuperPageCopies);
  values.push_back(stat.validPageCopies);
  values.push_back(calculateWearLeveling());
}

void FASTMapping::resetStatValues() {
  memset(&stat, 0, sizeof(stat));
}

}  // namespace FTL

}  // namespace SimpleSSD
