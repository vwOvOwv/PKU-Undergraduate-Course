#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "ir.hpp"

// =========================================================
// 前向声明
// =========================================================

class BaseAST;
class CompUnitAST;
class FuncDefAST;
class FuncTypeAST;
class BlockAST;
class StmtAST;
std::unique_ptr<ProgramIR> generate_ir(const std::unique_ptr<BaseAST>& ast);

// =========================================================
// 类定义
// =========================================================

class BaseAST {
public:
	virtual ~BaseAST() = default;
	virtual void dump() const = 0;
	virtual void generate_ir(ProgramIR* ir) const = 0;
};

class CompUnitAST : public BaseAST {
public:
	std::unique_ptr<FuncDefAST> func_def;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class FuncDefAST : public BaseAST {
public:
	std::unique_ptr<FuncTypeAST> ret_type;
	std::string func_name;
	std::unique_ptr<BlockAST> block;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class FuncTypeAST : public BaseAST {
public:
	std::string type;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class BlockAST : public BaseAST {
public:
	std::unique_ptr<StmtAST> stmt;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

// Statement
class StmtAST : public BaseAST {
public:
	int number;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

// =========================================================
// 方法实现
// =========================================================

inline void CompUnitAST::dump() const {
	std::cout << "CompUnitAST { ";
	func_def->dump();
	std::cout << " }";
}

inline void FuncDefAST::dump() const {
	std::cout << "FuncDefAST { ";
	ret_type->dump();
	std::cout << ", " << func_name << ", ";
	block->dump();
	std::cout << " }";
}

inline void FuncTypeAST::dump() const {
	std::cout << "FuncTypeAST { " << type << " }";
}

inline void BlockAST::dump() const {
	std::cout << "BlockAST { ";
	stmt->dump();
	std::cout << " }";
}

inline void StmtAST::dump() const {
	std::cout << "StmtAST { ";
	std::cout << number;
	std::cout << " }";
}


inline void CompUnitAST::generate_ir(ProgramIR* ir) const {
    func_def->generate_ir(ir);
}

inline void FuncDefAST::generate_ir(ProgramIR* ir) const {
	auto func = std::make_unique<FunctionIR>();
	func->func_name = func_name;
	if(ret_type->type == "int")
		func->ret_type = "i32";
	else
		func->ret_type = ret_type->type;
	ir->cur_func = func.get();
	ir->funcs.push_back(std::move(func));
	block->generate_ir(ir);
}

inline void FuncTypeAST::generate_ir(ProgramIR* ir) const {}

inline void BlockAST::generate_ir(ProgramIR* ir) const {
    auto basic_block = std::make_unique<BasicBlockIR>();
    basic_block->basic_block_name = "entry";
    ir->cur_block = basic_block.get();
    ir->cur_func->basic_blocks.push_back(std::move(basic_block));
    stmt->generate_ir(ir);
}

inline void StmtAST::generate_ir(ProgramIR* ir) const {
	auto val = std::make_unique<IntegerIR>();
	val->value = number;
	auto inst = std::make_unique<ReturnIR>();
	inst->ret_value = std::move(val);
	ir->cur_block->insts.push_back(std::move(inst));
}

inline std::unique_ptr<ProgramIR> generate_ir(const std::unique_ptr<BaseAST>& ast) {
    auto ir = std::make_unique<ProgramIR>();
    ast->generate_ir(ir.get());
    return ir;
}