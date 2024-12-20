#ifndef __MEM_CACHE_PREFETCH_LAB3_PF_HH__
#define __MEM_CACHE_PREFETCH_LAB3_PF_HH__

#include "mem/cache/prefetch/queued.hh"
#include "mem/packet.hh"
#include "params/Lab3Hyperion.hh"
#include <cstdint>
#include <deque>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

namespace gem5 {
namespace prefetch {
class Lab3Hyperion : public Queued {
public:
  	Lab3Hyperion(const Lab3HyperionParams &params);
  	~Lab3Hyperion() = default;
  	void calculatePrefetch(const PrefetchInfo &pfi,
                         std::vector<AddrPriority> &addresses,
                         const CacheAccessor &cache) override;
  	void notifyFill(const CacheAccessProbeArg &acc) override;

private:
	void update_histories(Addr PC, Addr block_addr, uint64_t time);
	void search_deltas(Addr PC, Addr block_addr);
	void learn_update_delta(Addr PC, Addr block_addr, uint64_t latency);

	double CONF_THRESHOLD_L1D;
	int entry_length;
	int degree;
	int max_prefetch_cnt;
	int max_counter;

	std::unordered_map<Addr, uint64_t> prefetch_queue;	// address -> time
	std::unordered_map<Addr, uint64_t> demand_queue;	// address -> time
	std::vector<std::pair<double, int64_t>> sorted_deltas;	// (confidence, delta)

	struct HisEntry{
		std::deque<std::pair<Addr, uint64_t>> addrs;	// (address, time)
	};

	struct DeltaEntry{
		int counter;
		std::vector<std::pair<int, int64_t>> deltas;	// (counter, delta)
	};

	std::unordered_map<Addr, HisEntry> PC_history_table;
	std::unordered_map<Addr, DeltaEntry> PC_delta_table;
	std::unordered_map<Addr, HisEntry> page_history_table;
	std::unordered_map<Addr, DeltaEntry> page_delta_table;

};
} // namespace prefetch
} // namespace gem5

#endif // __MEM_CACHE_PREFETCH_LAB3_PF_HH__
