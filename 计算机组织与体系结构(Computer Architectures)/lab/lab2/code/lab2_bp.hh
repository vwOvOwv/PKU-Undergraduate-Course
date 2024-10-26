#ifndef __CPU_PRED_LAB2_BP_HH__
#define __CPU_PRED_LAB2_BP_HH__

#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/Lab2PerceptronBP.hh"
#include <sys/types.h>
#include <vector>

namespace gem5 {
namespace branch_prediction {
class Lab2PerceptronBP : public BPredUnit {
public:
  	Lab2PerceptronBP(const Lab2PerceptronBPParams &params);
	
  	bool lookup(ThreadID tid, Addr branch_addr, void *&bp_history) override;
  	void updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
					   Addr target, void *&bp_history) override;
	void update(ThreadID tid, Addr pc, bool taken, void *&bp_history,
			  bool squashed, const StaticInstPtr &inst, Addr target) override;
 	void squash(ThreadID tid, void *&bp_history) override;

private:
  	void updateGlobalHistReg(ThreadID tid, bool taken);
	void uncondBranch(ThreadID tid, Addr pc, void * &bp_history);
	
	struct BPHistory{
		u_int64_t globalHistoryReg;
		int y;	// Store raw prediction result
		bool finalPred;
	};

	// Parameters below are adapted from bi_mode.hh, some of which are not used in lab2_bp.cc
  	std::vector<u_int64_t> globalHistoryReg;
	
	// Unsigned globalHistoryBits;
	unsigned historyRegisterMask;
	unsigned globalPredictorSize;
	unsigned globalCtrBits;
    unsigned globalHistoryMask;

	// Hyperparameters for perceptronBP
	unsigned threshold;
	unsigned historyLength;

	std::vector< std::vector<int> > weights; // i.e., PHT
};
} // namespace branch_prediction
} // namespace gem5

#endif // __CPU_PRED_LAB2_BP_HH__
