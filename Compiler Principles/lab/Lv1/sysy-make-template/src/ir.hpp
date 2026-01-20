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
class InstructionIR;
class ReturnIR;

// =========================================================
// 类定义
// =========================================================

class BaseIR {
public:
	virtual ~BaseIR() = default;
    virtual void dump() const = 0;
};

class ProgramIR : public BaseIR {
public:
    // Lv1 暂时不需要处理全局变量
    // std::vector<std::unique_ptr<ValueIR>> global_vars;
    std::vector<std::unique_ptr<FunctionIR>> funcs;
    FunctionIR *cur_func = nullptr;
    BasicBlockIR *cur_block = nullptr;
    void dump() const override;
};

class FunctionIR : public BaseIR {
public:
    std::string func_name, ret_type;
    std::vector<std::unique_ptr<BasicBlockIR>> basic_blocks;
    void dump() const override;
};

class BasicBlockIR : public BaseIR {
public:
    std::string basic_block_name;
    std::vector<std::unique_ptr<InstructionIR>> insts;
    void dump() const override;
};

class ValueIR : public BaseIR {
public:
    virtual ~ValueIR() = default;
    virtual void dump() const override = 0;
};

class IntegerIR : public ValueIR {
public:
    int value;
    void dump() const override;
};

class InstructionIR : public BaseIR {
public:
    virtual ~InstructionIR() = default;
    virtual void dump() const override = 0;
};

// Return
class ReturnIR : public InstructionIR {
public:
    std::unique_ptr<ValueIR> ret_value;
    void dump() const override;
};

// =========================================================
// 方法实现
// =========================================================

inline void ProgramIR::dump() const {
    for(const auto &func : funcs) {
        func->dump();
    }
}

inline void FunctionIR::dump() const {
    std::cout << "fun @" << func_name << "(): " << ret_type << " {" << std::endl;
    for(const auto &basic_block : basic_blocks)
        basic_block->dump();
    std::cout << "}" << std::endl;
}

inline void BasicBlockIR::dump() const {
    std::cout << "%" << basic_block_name << ":" << std::endl;
    for(const auto &inst : insts)
        inst->dump();
}

inline void IntegerIR::dump() const {
    std::cout << value;
}

inline void ReturnIR::dump() const {
    std::cout << "  ret ";
    if (ret_value) {
        ret_value->dump();
    }
    std::cout << std::endl;
}