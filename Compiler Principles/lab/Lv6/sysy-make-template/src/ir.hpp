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

enum class BinaryOpType {
    NE, EQ, GT, LT, GE, LE,
    ADD, SUB, MUL, DIV, MOD,
    AND, OR, XOR,
    SHL, SHR, SAR
};

struct Operand {
    enum Type { VOID, IMM, ID, VAR } type;
    
    int val; // IMM, ID
    std::string name; // VAR
    Operand() : type(VOID), val(0) {}
    Operand(Type t, int v) : type(t), val(v) {}
    Operand(std::string n) : type(VAR), val(0), name(n) {}

    void dump() const {
        if (type == IMM) {
            std::cout << val;
        } else if (type == ID) {
            std::cout << "%" << val;
        } else if (type == VAR) {
            std::cout << "@" << name; // 变量名前加 @
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
    std::vector<std::unique_ptr<FunctionIR>> funcs;
    Operand cur_val;
    int cur_inst_id = 0;
    FunctionIR *cur_func = nullptr;
    BasicBlockIR *cur_block = nullptr;
    void dump() override;
};

class FunctionIR : public BaseIR {
public:
    std::string func_name, func_type;
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

class InstructionIR : public BaseIR {
public:
    virtual ~InstructionIR() = default;
    virtual void dump()  override = 0;
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
    for( auto &func : funcs) {
        func->dump();
    }
}

inline void FunctionIR::dump()  {
    std::cout << "fun @" << func_name << "(): " << func_type << " {" << std::endl;
    for( auto &basic_block : basic_blocks)
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