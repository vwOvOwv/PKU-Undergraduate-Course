#pragma once

#include "ir.hpp"
#include <iostream>
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
    RiscvGenerator(const ProgramIR* ir, std::ostream& os) 
        : program(ir), os(os) {}

    void generate();

private:
    const ProgramIR* program;
    std::ostream& os;

    void visit(const FunctionIR* func);
    void visit(const BasicBlockIR* block);
    void visit(const ReturnIR* ret_inst);
    void visit(const IntegerIR* int_val);
};

// =========================================================
// 方法实现
// =========================================================

inline void RiscvGenerator::generate() {
    os << "  .text" << std::endl;
    for (const auto& func : program->funcs) {
        visit(func.get());
    }
}

inline void RiscvGenerator::visit(const FunctionIR* func) {
    // KoopaIR 函数名字里带@，而汇编不需要，所以要去掉第一个字符
    std::string name = func->func_name.substr(0); 
    
    os << "  .globl " << name << std::endl;
    os << name << ":" << std::endl;
    
    for (const auto& block : func->basic_blocks) {
        visit(block.get());
    }
}

inline void RiscvGenerator::visit(const BasicBlockIR* block) {
    for (const auto& inst : block->insts) {
        if (auto ret_inst = dynamic_cast<const ReturnIR*>(inst.get())) {
            visit(ret_inst);
        }
    }
}

inline void RiscvGenerator::visit(const ReturnIR* ret_inst) {
    // ret 0 -> 要把 0 放入 a0 寄存器
    auto val = ret_inst->ret_value.get();
    // Lv1 只有整数
    if (auto int_val = dynamic_cast<const IntegerIR*>(val)) {
        os << "  li a0, " << int_val->value << std::endl;
    }
    os << "  ret" << std::endl;
}