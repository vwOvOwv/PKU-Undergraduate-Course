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
    bool has_call = false;
    std::map<std::string, int> var_offset_map;  // name -> offset
    std::map<int, int> val_offset_map; // id -> offset

    void visit(const FunctionIR* func_ir);
    void visit(const BasicBlockIR* block_ir);
    void visit(const ReturnIR* ret_ir);
    void visit(const BinaryArithmeticIR* binary_op_ir);
    void visit(const AllocIR* alloc_ir);
    void visit(const LoadIR* load_ir);
    void visit(const StoreIR* store_ir);
    void visit(const BranchIR* branch_ir);
    void visit(const JumpIR* jump_ir);
    void visit(const CallIR* call_ir);
    void visit(const GlobalAllocIR* global_ir);

    void load_to_reg(const Operand& op, const std::string& reg_name);
    void store_from_reg(const std::string& reg_name, int target);
    void calculate_stack_size(const FunctionIR* func_ir);
    bool check_has_call(const FunctionIR* func_ir);
};

// =========================================================
// 方法实现
// =========================================================

inline void RiscvGenerator::generate() {
    if (!program->globals.empty()) {
        for (const auto& global : program->globals) {
            visit(global.get());
        }
    }

    std::cout << "  .text" << std::endl;
    for (const auto& func : program->funcs) {
        visit(func.get());
    }
}

inline bool RiscvGenerator::check_has_call(const FunctionIR* func_ir) {
    for(const auto& block : func_ir->basic_blocks) {
        for(const auto& inst : block->insts) {
            if(dynamic_cast<const CallIR*>(inst.get())) return true;
        }
    }
    return false;
}

inline void RiscvGenerator::visit(const FunctionIR* func_ir) {
    has_call = check_has_call(func_ir);
    calculate_stack_size(func_ir); 

    std::string name = func_ir->func_name;
    std::cout << "  .globl " << name << std::endl;
    std::cout << name << ":" << std::endl;

    // 分配栈帧
    if (stack_size > 0) {
        if (stack_size <= 2048) { // -2048 fits in imm12
            std::cout << "  addi  sp, sp, -" << stack_size << std::endl;
        } else {
            std::cout << "  li    t0, -" << stack_size << std::endl;
            std::cout << "  add   sp, sp, t0" << std::endl;
        }
    }

    if (has_call) {
        int ra_offset = stack_size - 4;
        std::cout << "  sw    ra, " << ra_offset << "(sp)" << std::endl;
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
        else if (auto branch = dynamic_cast<const BranchIR*>(inst.get()))
            visit(branch);
        else if (auto jump = dynamic_cast<const JumpIR*>(inst.get()))
            visit(jump);
        else if (auto call = dynamic_cast<const CallIR*>(inst.get()))
            visit(call);
    }
}

inline void RiscvGenerator::visit(const ReturnIR* ret_ir) {
    if (ret_ir->ret_value.type != Operand::VOID) {
        load_to_reg(ret_ir->ret_value, "a0"); // 返回值存入 a0
    }
    if (has_call) {
        int ra_offset = stack_size - 4;
        std::cout << "  lw    ra, " << ra_offset << "(sp)" << std::endl;
    }
    if (stack_size > 0) {
        if (stack_size <= 2047) {
            std::cout << "  addi  sp, sp, " << stack_size << std::endl;
        } else {
            std::cout << "  li    t0, " << stack_size << std::endl;
            std::cout << "  add   sp, sp, t0" << std::endl;
        }
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
    std::string name = load_ir->src_addr.name;
    
    // 检查是否为栈上的局部变量
    if (var_offset_map.count(name)) {
        int offset = var_offset_map[name];
        if (offset >= -2048 && offset <= 2047) {
            std::cout << "  lw    t0, " << offset << "(sp)" << std::endl;
        } else {
            std::cout << "  li    t2, " << offset << std::endl;
            std::cout << "  add   t2, t2, sp" << std::endl;
            std::cout << "  lw    t0, 0(t2)" << std::endl;
    }
    } 
    else {
        // 不在栈映射中，认为是全局变量
        // lui + lw (或使用伪指令 la)
        std::cout << "  la    t0, " << name << std::endl;
        std::cout << "  lw    t0, 0(t0)" << std::endl;
    }
    store_from_reg("t0", load_ir->target);
}

inline void RiscvGenerator::visit(const StoreIR* store_ir) {
    std::string name = store_ir->dst_addr.name;
    load_to_reg(store_ir->value, "t0");
    if (var_offset_map.count(name)) {
        // 局部变量
        int offset = var_offset_map[name];
        if (offset >= -2048 && offset <= 2047) {
            std::cout << "  sw    t0, " << offset << "(sp)" << std::endl;
        } else {
            std::cout << "  li    t2, " << offset << std::endl;
            std::cout << "  add   t2, t2, sp" << std::endl;
            std::cout << "  sw    t0, 0(t2)" << std::endl;
        }
    } 
    else {
        // 全局变量 store
        std::cout << "  la    t1, " << name << std::endl;
        std::cout << "  sw    t0, 0(t1)" << std::endl;
    }
}

inline void RiscvGenerator::visit(const BranchIR* branch_ir) {
    // 加载条件变量到 t0
    load_to_reg(branch_ir->cond, "t0");
    // 如果 t0 != 0，跳转到 true_label
    std::cout << "  bnez  t0, ." << branch_ir->true_label << std::endl;
    // 否则（fallthrough），跳转到 false_label
    std::cout << "  j     ." << branch_ir->false_label << std::endl;
}

inline void RiscvGenerator::visit(const JumpIR* jump_ir) {
    // 无条件跳转
    std::cout << "  j     ." << jump_ir->target_label << std::endl;
}

inline void RiscvGenerator::visit(const CallIR* call_ir) {
    for (size_t i = 0; i < call_ir->args.size() && i < 8; ++i) {
        std::string reg = "a" + std::to_string(i);
        load_to_reg(call_ir->args[i], reg);
    }
    for (size_t i = 8; i < call_ir->args.size(); ++i) {
        load_to_reg(call_ir->args[i], "t0");
        int offset = (i - 8) * 4;
        std::cout << "  sw    t0, " << offset << "(sp)" << std::endl;
    }
    std::cout << "  call  " << call_ir->func_name << std::endl; 
    if (call_ir->target != -1) {
        store_from_reg("a0", call_ir->target);
    }
}

inline void RiscvGenerator::visit(const GlobalAllocIR* global_ir) {
    std::cout << "  .data" << std::endl;             // 切换到数据段
    std::cout << "  .globl " << global_ir->name << std::endl;
    std::cout << "  .align 2" << std::endl;          // 4字节对齐
    std::cout << "  .type " << global_ir->name << ", @object" << std::endl; // 声明符号类型为对象
    std::cout << "  .size " << global_ir->name << ", 4" << std::endl;       // 声明大小为4字节
    std::cout << global_ir->name << ":" << std::endl;
    std::cout << "  .word " << global_ir->init_val << std::endl;
    std::cout << std::endl;
}

// calculate_stack_size, load_to_reg, store_from_reg

inline void RiscvGenerator::calculate_stack_size(const FunctionIR* func_ir) {
    var_offset_map.clear();
    val_offset_map.clear();
    stack_size = 0;

    int max_call_args = 0;
    for (const auto& block : func_ir->basic_blocks) {
        for (const auto& inst : block->insts) {
            if (auto call = dynamic_cast<const CallIR*>(inst.get())) {
                if (call->args.size() > max_call_args) {
                    max_call_args = call->args.size();
                }
            }
        }
    }

    if (max_call_args > 8) {
        stack_size = (max_call_args - 8) * 4;
    }

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
            else if (auto call = dynamic_cast<const CallIR*>(inst.get())) {
                if (call->target != -1) {
                    val_offset_map[call->target] = stack_size;
                    stack_size += 4;
                }
            }
        }
    }
    
    if (has_call) {
        stack_size += 4; 
    }
    if (stack_size % 16 != 0) {
        stack_size = (stack_size / 16 + 1) * 16;
    }
}

inline void RiscvGenerator::load_to_reg(const Operand& op, const std::string& reg_name) {
    if (op.type == Operand::IMM) {
        std::cout << "  li    " << reg_name << ", " << op.val << std::endl;
    } else if (op.type == Operand::ID) {
        int offset = val_offset_map[op.val];
        if (offset >= -2048 && offset <= 2047) {
            std::cout << "  lw    " << reg_name << ", " << offset << "(sp)" << std::endl;
        } else {
            std::cout << "  li    t2, " << offset << std::endl;
            std::cout << "  add   t2, t2, sp" << std::endl;
            std::cout << "  lw    " << reg_name << ", 0(t2)" << std::endl;
        }
    }
    else if (op.type == Operand::ARG) {
        if (op.val < 8) {
            std::cout << "  mv    " << reg_name << ", a" << op.val << std::endl;
        }
        else {
            int offset = stack_size + (op.val - 8) * 4;
            std::cout << "  lw    " << reg_name << ", " << offset << "(sp)" << std::endl;
        }
    }
    // 不需要处理VAR(@x)
}

inline void RiscvGenerator::store_from_reg(const std::string& reg_name, int target) {
    int offset = val_offset_map[target];
    if (offset >= -2048 && offset <= 2047) {
        std::cout << "  sw    " << reg_name << ", " << offset << "(sp)" << std::endl;
    } else {
        std::cout << "  li    t2, " << offset << std::endl;
        std::cout << "  add   t2, t2, sp" << std::endl;
        std::cout << "  sw    " << reg_name << ", 0(t2)" << std::endl;
    }
}