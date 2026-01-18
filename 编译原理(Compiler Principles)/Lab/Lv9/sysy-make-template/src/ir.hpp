#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

// =========================================================
// 前向声明
// =========================================================

class BaseIR;
class ProgramIR;
class FunctionIR;
class BasicBlockIR;
class InstructionIR;
class ReturnIR;
class BinaryArithmeticIR;
class AllocIR;
class LoadIR;
class StoreIR;
class BranchIR;
class JumpIR;
class CallIR;
class GlobalAllocIR;

enum class BinaryOpType {
    NE, EQ, GT, LT, GE, LE,
    ADD, SUB, MUL, DIV, MOD,
    AND, OR, XOR,
    SHL, SHR, SAR
};

struct Operand {
    enum Type { VOID, IMM, ID, VAR, ARG } type;
    
    int val; // IMM, ID
    std::string name; // VAR, ARG
    Operand() : type(VOID), val(0) {}
    Operand(Type t, int v) : type(t), val(v) {}
    Operand(std::string n) : type(VAR), val(0), name(n) {}

    static Operand make_arg(int index, std::string name) {
        Operand op;
        op.type = ARG;
        op.val = index;  // 索引，供 ASM 使用 (a0, a1...)
        op.name = name;  // 名字，供 IR dump 使用 (%x)
        return op;
    }

    void dump() const {
        if (type == IMM) {
            std::cout << val;
        } 
        else if (type == ID) {
            std::cout << "%" << val;
        } 
        else if (type == VAR) {
            std::cout << "@" << name;
        }
        else if (type == ARG) {
            std::cout << "%" << name;
        }
    }

    operator bool() const { return type != VOID; }
};

// =========================================================
// 类定义
// =========================================================

class BaseIR {
public:
    virtual ~BaseIR() = default;
    virtual void dump() = 0;
};

class ProgramIR : public BaseIR {
public:
    std::vector<std::unique_ptr<GlobalAllocIR>> globals;
    std::vector<std::unique_ptr<FunctionIR>> funcs;
    Operand cur_val;
    int cur_inst_id = 0;
    FunctionIR *cur_func = nullptr;
    BasicBlockIR *cur_block = nullptr;
    std::vector<std::string> loop_entry_stack; // 记录 continue 跳转的目标
    std::vector<std::string> loop_end_stack; // 记录 break 跳转的目标
    void dump() override;
};

class FunctionIR : public BaseIR {
public:
    std::string func_name, func_type;
    std::vector<std::string> params;
    Operand ret_value;
    std::vector<std::unique_ptr<BasicBlockIR>> basic_blocks;
    void dump() override;
};

class BasicBlockIR : public BaseIR {
public:
    std::string basic_block_name;
    std::vector<std::unique_ptr<InstructionIR>> insts;
    void dump() override;
};

class GlobalAllocIR : public BaseIR {
public:
    std::string name;
    int init_val;
    
    GlobalAllocIR(std::string name, int val) 
        : name(name), init_val(val) {}
    void dump() override;
};

class InstructionIR : public BaseIR {
public:
    virtual ~InstructionIR() = default;
    virtual void dump()  override = 0;
};

class CallIR : public InstructionIR {
public:
    std::string func_name;
    std::vector<Operand> args;
    int target; // 如果返回值是 void，此字段无意义

    CallIR(std::string name, std::vector<Operand> args, int target)
        : func_name(name), args(args), target(target) {}

    void dump() override {
        if (target != -1) // 假设 -1 表示 void
            std::cout << "  %" << target << " = ";
        else
            std::cout << "  ";
        
        std::cout << "call @" << func_name << "(";
        for (size_t i = 0; i < args.size(); ++i) {
            args[i].dump();
            if (i != args.size() - 1) std::cout << ", ";
        }
        std::cout << ")" << std::endl;
    }
};

class ReturnIR : public InstructionIR {
public:
    Operand ret_value;
    void dump()  override;
};

class BinaryArithmeticIR : public InstructionIR {
public:
    BinaryOpType op;
    int target;
    Operand op1, op2;

    BinaryArithmeticIR(BinaryOpType op, int target, Operand op1, Operand op2)
        : op(op), target(target), op1(op1), op2(op2) {}

    void dump() override;
};

class AllocIR : public InstructionIR {
public:
    std::string var_name;
    AllocIR(std::string name) : var_name(name) {}
    void dump() override;
};

class LoadIR : public InstructionIR {
public:
    int target;
    Operand src_addr;
    LoadIR(int target, Operand src) : target(target), src_addr(src) {}
    void dump() override;
};

class StoreIR : public InstructionIR {
public:
    Operand value;
    Operand dst_addr;
    StoreIR(Operand val, Operand dst) : value(val), dst_addr(dst) {}
    void dump() override;
};

class BranchIR : public InstructionIR {
public:
    Operand cond;
    std::string true_label;
    std::string false_label;

    BranchIR(Operand cond, std::string true_label, std::string false_label)
        : cond(cond), true_label(true_label), false_label(false_label) {}

    void dump() override;
};

class JumpIR : public InstructionIR {
public:
    std::string target_label;

    JumpIR(std::string target_label) : target_label(target_label) {}

    void dump() override;
};

// =========================================================
// 方法实现
// =========================================================

inline void ProgramIR::dump()  {
    std::cout << "decl @getint(): i32" << std::endl;
    std::cout << "decl @getch(): i32" << std::endl;
    std::cout << "decl @getarray(*i32): i32" << std::endl;
    std::cout << "decl @putint(i32)" << std::endl;
    std::cout << "decl @putch(i32)" << std::endl;
    std::cout << "decl @putarray(i32, *i32)" << std::endl;
    std::cout << "decl @starttime()" << std::endl;
    std::cout << "decl @stoptime()" << std::endl;
    std::cout << std::endl;

    for (auto &global : globals) {
        global->dump();
    }
    if (!globals.empty()) std::cout << std::endl;

    for( auto &func : funcs) {
        func->dump();
    }
}

inline void FunctionIR::dump()  {
    std::cout << "fun @" << func_name << "(";
    
    for (size_t i = 0; i < params.size(); ++i) {
        std::cout << "%" << params[i] << ": i32";
        if (i < params.size() - 1) std::cout << ", ";
    }
    std::cout << ")";

    if (func_type != "void") {
        std::cout << ": " << func_type;
    }

    std::cout << " {" << std::endl;
    for (auto &basic_block : basic_blocks)
        basic_block->dump();
    std::cout << "}" << std::endl;
}

inline void BasicBlockIR::dump()  {
    std::cout << "%" << basic_block_name << ":" << std::endl;
    for( auto &inst : insts)
        inst->dump();
}

inline void ReturnIR::dump()  {
    std::cout << "  ret ";
    if (ret_value) {
        ret_value.dump();
    }
    std::cout << std::endl;
}

inline void BinaryArithmeticIR::dump() {
    std::cout << "  %" << target << " = ";
    
    switch (op) {
        case BinaryOpType::NE: std::cout << "ne"; break;
        case BinaryOpType::EQ: std::cout << "eq"; break;
        case BinaryOpType::GT: std::cout << "gt"; break;
        case BinaryOpType::LT: std::cout << "lt"; break;
        case BinaryOpType::GE: std::cout << "ge"; break;
        case BinaryOpType::LE: std::cout << "le"; break;
        case BinaryOpType::ADD: std::cout << "add"; break;
        case BinaryOpType::SUB: std::cout << "sub"; break;
        case BinaryOpType::MUL: std::cout << "mul"; break;
        case BinaryOpType::DIV: std::cout << "div"; break;
        case BinaryOpType::MOD: std::cout << "mod"; break;
        case BinaryOpType::AND: std::cout << "and"; break;
        case BinaryOpType::OR:  std::cout << "or"; break;
        case BinaryOpType::XOR: std::cout << "xor"; break;
        case BinaryOpType::SHL: std::cout << "shl"; break;
        case BinaryOpType::SHR: std::cout << "shr"; break;
        case BinaryOpType::SAR: std::cout << "sar"; break;
        default: std::cout << "\nIR error: unknown binary op\n"; break;
    }

    std::cout << " ";
    op1.dump();
    std::cout << ", ";
    op2.dump();
    std::cout << std::endl;
}

inline void AllocIR::dump() {
    std::cout << "  @" << var_name << " = alloc i32" << std::endl;
}

inline void LoadIR::dump() {
    std::cout << "  %" << target << " = load ";
    src_addr.dump();
    std::cout << std::endl;
}

inline void StoreIR::dump() {
    std::cout << "  store ";
    value.dump();
    std::cout << ", ";
    dst_addr.dump();
    std::cout << std::endl;
}

inline void BranchIR::dump() {
    std::cout << "  br ";
    cond.dump();
    std::cout << ", %" << true_label << ", %" << false_label << std::endl;
}

inline void JumpIR::dump() {
    std::cout << "  jump %" << target_label << std::endl;
}

inline void GlobalAllocIR::dump() {    
    // global @var = alloc i32, zeroinit
    // global @var = alloc i32, 10
    std::cout << "global @" << name << " = alloc i32, ";
    if (init_val == 0) {
        std::cout << "zeroinit";
    } else {
        std::cout << init_val;
    }
    std::cout << std::endl;
}