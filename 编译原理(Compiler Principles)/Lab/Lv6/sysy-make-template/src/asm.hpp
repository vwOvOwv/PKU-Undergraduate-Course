#pragma once

#include "ir.hpp"
#include <string>
#include <map>
#include <iostream>

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

private:
    const ProgramIR* program;
    int stack_size = 0;
    std::map<std::string, int> var_offset_map;  // name -> offset
    std::map<int, int> val_offset_map; // id -> offset

    void visit(const FunctionIR* func_ir);
    void visit(const BasicBlockIR* block_ir);
    void visit(const ReturnIR* ret_ir);
    void visit(const BinaryArithmeticIR* binary_op_ir);
    void visit(const AllocIR* alloc_ir);
    void visit(const LoadIR* load_ir);
    void visit(const StoreIR* store_ir);

    void load_to_reg(const Operand& op, const std::string& reg_name);
    void store_from_reg(const std::string& reg_name, int target);
    void calculate_stack_size(const FunctionIR* func_ir);
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

inline void RiscvGenerator::visit(const FunctionIR* func_ir) {
    calculate_stack_size(func_ir); 

    std::string name = func_ir->func_name;
    std::cout << "  .globl " << name << std::endl;
    std::cout << name << ":" << std::endl;

    // 分配栈帧
    if (stack_size > 0) {
        std::cout << "  addi  sp, sp, -" << stack_size << std::endl;
    }

    for (const auto& block : func_ir->basic_blocks) {
        visit(block.get());
    }
}

inline void RiscvGenerator::visit(const BasicBlockIR* block_ir) {
    if (block_ir->basic_block_name != "entry") {
        std::cout << "." << block_ir->basic_block_name << ":" << std::endl;
    }

    for (const auto& inst : block_ir->insts) {
        if (auto ret = dynamic_cast<const ReturnIR*>(inst.get())) 
            visit(ret);
        else if (auto bin = dynamic_cast<const BinaryArithmeticIR*>(inst.get()))
            visit(bin);
        else if (auto alloc = dynamic_cast<const AllocIR*>(inst.get()))
            visit(alloc);
        else if (auto load = dynamic_cast<const LoadIR*>(inst.get()))
            visit(load);
        else if (auto store = dynamic_cast<const StoreIR*>(inst.get()))
            visit(store);
    }
}

inline void RiscvGenerator::visit(const ReturnIR* ret_ir) {
    if (ret_ir->ret_value.type != Operand::VOID) {
        load_to_reg(ret_ir->ret_value, "a0"); // 返回值存入 a0
    }
    // 恢复栈指针
    if (stack_size > 0) {
        std::cout << "  addi  sp, sp, " << stack_size << std::endl;
    }
    std::cout << "  ret" << std::endl;
}

inline void RiscvGenerator::visit(const BinaryArithmeticIR* bin_op_ir) {
    // 加载操作数到临时寄存器 t0, t1
    load_to_reg(bin_op_ir->op1, "t0");
    load_to_reg(bin_op_ir->op2, "t1");
    // 根据操作类型生成对应的汇编指令
    switch (bin_op_ir->op) {
        case BinaryOpType::NE:
            std::cout << "  sub   t0, t0, t1" << std::endl;
            std::cout << "  snez  t0, t0" << std::endl; // snez: set not equal zero
            break;
        case BinaryOpType::EQ:
            std::cout << "  sub   t0, t0, t1" << std::endl;
            std::cout << "  seqz  t0, t0" << std::endl; // seqz: set equal zero
            break;
        case BinaryOpType::GT:
            std::cout << "  slt   t0, t1, t0" << std::endl;
            break;
        case BinaryOpType::LT:
            std::cout << "  slt   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::GE: 
            std::cout << "  slt   t0, t0, t1" << std::endl;
            std::cout << "  seqz  t0, t0" << std::endl;
            break;
        case BinaryOpType::LE: 
            std::cout << "  sgt   t0, t0, t1" << std::endl;
            std::cout << "  seqz  t0, t0" << std::endl;
            break;
        case BinaryOpType::ADD:
            std::cout << "  add   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::SUB:
            std::cout << "  sub   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::MUL:
            std::cout << "  mul   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::DIV:
            std::cout << "  div   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::MOD:
            std::cout << "  rem   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::AND:
            std::cout << "  and   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::OR:
            std::cout << "  or    t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::XOR:
            std::cout << "  xor   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::SHL:
            std::cout << "  sll   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::SHR:
            std::cout << "  srl   t0, t0, t1" << std::endl;
            break;
        case BinaryOpType::SAR:
            std::cout << "  sra   t0, t0, t1" << std::endl;
            break;
    }
    // 将结果写回栈
    store_from_reg("t0", bin_op_ir->target);
}

inline void RiscvGenerator::visit(const AllocIR* alloc_ir) {}

inline void RiscvGenerator::visit(const LoadIR* load_ir) {
    int offset = var_offset_map[load_ir->src_addr.name];
    // t0 = Mem[sp + offset]
    std::cout << "  lw    t0, " << offset << "(sp)" << std::endl;
    // 将 t0 存入 %target
    store_from_reg("t0", load_ir->target);
}

inline void RiscvGenerator::visit(const StoreIR* store_ir) {
    // 将值加载到 t0
    load_to_reg(store_ir->value, "t0");
    // 找到 @dst 在栈上的偏移
    int offset = var_offset_map[store_ir->dst_addr.name];
    // Mem[sp + offset] = t0
    std::cout << "  sw    t0, " << offset << "(sp)" << std::endl;
}

// calculate_stack_size, load_to_reg, store_from_reg

inline void RiscvGenerator::calculate_stack_size(const FunctionIR* func_ir) {
    var_offset_map.clear();
    val_offset_map.clear();
    stack_size = 0;
    for (const auto& block : func_ir->basic_blocks) {
        for (const auto& inst : block->insts) {
            if (auto alloc = dynamic_cast<const AllocIR*>(inst.get())) {
                var_offset_map[alloc->var_name] = stack_size;
                stack_size += 4;
            }
            else if (auto load = dynamic_cast<const LoadIR*>(inst.get())) {
                val_offset_map[load->target] = stack_size;
                stack_size += 4;
            }
            else if (auto bin = dynamic_cast<const BinaryArithmeticIR*>(inst.get())) {
                val_offset_map[bin->target] = stack_size;
                stack_size += 4;
            }
        }
    }
    if (stack_size % 16 != 0) {
        stack_size = (stack_size / 16 + 1) * 16;
    }
}

inline void RiscvGenerator::load_to_reg(const Operand& op, const std::string& reg_name) {
    if (op.type == Operand::IMM) {
        std::cout << "  li    " << reg_name << ", " << op.val << std::endl;
    } else if (op.type == Operand::ID) {
        std::cout << "  lw    " << reg_name << ", " << val_offset_map[op.val] << "(sp)" << std::endl;
    }
    // 不需要处理VAR(@x)
}

inline void RiscvGenerator::store_from_reg(const std::string& reg_name, int target) {
    std::cout << "  sw    " << reg_name << ", " << val_offset_map[target] << "(sp)" << std::endl;
}