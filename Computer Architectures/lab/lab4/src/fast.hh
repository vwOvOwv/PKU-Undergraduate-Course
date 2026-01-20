#ifndef __FTL_FAST_MAPPING__
#define __FTL_FAST_MAPPING__

#include <sys/types.h>

#include <cinttypes>
#include <unordered_map>
#include <vector>

#include "ftl/abstract_ftl.hh"
#include "ftl/common/block.hh"
#include "ftl/ftl.hh"
#include "pal/pal.hh"

namespace SimpleSSD {

namespace FTL {

/* ======== Lab4: HELPER CLASSES ======== */

typedef int64_t LPN;   // Logical Page Number
typedef int64_t PPN;   // Physical Page Number
typedef int64_t POff;  // Page Offset
typedef int64_t LBN;   // Logical Block Number
typedef int64_t PBN;   // Physical Block Number

// Block mapping table, which maps LBN to PBN, used for data blocks.
class BlockMappingTable {
  std::unordered_map<LBN, PBN> table;

 public:
  // Update the mapping of LBN. If the mapping exist, update it. Otherwise,
  // insert it.
  void update(LBN lbn, PBN pbn);
  // Invalidate the mapping of LBN, erase it from the table.
  void invalidate(LBN lbn);
  // Find the mapping of LBN. If the mapping exist, return it. Otherwise, return
  // -1.
  PBN find(LBN lbn);
};

// Page mapping table, which maps LPN to PPN, used for log blocks.
class PageMappingTable {
  // Each log block has a vector, which contains LPN to PPN mapping. Each vector
  // has a fixed size, which is the number of pages in a block. The vector is
  // indexed by the page offset, and stores the LPN maps to this offset. The
  // PPN is collected by PBN + offset.
  std::unordered_map<PBN, std::vector<LPN>> table;              

 public:
  // Allocate a vector for a new log block and insert it into the table.
  void new_block(PBN pbn, uint64_t pageCount);
  // Check if the log block is in the table.
  bool find_block(PBN pbn);
  // Invalidate the log block, erase it from the table.
  void invalid_block(PBN pbn);
  // Update the mapping of LPN in the log block. If the mapping exist, update
  // it. Otherwise, insert it.
  void update_page(PBN pbn, LPN lpn, POff offset);
  // Invalidate the mapping of LPN in the log block, erase it from the table.
  void invalid_page(PBN pbn, LPN lpn);
  // Find the mapping of LPN in the log block. If the mapping exist, return it.
  // Otherwise, return -1.
  POff find_page(PBN pbn, LPN lpn);
  // Get all page mapping in the log block.
  const std::vector<LPN> &get_pages(PBN pbn);
};

// Used physical blocks table, which stores the `Block` class instances for used
// physical blocks.
class UsedPhyBlocks {
 public:
  std::unordered_map<PBN, Block> blocks;
  // Insert a new block into the table.
  void insert(PBN pbn, Block block);
  // Erase a block from the table.
  void erase(PBN pbn);
  // Find a block in the table and return the pointer if exist. else nullptr.
  Block *find(PBN pbn);
};

// Free block list, which stores the `Block` class instances for free blocks.
class FreeBlockList {
  std::list<Block> freeBlocks;
  uint32_t nFreeBlocks;

 public:
  // Insert a new block into the list.
  void insert(Block block);
  // Pop a block from the list.
  Block pop();
  // Get the number of free blocks.
  uint32_t size();
};

class FASTMapping : public AbstractFTL {
 private:
  PAL::PAL *pPAL;
  ConfigReader &conf;

  BlockMappingTable BMT;
  PageMappingTable PMT;
  UsedPhyBlocks usedBlocks;
  FreeBlockList freeBlocks;

  int64_t pagePerBlock;

  // A list of RW blocks, which stores the PBN of RW blocks.
  std::list<PBN> RWBlockList;
  // Max number of RW blocks. Fixed to 6.
  uint64_t RWBlockSize;

  // SW block PBN.
  PBN SWBlockPBN;
  // SW block LBN.
  LBN SWBlockLBN;
  // SW block valid flag.
  bool bSWBlockValid;

  struct {
    uint64_t gcCount;
    uint64_t reclaimedBlocks;
    uint64_t validSuperPageCopies;
    uint64_t validPageCopies;
  } stat;

  /* ======== Lab4 TODO: FUNCTIONS REQUIRE IMPLEMENTATION ======== */

  // Invalidate the old page of {LBN, offset} in any RW, SW or data blocks.
  void invalidateOldPage(LBN lbn, POff offset);
  // Find a page of LPN in any RW block, return the PBN and page offset if
  // found, else {-1, -1}.
  std::pair<PBN, POff> findPageInRWBlocks(LPN lpn);
  // Find a page of {LBN, offset} in SW block, return true if found, else false.
  bool findPageInSWBlock(LBN lbn, POff offset);
  // Find a page of {LBN, offset} in any data blocks, return the PBN of the data
  // block if found, else -1.
  PBN findPageInDataBlocks(LBN lbn, POff offset);
  // Check if a page of {LBN, offset} in any data block is free (not written
  // since last erase), return true if free, else false.
  bool pageIsFreeInDataBlock(LBN lbn, POff offset);
  // Give a write target by {LBN, offset}, check if it can be written to SW
  // block, return true if can be written, else false.
  // If it is needed to merge/switch the SW block first to write this page,
  // perform the merge/switch operation (based on tick and sendToPAL) and return
  // true.
  bool canWriteToSWBlockAfterMerge(LBN lbn, POff offset, uint64_t &tick,
                                   bool sendToPAL);
  // Find a free page in RW blocks, return the PBN and page offset if found,
  // else {-1, -1}.
  // If there is no free page in RW blocks, allocate a new RW block and return
  // the PBN and page offset.
  // If the RW block number reaches the maximum, perform the merge operation
  // (based on tick and sendToPAL) and then allocate.
  std::pair<PBN, POff> findFreePageInRWBlocks(uint64_t &tick, bool sendToPAL);

  /* ======== Lab4: FUNCTIONS ALREADY IMPLEMENTED ======== */

  void performPALRequestForMerge(std::vector<PAL::Request> &reads,
                                 std::vector<PAL::Request> &writes,
                                 std::vector<PAL::Request> &erases,
                                 uint64_t &tick, bool sendToPAL);

  // Perform read operation, read the data from log or data blocks and update
  // tick.
  void readInternal(Request &, uint64_t &);
  // Perform write operation, write the data to log or data blocks and update
  // tick.
  void writeInternal(Request &, uint64_t &, bool = true);
  // Perform erase operation, erase a blocks and update tick.
  // Lab4 NOTE: This function will automatically perform the erase on Block
  // class and update on usedBLocks and freeBlocks, so you do not need to
  // perform these operations again when you call this function.
  void eraseInternal(PAL::Request &, uint64_t &);

  /* ======== Lab4: FUNCTIONS NEED NOT CHANGE ======== */

  float calculateWearLeveling();
  void calculateTotalPages(uint64_t &, uint64_t &);
  float freeBlockRatio();
  void trimInternal(Request &, uint64_t &);

 public:
  FASTMapping(ConfigReader &, Parameter &, PAL::PAL *, DRAM::AbstractDRAM *);
  ~FASTMapping();

  bool initialize() override;

  void read(Request &, uint64_t &) override;
  void write(Request &, uint64_t &) override;
  void trim(Request &, uint64_t &) override;

  void format(LPNRange &, uint64_t &) override;

  Status *getStatus(uint64_t, uint64_t) override;

  void getStatList(std::vector<Stats> &, std::string) override;
  void getStatValues(std::vector<double> &) override;
  void resetStatValues() override;
};

}  // namespace FTL

}  // namespace SimpleSSD

#endif
