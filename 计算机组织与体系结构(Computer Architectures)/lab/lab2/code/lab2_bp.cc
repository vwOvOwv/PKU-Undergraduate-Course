#include "cpu/pred/lab2_bp.hh"
#include <cstdint>

namespace gem5 {
namespace branch_prediction {
Lab2PerceptronBP::Lab2PerceptronBP(const Lab2PerceptronBPParams &params)
    :   BPredUnit(params),
        globalHistoryReg(params.numThreads, 0),
        globalPredictorSize(params.perceptronTableSize),
        historyLength(params.historyLength)
{
    if (!isPowerOf2(globalPredictorSize))
        fatal("Invalid global history predictor size.\n");

    globalHistoryMask = globalPredictorSize - 1;  // Used to calculate indices of weights
    threshold = static_cast<int>(1.93 * historyLength + 14); // This formula comes from the paper
    weights = std::vector< std::vector<int> >(globalPredictorSize, 
                std::vector<int>(historyLength + 1));   // Add an extra weight as bias
}

void Lab2PerceptronBP::uncondBranch(ThreadID tid, Addr pc, void * &bp_history){ // Cope with uncond. branches
    BPHistory *history = new BPHistory;
    history->globalHistoryReg = globalHistoryReg[tid];
    history->finalPred = true;
    bp_history = static_cast<void*>(history);
}

bool Lab2PerceptronBP::lookup(ThreadID tid, Addr branch_addr,
                              void *&bp_history) {
    unsigned globalHistoryIdx = (((branch_addr >> instShiftAmt) // The same hash function as other methods
                                ^ globalHistoryReg[tid])
                                & globalHistoryMask);
    
    assert(globalHistoryIdx < globalPredictorSize);

    std::vector<int> weight = weights[globalHistoryIdx];

    // Conduct prediction
    int y = weight[0];
    for(int i = 1; i < historyLength + 1; i++){
        int tmp = (globalHistoryReg[tid] >> (i - 1)) & 1;
        if(tmp == 0)
            y -= weight[i];
        else
            y += weight[i]; 
    }
    bool finalPrediction = (y >= 0);

    BPHistory *history = new BPHistory;
    history->globalHistoryReg = globalHistoryReg[tid];
    history->y = y; // Store raw result for training
    history->finalPred = finalPrediction;
    bp_history = static_cast<void *>(history);

    return finalPrediction;
}

void Lab2PerceptronBP::updateHistories(ThreadID tid, Addr pc, bool uncond,
                                       bool taken, Addr target,
                                       void *&bp_history) {
    assert(uncond || bp_history);
    if (uncond) {   // Unconditional branches call it, since there is no need to look up
        uncondBranch(tid, pc, bp_history);
    }
    updateGlobalHistReg(tid, taken);
}

void Lab2PerceptronBP::update(ThreadID tid, Addr pc, bool taken,
                              void *&bp_history, bool squashed,
                              const StaticInstPtr &inst, Addr target) {
    assert(bp_history);

    BPHistory *history = static_cast<BPHistory *>(bp_history);

    if(squashed){   // The prediciton is squashed, and we still need to record actual results
        globalHistoryReg[tid] = (history->globalHistoryReg << 1) | taken;
        return;
    }

    unsigned globalHistoryIdx = (((pc >> instShiftAmt)
                                ^ history->globalHistoryReg)
                                & globalHistoryMask);

    assert(globalHistoryIdx < globalPredictorSize);

    // Training perceptrons
    int t;
    if(history->finalPred == taken){ // sign(y) == t
        if(taken)
            t = 1;
        else
            t = -1;

        if(abs(history->y) <= threshold){
            weights[globalHistoryIdx][0] += t;
            for(int i = 1; i < historyLength + 1; i++){
                unsigned long tmp = (globalHistoryReg[tid] >> (i - 1)) & 1;
                if(tmp == 1)
                    weights[globalHistoryIdx][i] += t;
                else
                    weights[globalHistoryIdx][i] -= t; 
            }
        }
    }
    else{   // sign(y) != t
        if(taken)
            t = 1;
        else
            t = -1;
        
        weights[globalHistoryIdx][0] += t;
        for(int i = 1; i < historyLength + 1; i++){ // No need to check y here.
            unsigned long tmp = (globalHistoryReg[tid] >> (i - 1)) & 1;
            if(tmp == 1)
                weights[globalHistoryIdx][i] += t;
            else
                weights[globalHistoryIdx][i] -= t; 
        }
    }

    delete history;
    bp_history = nullptr;
}

void Lab2PerceptronBP::squash(ThreadID tid, void *&bp_history) {
    BPHistory *history = static_cast<BPHistory*>(bp_history);
    globalHistoryReg[tid] = history->globalHistoryReg;
    delete history;
    bp_history = nullptr;
}

void Lab2PerceptronBP::updateGlobalHistReg(ThreadID tid, bool taken)
{
    globalHistoryReg[tid] = taken ? (globalHistoryReg[tid] << 1) | 1 :
                               (globalHistoryReg[tid] << 1);
    globalHistoryReg[tid] &= (1 << historyLength) - 1;
}
} // namespace branch_prediction
} // namespace gem5