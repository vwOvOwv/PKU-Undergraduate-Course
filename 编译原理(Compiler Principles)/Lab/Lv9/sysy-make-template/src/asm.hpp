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
    int saved_reg_args_base = 0; 
    int ra_offset = 0;
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
    void visit(const GetElementPtrIR* gep_ir);
    void visit(const GetPtrIR* gep_ir);

    void load_to_reg(const Operand& op, const std::string& reg_name);
    void load_addr_to_reg(const Operand& op, const std::string& reg_name);
    void store_from_reg(const std::string& reg_name, int target);
    void calculate_stack_size(const FunctionIR* func_ir);
    bool check_has_call(const FunctionIR* func_ir);
    void safe_lw(const std::string& rd, int offset, const std::string& base);
    void safe_sw(const std::string& rs, int offset, const std::string& base);
};

// =========================================================
// 方法实现
// =========================================================

inline void RiscvGenerator::generate() {
    if (!program->globals.empty()) {
        std::cout << "  .data" << std::endl;
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

inline void RiscvGenerator::safe_lw(const std::string& rd, int offset, const std::string& base) {
    if (offset >= -2048 && offset <= 2047) {
        std::cout << "  lw    " << rd << ", " << offset << "(" << base << ")" << std::endl;
    } else {
        std::cout << "  li    t2, " << offset << std::endl;
        std::cout << "  add   t2, t2, " << base << std::endl;
        std::cout << "  lw    " << rd << ", 0(t2)" << std::endl;
    }
}

inline void RiscvGenerator::safe_sw(const std::string& rs, int offset, const std::string& base) {
    if (offset >= -2048 && offset <= 2047) {
        std::cout << "  sw    " << rs << ", " << offset << "(" << base << ")" << std::endl;
    } else {
        std::cout << "  li    t2, " << offset << std::endl;
        std::cout << "  add   t2, t2, " << base << std::endl;
        std::cout << "  sw    " << rs << ", 0(t2)" << std::endl;
    }
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
        safe_sw("ra", ra_offset, "sp");
    }

    int num_params = func_ir->params.size();
    int num_reg_params = std::min(num_params, 8);
    for (int i = 0; i < num_reg_params; ++i) {
        int offset = saved_reg_args_base + i * 4;
        std::string reg = "a" + std::to_string(i);
        safe_sw(reg, offset, "sp");
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
        else if (auto gep = dynamic_cast<const GetElementPtrIR*>(inst.get()))
            visit(gep);
        else if (auto ptr = dynamic_cast<const GetPtrIR*>(inst.get()))
            visit(ptr);
    }
}

inline void RiscvGenerator::visit(const ReturnIR* ret_ir) {
    if (ret_ir->ret_value.type != Operand::VOID) {
        load_to_reg(ret_ir->ret_value, "a0"); // 返回值存入 a0
    }
    if (has_call) {
        safe_lw("ra", this->ra_offset, "sp");
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
    if (load_ir->src_addr.type == Operand::VAR) {
        std::string name = load_ir->src_addr.name;
        if (var_offset_map.count(name)) { // 栈上局部变量
            int offset = var_offset_map[name];
            safe_lw("t0", offset, "sp");
        } else { // 全局变量
            std::cout << "  la    t0, " << name << std::endl;
            std::cout << "  lw    t0, 0(t0)" << std::endl;
        }
    }
    else if (load_ir->src_addr.type == Operand::ID) {
        load_to_reg(load_ir->src_addr, "t1");
        std::cout << "  lw    t0, 0(t1)" << std::endl;
    }
    store_from_reg("t0", load_ir->target);
}

inline void RiscvGenerator::visit(const StoreIR* store_ir) {
    load_to_reg(store_ir->value, "t0"); // 值在 t0

    if (store_ir->dst_addr.type == Operand::VAR) {
        std::string name = store_ir->dst_addr.name;
        if (var_offset_map.count(name)) {
            int offset = var_offset_map[name];
            safe_sw("t0", offset, "sp");
        } else {
            std::cout << "  la    t1, " << name << std::endl;
            std::cout << "  sw    t0, 0(t1)" << std::endl;
        }
    }
    else if (store_ir->dst_addr.type == Operand::ID) {
        load_to_reg(store_ir->dst_addr, "t1");
        std::cout << "  sw    t0, 0(t1)" << std::endl;
    }
}

inline void RiscvGenerator::visit(const GetElementPtrIR* gep_ir) {
    load_addr_to_reg(gep_ir->base, "t0");
    load_to_reg(gep_ir->offset, "t1");
    std::cout << "  slli  t1, t1, 2" << std::endl;
    std::cout << "  add   t0, t0, t1" << std::endl;
    store_from_reg("t0", gep_ir->target);
}

inline void RiscvGenerator::visit(const GetPtrIR* gep_ir) {
    load_addr_to_reg(gep_ir->base, "t0");
    load_to_reg(gep_ir->offset, "t1");
    std::cout << "  slli  t1, t1, 2" << std::endl;
    std::cout << "  add   t0, t0, t1" << std::endl;
    store_from_reg("t0", gep_ir->target);
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
        safe_sw("t0", offset, "sp");
    }
    std::cout << "  call  " << call_ir->func_name << std::endl; 
    if (call_ir->target != -1) {
        store_from_reg("a0", call_ir->target);
    }
}

inline void RiscvGenerator::visit(const GlobalAllocIR* global_ir) {
    std::string name = global_ir->name;
    int size_bytes = global_ir->size * 4;

    std::cout << "  .globl " << name << std::endl;
    std::cout << "  .align 2" << std::endl;
    std::cout << "  .type " << name << ", @object" << std::endl;
    std::cout << "  .size " << name << ", " << size_bytes << std::endl;
    std::cout << name << ":" << std::endl;
    
    if (global_ir->init_vals.empty()) {
        std::cout << "  .zero " << size_bytes << std::endl;
    } else {
        for (int val : global_ir->init_vals) {
            std::cout << "  .word " << val << std::endl;
        }
        int initialized_bytes = global_ir->init_vals.size() * 4;
        if (initialized_bytes < size_bytes) {
            std::cout << "  .zero " << (size_bytes - initialized_bytes) << std::endl;
        }
    }
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

    saved_reg_args_base = stack_size;
    int num_params = func_ir->params.size();
    int num_reg_params = std::min(num_params, 8);
    stack_size += num_reg_params * 4;

    for (const auto& block : func_ir->basic_blocks) {
        for (const auto& inst : block->insts) {
            if (auto alloc = dynamic_cast<const AllocIR*>(inst.get())) {
                var_offset_map[alloc->var_name] = stack_size;
                stack_size += alloc->size * 4;
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
            else if (auto gep = dynamic_cast<const GetElementPtrIR*>(inst.get())) {
                val_offset_map[gep->target] = stack_size;
                stack_size += 4; 
            }
            else if (auto ptr = dynamic_cast<const GetPtrIR*>(inst.get())) {
                val_offset_map[ptr->target] = stack_size;
                stack_size += 4;
            }
        }
    }
    
    if (has_call) {
        ra_offset = stack_size;
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
        safe_lw(reg_name, offset, "sp");
    }
    else if (op.type == Operand::ARG) {
        if (op.val < 8) {
            int offset = saved_reg_args_base + op.val * 4;
            safe_lw(reg_name, offset, "sp");
        }
        else {
            int offset = stack_size + (op.val - 8) * 4;
            safe_lw(reg_name, offset, "sp");
        }
    }
    // 不需要处理VAR(@x)
}

inline void RiscvGenerator::load_addr_to_reg(const Operand& op, const std::string& reg_name) {
    if (op.type == Operand::VAR) {
        std::string name = op.name;
        if (var_offset_map.count(name)) {
            int offset = var_offset_map[name];
            std::cout << "  li    " << reg_name << ", " << offset << std::endl;
            std::cout << "  add   " << reg_name << ", " << reg_name << ", sp" << std::endl;
        } else {
            if (!name.empty() && name[0] == '@') name = name.substr(1);
            std::cout << "  la    " << reg_name << ", " << name << std::endl;
        }
    } else if (op.type == Operand::ID) {
        load_to_reg(op, reg_name);
    } else if (op.type == Operand::ARG) {
        load_to_reg(op, reg_name);
    }
}

inline void RiscvGenerator::store_from_reg(const std::string& reg_name, int target) {
    int offset = val_offset_map[target];
    safe_sw(reg_name, offset, "sp");
}