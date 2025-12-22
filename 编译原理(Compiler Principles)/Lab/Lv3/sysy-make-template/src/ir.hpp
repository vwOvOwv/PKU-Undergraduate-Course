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
class ValueIR;
class IntegerIR;
class RegisterIR;
class InstructionIR;
class ReturnIR;
class EqIR;
class SubIR;

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
    std::unique_ptr<ValueIR> cur_val;   // 当前生成的值（具体的数 或 临时寄存器id）
    int next_reg_id = 0;    // 已经使用了的临时寄存器数量
    FunctionIR *cur_func = nullptr;
    BasicBlockIR *cur_block = nullptr;
    void dump() override;
};

class FunctionIR : public BaseIR {
public:
    std::string func_name, func_type;
    std::unique_ptr<ValueIR> ret_value;
    std::vector<std::unique_ptr<BasicBlockIR>> basic_blocks;
    void dump() override;
};

class BasicBlockIR : public BaseIR {
public:
    std::string basic_block_name;
    std::vector<std::unique_ptr<InstructionIR>> insts;
    void dump() override;
};

class ValueIR : public BaseIR {
public:
    int value;
    virtual ~ValueIR() = default;
    virtual void dump() override = 0;
};

class IntegerIR : public ValueIR {
public:
    IntegerIR() {}
    IntegerIR(int val) { value = val; }
    void dump() override;
};

class RegisterIR : public ValueIR {
public:
    RegisterIR() {}
    RegisterIR(int val) { value = val; }
    void dump() override;
};

class InstructionIR : public BaseIR {
public:
    virtual ~InstructionIR() = default;
    virtual void dump()  override = 0;
};

class ReturnIR : public InstructionIR {
public:
    std::unique_ptr<ValueIR> ret_value; // 返回的数值或临时寄存器id
    void dump()  override;
};

class EqIR : public InstructionIR {
public:
    std::unique_ptr<ValueIR> target, op1, op2;
    void dump()  override;
};

class SubIR : public InstructionIR {
public:
    std::unique_ptr<ValueIR> target, op1, op2;
    void dump()  override;
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

inline void IntegerIR::dump()  {
    std::cout << value;
}

inline void RegisterIR::dump()  {
    std::cout << '%' << value;
}

inline void ReturnIR::dump()  {
    std::cout << "  ret ";
    if (ret_value) {
        ret_value->dump();
    }
    std::cout << std::endl;
}

inline void EqIR::dump()  {
    std::cout << "  ";
    target->dump();
    std::cout << " = eq ";
    op1->dump();
    std::cout << ", ";
    op2->dump();
    std::cout << std::endl;
}

inline void SubIR::dump()  {
    std::cout << "  ";
    target->dump();
    std::cout << " = sub ";
    op1->dump();
    std::cout << ", ";
    op2->dump();
    std::cout << std::endl;
}