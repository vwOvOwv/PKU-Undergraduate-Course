#include "mem/cache/prefetch/lab3_pf.hh"
#include "debug/HWPrefetch.hh"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <utility>

namespace gem5 {
namespace prefetch {
Lab3Hyperion::Lab3Hyperion(const Lab3HyperionParams &params) 
    :   Queued(params), 
        CONF_THRESHOLD_L1D(params.CONF_THRESHOLD_L1D),
        entry_length(params.entry_length),
        degree(params.degree),
        max_prefetch_cnt(params.max_prefetch_cnt),
        max_counter(params.max_counter){}

void Lab3Hyperion::update_histories(Addr PC, Addr block_addr, uint64_t time){
    DPRINTF(HWPrefetch, "update_histories\n");

    PC_history_table[PC].addrs.push_back(std::make_pair(block_addr, time));
    DPRINTF(HWPrefetch, "   PC: %x, block_addr: %x\n", PC, block_addr);
    DPRINTF(HWPrefetch, "   Current entry length: %d\n", PC_history_table[PC].addrs.size());
    while(PC_history_table[PC].addrs.size() > entry_length)
        PC_history_table[PC].addrs.pop_front();

    Addr page_index = pageIndex(block_addr);
    page_history_table[page_index].addrs.push_back(std::make_pair(block_addr, time));
    DPRINTF(HWPrefetch, "   Page Number: %x, block_addr: %x\n", page_index, block_addr);
    DPRINTF(HWPrefetch, "   Current entry length: %d\n", page_history_table[page_index].addrs.size());
    while(page_history_table[page_index].addrs.size() > entry_length)
        page_history_table[page_index].addrs.pop_front();
}

void Lab3Hyperion::search_deltas(Addr PC, Addr block_addr) {
    DPRINTF(HWPrefetch, "search_deltas\n");
    sorted_deltas.clear();
    DPRINTF(HWPrefetch, "   PC: %x, block_addr: %x\n", PC, block_addr);

    if(PC_delta_table.count(PC) > 0){
        DPRINTF(HWPrefetch, "   PC_delta_table[PC] exists\n");
        DPRINTF(HWPrefetch, "   Global counter: %d\n", PC_delta_table[PC].counter);
        DPRINTF(HWPrefetch, "   Current entry length: %d\n", PC_delta_table[PC].deltas.size());

        // if(PC_delta_table[PC].counter > 0.2 * max_counter){
            for(auto i = PC_delta_table[PC].deltas.begin(); i != PC_delta_table[PC].deltas.end(); i++){
                double confidence = static_cast<double>(i->first) / PC_delta_table[PC].counter;
                DPRINTF(HWPrefetch, "   Local counter: %d\n", i->first);
                if(confidence > CONF_THRESHOLD_L1D){
                    sorted_deltas.push_back(std::make_pair(confidence, i->second));
                }
            }
        // }
    }

    Addr page_index = pageIndex(block_addr);
    DPRINTF(HWPrefetch, "   Page Number: %x, block_addr: %x\n", page_index, block_addr);
    if(page_delta_table.count(page_index) > 0){
        DPRINTF(HWPrefetch, "   page_delta_table[page_index] exists\n");
        DPRINTF(HWPrefetch, "   Global counter: %d\n", page_delta_table[page_index].counter);
        DPRINTF(HWPrefetch, "   Current entry length: %d\n", page_delta_table[page_index].deltas.size());

        // if(page_delta_table[page_index].counter > 0.2 * max_counter){
            for(auto i = page_delta_table[page_index].deltas.begin(); i != page_delta_table[page_index].deltas.end(); i++){
                double confidence = static_cast<double>(i->first) / page_delta_table[page_index].counter;
                DPRINTF(HWPrefetch, "   Local counter: %d\n", i->first);
                if(confidence > CONF_THRESHOLD_L1D){
                    sorted_deltas.push_back(std::make_pair(confidence, i->second));
                }
            }
        // }
    }

    std::sort(sorted_deltas.begin(), sorted_deltas.end(), std::greater<std::pair<double, int64_t>>());
}

void Lab3Hyperion::learn_update_delta(Addr PC, Addr block_addr, uint64_t best_request_time){
    DPRINTF(HWPrefetch, "learn_update_delta\n");
    DPRINTF(HWPrefetch, "   PC: %x, block_addr: %x\n", PC, block_addr);
    DPRINTF(HWPrefetch, "   best prefetch time: %d\n", best_request_time);
    int64_t learned_pc_delta = INT64_MIN;
    if(PC_history_table.count(PC) > 0){
        DPRINTF(HWPrefetch, "   PC_history_table[PC] exists\n");
        for(auto i = PC_history_table[PC].addrs.begin(); i != PC_history_table[PC].addrs.end(); i++){
            if(i->second < best_request_time)
                learned_pc_delta = ((int64_t)block_addr - (int64_t)i->first) / (int64_t)blkSize;
            else
                break;
        }
    }

    DPRINTF(HWPrefetch, "   learned pc delta: %ld\n", learned_pc_delta);
    if(learned_pc_delta != INT64_MIN){
        if(PC_delta_table[PC].deltas.empty()){
            DPRINTF(HWPrefetch, "   Empty PC_delta_table[PC]\n");
            PC_delta_table[PC].counter = 1;
            PC_delta_table[PC].deltas.push_back(std::make_pair(1, learned_pc_delta));
        }
        else{
            DPRINTF(HWPrefetch, "   PC_delta_table[PC] exists\n");
            PC_delta_table[PC].counter++;
            if(PC_delta_table[PC].counter == max_counter){
                PC_delta_table[PC].counter /= 2;
                for(auto i = PC_delta_table[PC].deltas.begin(); i!= PC_delta_table[PC].deltas.end(); i++)
                    i->first /= 2;
            }
            bool found = false;
            for(auto i = PC_delta_table[PC].deltas.begin(); i!= PC_delta_table[PC].deltas.end(); i++){
                if(i->second == learned_pc_delta){
                    DPRINTF(HWPrefetch, "   Find learned_pc_delta in PC Delta Table\n");
                    found = true;
                    i->first++;
                    break;
                }
            }
            if(!found){
                DPRINTF(HWPrefetch, "   Not find learned_pc_delta in PC Delta Table\n");
                if(PC_delta_table[PC].deltas.size() < entry_length){
                    PC_delta_table[PC].deltas.push_back(std::make_pair(1, learned_pc_delta));
                }
                else{
                    std::sort(PC_delta_table[PC].deltas.begin(), PC_delta_table[PC].deltas.end(), std::greater<std::pair<double, int64_t>>());
                    while(PC_delta_table[PC].deltas.size() >= entry_length)
                        PC_delta_table[PC].deltas.pop_back();
                    PC_delta_table[PC].deltas.push_back(std::make_pair(1, learned_pc_delta));
                }
            }
            // int sum = 0;
            // for(auto i = PC_delta_table[PC].deltas.begin(); i!= PC_delta_table[PC].deltas.end(); i++)
            //     sum += i->first;
            // if(sum > max_counter){
            //     sum = 0;
            //     for(auto i = PC_delta_table[PC].deltas.begin(); i!= PC_delta_table[PC].deltas.end(); i++){
            //         i->first = std::max(1, i->first / 2);
            //         sum += i->first;
            //     }
            // }
            // PC_delta_table[PC].counter = sum;
        }
    }

    Addr page_index = pageIndex(block_addr);
    int64_t learned_page_delta = INT64_MIN;
    DPRINTF(HWPrefetch, "   Page index: %x, block_addr: %x\n", page_index, block_addr);
    DPRINTF(HWPrefetch, "   best prefetch time: %d\n", best_request_time);

    if(page_history_table.count(page_index) > 0){
        DPRINTF(HWPrefetch, "   page_history_table[page_index] exists\n");
        for(auto i = page_history_table[page_index].addrs.begin(); i != page_history_table[page_index].addrs.end(); i++){
            if(i->second < best_request_time)
                learned_page_delta = ((int64_t)block_addr - (int64_t)i->first) / (int64_t)blkSize;
            else
                break;
        }
    }
    DPRINTF(HWPrefetch, "   learned page delta: %ld\n", learned_page_delta);
    if(learned_page_delta != INT64_MIN){
        if(page_delta_table[page_index].deltas.empty()){
            DPRINTF(HWPrefetch, "   Empty page_delta_table[page_index]\n");
            page_delta_table[page_index].counter = 1;
            page_delta_table[page_index].deltas.push_back(std::make_pair(1, learned_page_delta));
        }
        else{
            DPRINTF(HWPrefetch, "   page_delta_table[page_index] exists\n");
            page_delta_table[page_index].counter++;
            if(page_delta_table[page_index].counter == max_counter){
                page_delta_table[page_index].counter /= 2;
                for(auto i = page_delta_table[page_index].deltas.begin(); i!= page_delta_table[page_index].deltas.end(); i++)
                    i->first /= 2;
            }
            bool found = false;
            for(auto i = page_delta_table[page_index].deltas.begin(); i!= page_delta_table[page_index].deltas.end(); i++){
                if(i->second == learned_page_delta){
                    DPRINTF(HWPrefetch, "   Find learned_page_delta in page Delta Table\n");
                    found = true;
                    i->first++;
                    break;
                }
            }
            if(!found){
                DPRINTF(HWPrefetch, "   Not find learned_page_delta in page Delta Table\n");
                if(page_delta_table[page_index].deltas.size() < entry_length){
                    page_delta_table[page_index].deltas.push_back(std::make_pair(1, learned_page_delta));
                }
                else{
                    std::sort(page_delta_table[page_index].deltas.begin(), page_delta_table[page_index].deltas.end(), std::greater<std::pair<double, int64_t>>());
                    while(page_delta_table[page_index].deltas.size() >= entry_length)
                        page_delta_table[page_index].deltas.pop_back();
                    page_delta_table[page_index].deltas.push_back(std::make_pair(1, learned_page_delta));
                }
            }
            // int sum = 0;
            // for(auto i = page_delta_table[page_index].deltas.begin(); i!= page_delta_table[page_index].deltas.end(); i++)
            //     sum += i->first;
            // if(sum > max_counter){
            //     sum = 0;
            //     for(auto i = page_delta_table[page_index].deltas.begin(); i!= page_delta_table[page_index].deltas.end(); i++){
            //         i->first = std::max(1, i->first / 2);
            //         sum += i->first;
            //     }
            // }
            // page_delta_table[page_index].counter = sum;
        }
    }
}

void Lab3Hyperion::calculatePrefetch(const PrefetchInfo &pfi,   // base.hh
                                     std::vector<AddrPriority> &addresses,
                                     const CacheAccessor &cache) {  // src/mem/cache/cache_probe_arg.hh
    DPRINTF(HWPrefetch, "calculatePrefetch\n");
    if (!pfi.hasPC()) {
        DPRINTF(HWPrefetch, "Ignoring request with no PC.\n");
        return;
    }
    uint64_t access_time = curTick();
    Addr PC = pfi.getPC();
    Addr block_addr = blockAddress(pfi.getAddr());

    if(!pfi.isCacheMiss()){   // prefetch hit，此时prefetch_queue中一定有block_addr
        DPRINTF(HWPrefetch, "   Prefetch hit!\n");
        DPRINTF(HWPrefetch, "   last access time: %d\n", prefetch_queue[block_addr]);
        if(prefetch_queue[block_addr] >= access_time - prefetch_queue[block_addr]){
            uint64_t best_request_time = prefetch_queue[block_addr] - (access_time - prefetch_queue[block_addr]);
            learn_update_delta(PC, block_addr, best_request_time);
        }
    }
    else{   // demand miss
        demand_queue[block_addr] = access_time;
    }

    update_histories(PC, block_addr, access_time);
    search_deltas(PC, block_addr);    
    int upper = std::min(max_prefetch_cnt, static_cast<int>(sorted_deltas.size()));
    DPRINTF(HWPrefetch, "issue prefetch\n");
    DPRINTF(HWPrefetch, "   Prefetch counts: %d\n", upper);
    for(int i = 0; i < upper; i++){
        int64_t delta = sorted_deltas[i].second;
        for(int j = 0; j < degree; j++){
            Addr tmp_addr = block_addr + delta * (j + 1) * blkSize;
            addresses.push_back(AddrPriority(tmp_addr, 0));
            prefetch_queue[tmp_addr] = curTick();
        }
    }
}

void Lab3Hyperion::notifyFill(const CacheAccessProbeArg &acc) { // src/mem/cache/cache_probe_arg.hh
    DPRINTF(HWPrefetch, "notifyFill\n");
    if(acc.pkt->cmd.isHWPrefetch()){
        DPRINTF(HWPrefetch, "   Ignoring prefetch fill.\n");
        return;
    }
    if(!acc.pkt->req->hasPC()){
        DPRINTF(HWPrefetch, "   Ignoring fill with no PC.\n");
        return;
    }
    Addr PC = acc.pkt->req->getPC();
    Addr block_addr = blockAddress(acc.pkt->getAddr());
    Addr page_index = pageIndex(block_addr);
    uint64_t fill_time = curTick();
    uint64_t last_access_time = demand_queue[block_addr];
    uint64_t latency = fill_time - last_access_time;
    // uint64_t latency = UINT64_MAX;
    // uint64_t last_access_time;
    
    // DPRINTF(HWPrefetch, "   PC: %x, block_addr: %x\n", PC, block_addr);
    // for(auto i = PC_history_table[PC].addrs.begin(); i != PC_history_table[PC].addrs.end(); i++){
    //     if(block_addr == i->first){
    //         DPRINTF(HWPrefetch, "   Find block address in PC history table\n");
    //         latency = fill_time - i->second;
    //         last_access_time = i->second;
    //     }
    // }
    
    // DPRINTF(HWPrefetch, "   Page Number: %x, block_addr: %x\n", page_index, block_addr);
    // for(auto i = page_history_table[page_index].addrs.begin(); i != page_history_table[page_index].addrs.end(); i++){
    //     if(block_addr == i->first){
    //         DPRINTF(HWPrefetch, "   Find block address in page history table\n");
    //         if(latency > fill_time - i->second){
    //             latency = fill_time - i->second;
    //             last_access_time = i->second;
    //         }
    //     }
    // }
    
    // if(latency != UINT64_MAX){
        DPRINTF(HWPrefetch, "   Latency: %d, last access time: %d\n", latency, last_access_time);
        if(last_access_time > latency)
            learn_update_delta(PC, block_addr, last_access_time - latency);
    // }
}
} // namespace prefetch
} // namespace gem5
