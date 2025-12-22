#pragma once

#include "ir.hpp"
#include <string>

// =========================================================
// 前向声明
// =========================================================

class RiscvGenerator;

// =========================================================
// 类定义
// =========================================================

class RiscvGenerator {
public:
    RiscvGenerator(const ProgramIR* ir) : program(ir) {}

    void generate();
    std::string get_reg_name(int reg_id);

private:
    const ProgramIR* program;

    void visit(const FunctionIR* func_ir);
    void visit(const BasicBlockIR* block_ir);
    void visit(const ReturnIR* ret_ir);
    void visit(const EqIR* eq_ir);
    void visit(const SubIR* sub_ir);

    std::vector<std::string> reg_names = {
        "x0", 
        "t0", "t1", "t2", "t3", "t4", "t5", "t6",
        "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    };
};

// =========================================================
// 方法实现
// =========================================================

inline void RiscvGenerator::generate() {
    std::cout << "  .text" << std::endl;
    for (const auto& func : program->funcs) {
        visit(func.get());
    }
}

inline std::string RiscvGenerator::get_reg_name(int reg_id) {
    if (reg_id >= 0 && reg_id < static_cast<int>(reg_names.size())) {
        return reg_names[reg_id];
    }
    return "unknown_reg";
}

inline void RiscvGenerator::visit(const FunctionIR* func_ir) {
    // KoopaIR 函数名字里带@，而汇编不需要，所以要去掉第一个字符
    std::string name = func_ir->func_name.substr(0); 
    
    std::cout << "  .globl " << name << std::endl;
    std::cout << name << ":" << std::endl;
    
    for (const auto& block : func_ir->basic_blocks) {
        visit(block.get());
    }
}

inline void RiscvGenerator::visit(const BasicBlockIR* block_ir) {
    for (const auto& inst : block_ir->insts) {
        // 只有 ReturnIR
        if (auto ret_inst = dynamic_cast<const ReturnIR*>(inst.get())) {
            visit(ret_inst);
        }
    }
}

inline void RiscvGenerator::visit(const ReturnIR* ret_ir) {
    auto val_ir = ret_ir->ret_value.get();
    if (val_ir) {
        std::cout << "  li a0, ";
        val_ir->dump();
        std::cout << std::endl;
    }
    std::cout << "  ret" << std::endl;
}

// inline void RiscvGenerator::visit(const EqIR* eq_ir) {
//     std::cout << "  li    " << get_reg_name(eq_ir->target->value) << ", 0" << std::endl;
//     std::cout << "  seqz   " << 
// }
