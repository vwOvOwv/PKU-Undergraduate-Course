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

class ExpAST;
class UnaryExpAST;
class PrimaryExpAST;
class UnaryOpAST;
class NumberAST;

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
	std::unique_ptr<FuncDefAST> func_def_ast;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class FuncDefAST : public BaseAST {
public:
	std::unique_ptr<FuncTypeAST> func_type_ast;
	std::string func_name;
	std::unique_ptr<BlockAST> block_ast;
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
	std::unique_ptr<StmtAST> stmt_ast;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class StmtAST : public BaseAST {
public:
	std::unique_ptr<ExpAST> exp_ast;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class ExpAST : public BaseAST {
public:
	std::unique_ptr<UnaryExpAST> unary_exp_ast;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class UnaryExpAST : public BaseAST {
public:
	std::unique_ptr<PrimaryExpAST> primary_exp_ast;
	std::unique_ptr<UnaryOpAST> unary_op_ast;
	std::unique_ptr<UnaryExpAST> unary_exp_ast;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class PrimaryExpAST : public BaseAST {
public:
	std::unique_ptr<ExpAST> exp_ast;
	std::unique_ptr<NumberAST> number_ast;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class UnaryOpAST : public BaseAST {
public:
	std::string op;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class NumberAST : public BaseAST {
public:
	int number;	// all numbers are 32-bit integers
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

// =========================================================
// 方法实现
// =========================================================

// dump 已经不用了，但是还是实现一下，可能可以用于 debug
inline void CompUnitAST::dump() const {
	std::cout << "CompUnitAST { ";
	func_def_ast->dump();
	std::cout << " }";
}

inline void FuncDefAST::dump() const {
	std::cout << "FuncDefAST { ";
	func_type_ast->dump();
	std::cout << ", " << func_name << ", ";
	block_ast->dump();
	std::cout << " }";
}

inline void FuncTypeAST::dump() const {
	std::cout << "FuncTypeAST { " << type << " }";
}

inline void BlockAST::dump() const {
	std::cout << "BlockAST { ";
	stmt_ast->dump();
	std::cout << " }";
}

inline void StmtAST::dump() const {
	std::cout << "StmtAST { ";
	exp_ast->dump();
	std::cout << " }";
}

inline void ExpAST::dump() const {
	std::cout << "ExpAST { ";
	unary_exp_ast->dump();
	std::cout << " }";
}

inline void PrimaryExpAST::dump() const {
	std::cout << "PrimaryExpAST { ";
	if (exp_ast) {
		std::cout << "(";
		exp_ast->dump();
		std::cout << ")";
	} else if (number_ast) {
		number_ast->dump();
	}
	std::cout << " }";
}

inline void NumberAST::dump() const {
	std::cout << "NumberAST { " << number << " }";
}

inline void UnaryExpAST::dump() const {
	std::cout << "UnaryExpAST { ";
	if (primary_exp_ast) {
		primary_exp_ast->dump();
	} else if (unary_op_ast && unary_exp_ast) {
		unary_op_ast->dump();
		unary_exp_ast->dump();
	}
	std::cout << " }";
}

inline void UnaryOpAST::dump() const {
	std::cout << "UnaryOpAST { " << op << " }";
}

// generate_ir

inline void CompUnitAST::generate_ir(ProgramIR* ir) const {
    func_def_ast->generate_ir(ir);
}

inline void FuncDefAST::generate_ir(ProgramIR* ir) const {
	auto func_ir = std::make_unique<FunctionIR>();
	func_ir->func_name = func_name;
	if(func_type_ast->type == "int")
		func_ir->func_type = "i32";
	else
		func_ir->func_type = func_type_ast->type;
	ir->cur_func = func_ir.get();
	ir->funcs.push_back(std::move(func_ir));

	block_ast->generate_ir(ir);
}

inline void FuncTypeAST::generate_ir(ProgramIR* ir) const {}

inline void BlockAST::generate_ir(ProgramIR* ir) const {
    auto basic_block_ir = std::make_unique<BasicBlockIR>();
    basic_block_ir->basic_block_name = "entry";
    ir->cur_block = basic_block_ir.get();
    ir->cur_func->basic_blocks.push_back(std::move(basic_block_ir));
    stmt_ast->generate_ir(ir);
}

inline void StmtAST::generate_ir(ProgramIR* ir) const {
	exp_ast->generate_ir(ir);
	auto ret_ir = std::make_unique<ReturnIR>();
	ret_ir->ret_value = std::move(ir->cur_val);
	ir->cur_block->insts.push_back(std::move(ret_ir));
}

inline void ExpAST::generate_ir(ProgramIR* ir) const {
	unary_exp_ast->generate_ir(ir);
}

inline void UnaryExpAST::generate_ir(ProgramIR* ir) const {
	if (primary_exp_ast) {
		primary_exp_ast->generate_ir(ir);
	} else if (unary_op_ast && unary_exp_ast) {
		unary_exp_ast->generate_ir(ir);	// 先求出被取负的值/临时寄存器id
		unary_op_ast->generate_ir(ir);
	}
}

inline void PrimaryExpAST::generate_ir(ProgramIR* ir) const {
	if (exp_ast) {
		exp_ast->generate_ir(ir);
	} else if (number_ast) {
		number_ast->generate_ir(ir);
	}
}

inline void UnaryOpAST::generate_ir(ProgramIR* ir) const {
	if (op == "+") {
		// 不用做任何操作
	}
	else if (op == "-") {
		int cur_reg_id = ir->next_reg_id++;
		auto sub_ir = std::make_unique<SubIR>();
		auto zero_ir = std::make_unique<IntegerIR>(0);
		auto reg_ir1 = std::make_unique<RegisterIR>(cur_reg_id);
		auto reg_ir2 = std::make_unique<RegisterIR>(cur_reg_id);
		
		sub_ir->target = std::move(reg_ir1);
		sub_ir->op1 = std::move(zero_ir);
		sub_ir->op2 = std::move(ir->cur_val);	// 此时 cur_val 存的是被取负的值/临时寄存器id

		ir->cur_val = std::move(reg_ir2);	// 更新 cur_val 为新的临时寄存器id
		ir->cur_block->insts.push_back(std::move(sub_ir));
	}
	else if (op == "!") {
		int cur_reg_id = ir->next_reg_id++;
		auto eq_ir = std::make_unique<EqIR>();
		auto zero_ir = std::make_unique<IntegerIR>(0);
		auto reg_ir1 = std::make_unique<RegisterIR>(cur_reg_id);
		auto reg_ir2 = std::make_unique<RegisterIR>(cur_reg_id);

		eq_ir->target = std::move(reg_ir1);
		eq_ir->op1 = std::move(ir->cur_val);
		eq_ir->op2 = std::move(zero_ir);

		ir->cur_val = std::move(reg_ir2);
		ir->cur_block->insts.push_back(std::move(eq_ir));
	}
}

inline void NumberAST::generate_ir(ProgramIR* ir) const {
	auto int_ir = std::make_unique<IntegerIR>();
	int_ir->value = number;
	ir->cur_val = std::move(int_ir);
}

inline std::unique_ptr<ProgramIR> generate_ir(const std::unique_ptr<BaseAST>& ast) {
    auto ir = std::make_unique<ProgramIR>();
    ast->generate_ir(ir.get());
    return ir;
}