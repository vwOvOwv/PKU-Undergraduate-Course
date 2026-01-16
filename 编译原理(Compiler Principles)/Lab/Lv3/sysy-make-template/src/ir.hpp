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

enum class BinaryOpType {
    NE, EQ, GT, LT, GE, LE,
    ADD, SUB, MUL, DIV, MOD,
    AND, OR, XOR,
    SHL, SHR, SAR
};

struct Operand {
    enum Type {VOID, IMM, ID } type;
    
    int val; 
    Operand() : type(VOID), val(0) {}
    Operand(Type t, int v) : type(t), val(v) {}

    void dump() const {
        if (type == IMM) {
            std::cout << val;
        } else if (type == ID) {
            std::cout << "%" << val;
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