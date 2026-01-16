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
class AddExpAST;
class MulExpAST;
class LOrExpAST;
class LAndExpAST;
class EqExpAST;
class RelExpAST;
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
	std::unique_ptr<LOrExpAST> lor_exp_ast;
	void dump() const override;
	void generate_ir(ProgramIR* ir) const override;
};

class AddExpAST : public BaseAST {
public:
	// 有op
    std::unique_ptr<AddExpAST> left_ast;
    std::string op;
    std::unique_ptr<MulExpAST> right_ast;
    // 没有op
    std::unique_ptr<MulExpAST> mul_exp_ast; 

    void dump() const override;
    void generate_ir(ProgramIR* ir) const override;
};

class MulExpAST : public BaseAST {
public:
	// 有op
    std::unique_ptr<MulExpAST> left_ast;
    std::string op;
    std::unique_ptr<UnaryExpAST> right_ast;
	// 没有op
    std::unique_ptr<UnaryExpAST> unary_exp_ast;

    void dump() const override;
    void generate_ir(ProgramIR* ir) const override;
};

class LOrExpAST : public BaseAST {
public:
    std::unique_ptr<LOrExpAST> left_ast;
    std::string op;
    std::unique_ptr<LAndExpAST> right_ast; // 指向下一级
    std::unique_ptr<LAndExpAST> land_exp_ast; // 单个情况
    void dump() const override;
    void generate_ir(ProgramIR* ir) const override;
};

class LAndExpAST : public BaseAST {
public:
    std::unique_ptr<LAndExpAST> left_ast;
    std::string op;
    std::unique_ptr<EqExpAST> right_ast;
    std::unique_ptr<EqExpAST> eq_exp_ast;
    void dump() const override;
    void generate_ir(ProgramIR* ir) const override;
};

class EqExpAST : public BaseAST {
public:
    std::unique_ptr<EqExpAST> left_ast;
    std::string op;
    std::unique_ptr<RelExpAST> right_ast;
    std::unique_ptr<RelExpAST> rel_exp_ast;
    void dump() const override;
    void generate_ir(ProgramIR* ir) const override;
};

class RelExpAST : public BaseAST {
public:
    std::unique_ptr<RelExpAST> left_ast;
    std::string op;
    std::unique_ptr<AddExpAST> right_ast;
    std::unique_ptr<AddExpAST> add_exp_ast;
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
	int number;
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
	lor_exp_ast->dump();
	std::cout << " }";
}

inline void AddExpAST::dump() const {
    std::cout << "AddExpAST { ";
    if (mul_exp_ast) {
        mul_exp_ast->dump();
    } else {
        left_ast->dump();
        std::cout << " " << op << " ";
        right_ast->dump();
    }
    std::cout << " }";
}

inline void MulExpAST::dump() const {
    std::cout << "MulExpAST { ";
    if (unary_exp_ast) {
        unary_exp_ast->dump();
    } else {
        left_ast->dump();
        std::cout << " " << op << " ";
        right_ast->dump();
    }
    std::cout << " }";
}

inline void LOrExpAST::dump() const {
    std::cout << "OrExpAST { ";
    if (land_exp_ast) {
        land_exp_ast->dump();
    } else {
        left_ast->dump();
        std::cout << " " << op << " ";
        right_ast->dump();
    }
    std::cout << " }";
}

inline void LAndExpAST::dump() const {
    std::cout << "AndExpAST { ";
    if (eq_exp_ast) {
        eq_exp_ast->dump();
    } else {
        left_ast->dump();
        std::cout << " " << op << " ";
        right_ast->dump();
    }
    std::cout << " }";
}

inline void EqExpAST::dump() const {
    std::cout << "EqExpAST { ";
    if (rel_exp_ast) {
        rel_exp_ast->dump();
    } else {
        left_ast->dump();
        std::cout << " " << op << " ";
        right_ast->dump();
    }
    std::cout << " }";
}

inline void RelExpAST::dump() const {
    std::cout << "RelExpAST { ";
    if (add_exp_ast) {
        add_exp_ast->dump();
    } else {
        left_ast->dump();
        std::cout << " " << op << " ";
        right_ast->dump();
    }
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
	ret_ir->ret_value = ir->cur_val;
	ir->cur_block->insts.push_back(std::move(ret_ir));
}

inline void ExpAST::generate_ir(ProgramIR* ir) const {
	lor_exp_ast->generate_ir(ir);
}

inline void AddExpAST::generate_ir(ProgramIR* ir) const {
    if (mul_exp_ast) {
        mul_exp_ast->generate_ir(ir);
    } else {
        left_ast->generate_ir(ir);
        Operand left_val = ir->cur_val;

        right_ast->generate_ir(ir);
        Operand right_val = ir->cur_val;

        int target = ir->cur_inst_id++;
        BinaryOpType type;
        if (op == "+") 
			type = BinaryOpType::ADD;
        else if (op == "-")
			type = BinaryOpType::SUB;
        auto inst = std::make_unique<BinaryArithmeticIR>(
            type, target, left_val, right_val
        );

        ir->cur_block->insts.push_back(std::move(inst));
        ir->cur_val = Operand(Operand::ID, target);
    }
}

inline void MulExpAST::generate_ir(ProgramIR* ir) const {
    if (unary_exp_ast) {
        unary_exp_ast->generate_ir(ir);
    } else {
        left_ast->generate_ir(ir);
        Operand left_val = ir->cur_val;

        right_ast->generate_ir(ir);
        Operand right_val = ir->cur_val;

        int target = ir->cur_inst_id++;
        BinaryOpType type;
        if (op == "*") 
			type = BinaryOpType::MUL;
        else if (op == "/") 
			type = BinaryOpType::DIV;
        else if (op == "%")
			type = BinaryOpType::MOD;
        auto inst = std::make_unique<BinaryArithmeticIR>(
            type, target, left_val, right_val
        );

        ir->cur_block->insts.push_back(std::move(inst));
        ir->cur_val = Operand(Operand::ID, target);
    }
}

inline void LOrExpAST::generate_ir(ProgramIR* ir) const {
    if (land_exp_ast) {
        land_exp_ast->generate_ir(ir);
    } else {
        left_ast->generate_ir(ir);
        Operand left_val = ir->cur_val;

        int temp1_id = ir->cur_inst_id++;
        auto ne1_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::NE, 
            temp1_id, 
            left_val, 
            Operand(Operand::IMM, 0)
        );
        ir->cur_block->insts.push_back(std::move(ne1_inst));
        Operand ne1_val = Operand(Operand::ID, temp1_id);

        right_ast->generate_ir(ir);
        Operand right_val = ir->cur_val;

        int temp2_id = ir->cur_inst_id++;
        auto ne2_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::NE, 
            temp2_id, 
            right_val, 
            Operand(Operand::IMM, 0)
        );
        ir->cur_block->insts.push_back(std::move(ne2_inst));
        Operand ne2_val = Operand(Operand::ID, temp2_id);

        int target = ir->cur_inst_id++;
        auto or_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::OR,
            target, 
            ne1_val, 
            ne2_val
        );
        ir->cur_block->insts.push_back(std::move(or_inst));

        ir->cur_val = Operand(Operand::ID, target);
    }
}

inline void LAndExpAST::generate_ir(ProgramIR* ir) const {
    if (eq_exp_ast) {
        eq_exp_ast->generate_ir(ir);
    } else {
        left_ast->generate_ir(ir);
        Operand left_val = ir->cur_val;

        int temp1_id = ir->cur_inst_id++;
        auto ne1_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::NE, 
            temp1_id, 
            left_val, 
            Operand(Operand::IMM, 0)
        );
        ir->cur_block->insts.push_back(std::move(ne1_inst));
        Operand ne1_val = Operand(Operand::ID, temp1_id);


        right_ast->generate_ir(ir);
        Operand right_val = ir->cur_val;

        int temp2_id = ir->cur_inst_id++;
        auto ne2_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::NE, 
            temp2_id, 
            right_val, 
            Operand(Operand::IMM, 0)
        );
        ir->cur_block->insts.push_back(std::move(ne2_inst));
        Operand ne2_val = Operand(Operand::ID, temp2_id);

        int target = ir->cur_inst_id++;
        auto and_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::AND,
            target, 
            ne1_val, 
            ne2_val
        );
        ir->cur_block->insts.push_back(std::move(and_inst));

        ir->cur_val = Operand(Operand::ID, target);
    }
}

inline void RelExpAST::generate_ir(ProgramIR* ir) const {
    if (add_exp_ast) {
        add_exp_ast->generate_ir(ir);
    } else {
        left_ast->generate_ir(ir);
        Operand left_val = ir->cur_val;
        right_ast->generate_ir(ir);
        Operand right_val = ir->cur_val;

        int target = ir->cur_inst_id++;
        BinaryOpType type;
        if (op == "<")
            type = BinaryOpType::LT;
        else if (op == ">")
            type = BinaryOpType::GT;
        else if (op == "<=")
            type = BinaryOpType::LE;
        else if (op == ">=")
            type = BinaryOpType::GE;
        
        auto inst = std::make_unique<BinaryArithmeticIR>(type, target, left_val, right_val);
        ir->cur_block->insts.push_back(std::move(inst));
        ir->cur_val = Operand(Operand::ID, target);
    }
}

inline void EqExpAST::generate_ir(ProgramIR* ir) const {
    if (rel_exp_ast) {
        rel_exp_ast->generate_ir(ir);
    } else {
        left_ast->generate_ir(ir);
        Operand left_val = ir->cur_val;
        right_ast->generate_ir(ir);
        Operand right_val = ir->cur_val;

        int target = ir->cur_inst_id++;
        BinaryOpType type;
        if (op == "==")
            type = BinaryOpType::EQ;
        else if (op == "!=")
            type = BinaryOpType::NE;

        auto inst = std::make_unique<BinaryArithmeticIR>(type, target, left_val, right_val);
        ir->cur_block->insts.push_back(std::move(inst));
        ir->cur_val = Operand(Operand::ID, target);
    }
}

inline void UnaryExpAST::generate_ir(ProgramIR* ir) const {
	if (primary_exp_ast) {
		primary_exp_ast->generate_ir(ir);
	} else if (unary_op_ast && unary_exp_ast) {
		unary_exp_ast->generate_ir(ir);	// 先求出被取负的值/id
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
		int target = ir->cur_inst_id++;
		auto inst = std::make_unique<BinaryArithmeticIR>(
			BinaryOpType::SUB,
			target,
			Operand(Operand::IMM, 0),
			ir->cur_val
		);
		ir->cur_block->insts.push_back(std::move(inst));
		ir->cur_val = Operand(Operand::ID, target);
	}
	else if (op == "!") {
		int target = ir->cur_inst_id++;

		auto inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::EQ,
            target,
            ir->cur_val,
            Operand(Operand::IMM, 0)
        );

		ir->cur_block->insts.push_back(std::move(inst));
		ir->cur_val = Operand(Operand::ID, target);
	}
}

inline void NumberAST::generate_ir(ProgramIR* ir) const {
	ir->cur_val = Operand(Operand::IMM, number);
}

inline std::unique_ptr<ProgramIR> generate_ir(const std::unique_ptr<BaseAST>& ast) {
    auto ir = std::make_unique<ProgramIR>();
    ast->generate_ir(ir.get());
    return ir;
}