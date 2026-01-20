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
    std::string get_reg_id(int ir_inst_id);

private:
    const ProgramIR* program;

    void visit(const FunctionIR* func_ir);
    void visit(const BasicBlockIR* block_ir);
    void visit(const ReturnIR* ret_ir);
    void visit(const BinaryArithmeticIR* binary_op_ir);

    std::vector<std::string> reg_names = {
        "x0", 
        "t0", "t1", "t2", "t3", "t4", "t5", "t6",
        "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    };
};

// =========================================================
// 方法实现
// =========================================================

inline std::string RiscvGenerator::get_reg_id(int ir_inst_id) {
    int phy_id = ir_inst_id + 3;
    return reg_names[phy_id];
}

inline void RiscvGenerator::generate() {
    std::cout << "  .text" << std::endl;
    for (const auto& func : program->funcs) {
        visit(func.get());
    }
}

inline void RiscvGenerator::visit(const FunctionIR* func_ir) {
    // KoopaIR 函数名带@，riscv没有，所以要去掉第一个字符
    std::string name = func_ir->func_name.substr(0); 
    
    std::cout << "  .globl " << name << std::endl;
    std::cout << name << ":" << std::endl;
    
    for (const auto& block : func_ir->basic_blocks) {
        visit(block.get());
    }
}

inline void RiscvGenerator::visit(const BasicBlockIR* block_ir) {
    for (const auto& inst : block_ir->insts) {
        if (auto ret_inst = dynamic_cast<const ReturnIR*>(inst.get())) {
            visit(ret_inst);
        }
        else if (auto bin_inst = dynamic_cast<const BinaryArithmeticIR*>(inst.get())) {
            visit(bin_inst);
        }
    }
}

inline void RiscvGenerator::visit(const ReturnIR* ret_ir) {
    auto val_ir = ret_ir->ret_value.type != Operand::VOID ? &ret_ir->ret_value : nullptr;
    if (val_ir) {
        if (ret_ir->ret_value.type == Operand::IMM) {
            std::cout << "  li a0, " << ret_ir->ret_value.val << std::endl;
        } 
        else if (ret_ir->ret_value.type == Operand::ID) {
            std::cout << "  mv a0, " << get_reg_id(ret_ir->ret_value.val) << std::endl;
        }
    }
    std::cout << "  ret" << std::endl;
}

inline void RiscvGenerator::visit(const BinaryArithmeticIR* bin_op_ir) {
    std::string target_reg_id = get_reg_id(bin_op_ir->target);

    // 加载操作数到寄存器t0和t1
    if (bin_op_ir->op1.type == Operand::IMM) {
        std::cout << "  li    t0, " << bin_op_ir->op1.val << std::endl;
    } else {
        std::cout << "  mv    t0, " << get_reg_id(bin_op_ir->op1.val) << std::endl;
    }
    if (bin_op_ir->op2.type == Operand::IMM) {
        std::cout << "  li    t1, " << bin_op_ir->op2.val << std::endl;
    } else {
        std::cout << "  mv    t1, " << get_reg_id(bin_op_ir->op2.val) << std::endl;
    }

    // 根据操作类型生成对应的汇编指令
    switch (bin_op_ir->op) {
        case BinaryOpType::NE:
            std::cout << "  sub   " << target_reg_id << ", t0, t1" << std::endl;
            std::cout << "  snez  " << target_reg_id << ", " << target_reg_id << std::endl; // snez: set not equal zero
            break;
        case BinaryOpType::EQ:
            std::cout << "  sub   " << target_reg_id << ", t0, t1" << std::endl;
            std::cout << "  seqz  " << target_reg_id << ", " << target_reg_id << std::endl; // seqz: set equal zero
            break;
        case BinaryOpType::GT:
            std::cout << "  slt   " << target_reg_id << ", t1, t0" << std::endl;
            break;
        case BinaryOpType::LT:
            std::cout << "  slt   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::GE: 
            std::cout << "  slt   " << target_reg_id << ", t0, t1" << std::endl;
            std::cout << "  seqz  " << target_reg_id << ", " << target_reg_id << std::endl;
            break;
        case BinaryOpType::LE: 
            std::cout << "  sgt   " << target_reg_id << ", t0, t1" << std::endl;
            std::cout << "  seqz  " << target_reg_id << ", " << target_reg_id << std::endl;
            break;
        case BinaryOpType::ADD:
            std::cout << "  add   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::SUB:
            std::cout << "  sub   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::MUL:
            std::cout << "  mul   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::DIV:
            std::cout << "  div   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::MOD:
            std::cout << "  rem   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::AND:
            std::cout << "  and   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::OR:
            std::cout << "  or    " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::XOR:
            std::cout << "  xor   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::SHL:
            std::cout << "  sll   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::SHR:
            std::cout << "  srl   " << target_reg_id << ", t0, t1" << std::endl;
            break;
        case BinaryOpType::SAR:
            std::cout << "  sra   " << target_reg_id << ", t0, t1" << std::endl;
            break;
    }
}