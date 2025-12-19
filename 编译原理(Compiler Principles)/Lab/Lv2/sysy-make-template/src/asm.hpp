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

    void Generate();

private:
    const ProgramIR* program;
    std::ostream& os;

    void Visit(const FunctionIR* func);
    void Visit(const BasicBlockIR* block);
    void Visit(const ReturnIR* ret_inst);
    void Visit(const IntegerIR* int_val);
};

// =========================================================
// 方法实现
// =========================================================

inline void RiscvGenerator::Generate() {
    os << "  .text" << std::endl;
    for (const auto& func : program->funcs) {
        Visit(func.get());
    }
}

inline void RiscvGenerator::Visit(const FunctionIR* func) {
    // KoopaIR 函数名字里带@，而汇编不需要，所以要去掉第一个字符
    std::string name = func->func_name.substr(0); 
    
    os << "  .globl " << name << std::endl;
    os << name << ":" << std::endl;
    
    for (const auto& block : func->basic_blocks) {
        Visit(block.get());
    }
}

inline void RiscvGenerator::Visit(const BasicBlockIR* block) {
    for (const auto& inst : block->insts) {
        // Lv1 只有 ReturnIR
        if (auto ret_inst = dynamic_cast<const ReturnIR*>(inst.get())) {
            Visit(ret_inst);
        }
    }
}

// 4. 生成 Return 指令
inline void RiscvGenerator::Visit(const ReturnIR* ret_inst) {
    // ret 0 -> 要把 0 放入 a0 寄存器
    auto val = ret_inst->ret_value.get();
    // Lv1 只有整数
    if (auto int_val = dynamic_cast<const IntegerIR*>(val)) {
        os << "  li a0, " << int_val->value << std::endl;
    }
    os << "  ret" << std::endl;
}