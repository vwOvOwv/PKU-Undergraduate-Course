#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include "ir.hpp"
#include "symtab.hpp"

// =========================================================
// 前向声明
// =========================================================

extern SymbolTable symbol_table;

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

class BTypeAST;
class DeclAST;
class ConstDeclAST;
class ConstDefAST;
class ConstInitValAST;
class ConstExpAST;
class LValAST;
class VarDeclAST;
class VarDefAST;
class InitValAST;

std::unique_ptr<ProgramIR> generate_ir(const std::unique_ptr<BaseAST>& ast);

// =========================================================
// 类定义
// =========================================================

class BaseAST {
public:
	virtual ~BaseAST() = default;
	virtual void generate_ir(ProgramIR* ir) const = 0;
    virtual int calculate_val() const {
        throw std::runtime_error("This node cannot be evaluated at compile time.");
    }
};

class CompUnitAST : public BaseAST {
public:
	std::unique_ptr<FuncDefAST> func_def;
	void generate_ir(ProgramIR* ir) const override;
};

class FuncDefAST : public BaseAST {
public:
	std::unique_ptr<FuncTypeAST> func_type;
	std::string func_name;
	std::unique_ptr<BlockAST> block;
	void generate_ir(ProgramIR* ir) const override;
};

class FuncTypeAST : public BaseAST {
public:
	std::string type;
	void generate_ir(ProgramIR* ir) const override;
};

class BlockAST : public BaseAST {
public:
    std::vector<std::unique_ptr<BaseAST>> body; 
    void generate_ir(ProgramIR* ir) const override;
};

class StmtAST : public BaseAST {};

class ReturnStmtAST : public StmtAST {
public:
    std::unique_ptr<ExpAST> exp;
    void generate_ir(ProgramIR* ir) const override;
};

class AssignStmtAST : public StmtAST {
public:
    std::unique_ptr<LValAST> lval;
    std::unique_ptr<ExpAST> exp;
    void generate_ir(ProgramIR* ir) const override;
};

class ExpStmtAST : public StmtAST {
public:
    std::unique_ptr<ExpAST> exp; // 如果是空语句 ";"，则此指针为空
    void generate_ir(ProgramIR* ir) const override;
};

class ExpAST : public BaseAST {
public:
	std::unique_ptr<LOrExpAST> lor_exp;
	void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class AddExpAST : public BaseAST {
public:
	// 有op
    std::unique_ptr<AddExpAST> left;
    std::string op;
    std::unique_ptr<MulExpAST> right;
    // 没有op
    std::unique_ptr<MulExpAST> mul_exp; 
    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class MulExpAST : public BaseAST {
public:
	// 有op
    std::unique_ptr<MulExpAST> left;
    std::string op;
    std::unique_ptr<UnaryExpAST> right;
	// 没有op
    std::unique_ptr<UnaryExpAST> unary_exp;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class LOrExpAST : public BaseAST {
public:
    std::unique_ptr<LOrExpAST> left;
    std::string op;
    std::unique_ptr<LAndExpAST> right;
    std::unique_ptr<LAndExpAST> land_exp;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class LAndExpAST : public BaseAST {
public:
    std::unique_ptr<LAndExpAST> left;
    std::string op;
    std::unique_ptr<EqExpAST> right;
    std::unique_ptr<EqExpAST> eq_exp;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class EqExpAST : public BaseAST {
public:
    std::unique_ptr<EqExpAST> left;
    std::string op;
    std::unique_ptr<RelExpAST> right;
    std::unique_ptr<RelExpAST> rel_exp;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class RelExpAST : public BaseAST {
public:
    std::unique_ptr<RelExpAST> left;
    std::string op;
    std::unique_ptr<AddExpAST> right;
    std::unique_ptr<AddExpAST> add_exp;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class UnaryExpAST : public BaseAST {
public:
	std::unique_ptr<PrimaryExpAST> primary_exp;
	std::unique_ptr<UnaryOpAST> unary_op;
	std::unique_ptr<UnaryExpAST> unary_exp;

	void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class PrimaryExpAST : public BaseAST {
public:
    std::unique_ptr<ExpAST> exp;
    std::unique_ptr<NumberAST> number;
    std::unique_ptr<LValAST> lval;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class UnaryOpAST : public BaseAST {
public:
	std::string op;

	void generate_ir(ProgramIR* ir) const override;
};

class NumberAST : public BaseAST {
public:
	int number;

	void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class BTypeAST : public BaseAST {
public:
    std::string type;
    void generate_ir(ProgramIR* ir) const override;
};

class DeclAST : public BaseAST {};

class ConstDeclAST : public DeclAST {
public:
    std::string b_type; // "int"
    std::vector<std::unique_ptr<ConstDefAST>> def_list;

    void generate_ir(ProgramIR* ir) const override;
};

class ConstDefAST : public BaseAST {
public:
    std::string id;
    std::unique_ptr<ConstInitValAST> init_val;

    void generate_ir(ProgramIR* ir) const override;
};

class ConstInitValAST : public BaseAST {
public:
    std::unique_ptr<ConstExpAST> const_exp;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class ConstExpAST : public BaseAST {
public:
    std::unique_ptr<ExpAST> exp;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class LValAST : public BaseAST {
public:
    std::string id;

    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

class VarDeclAST : public DeclAST {
public:
    std::string b_type;
    std::vector<std::unique_ptr<VarDefAST>> var_def_list;
    void generate_ir(ProgramIR* ir) const override;
};

class VarDefAST : public BaseAST {
public:
    std::string id;
    std::unique_ptr<InitValAST> init_val;
    void generate_ir(ProgramIR* ir) const override;
};

class InitValAST : public BaseAST {
public:
    std::unique_ptr<ExpAST> exp;
    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

// =========================================================
// 方法实现
// =========================================================

// generate_ir
inline void CompUnitAST::generate_ir(ProgramIR* ir) const {
    func_def->generate_ir(ir);
}

inline void FuncDefAST::generate_ir(ProgramIR* ir) const {
    auto func_ir = std::make_unique<FunctionIR>();
    func_ir->func_name = func_name;

    if(func_type->type == "int")
        func_ir->func_type = "i32";
    else
        func_ir->func_type = func_type->type;
    
    ir->cur_func = func_ir.get();
    ir->funcs.push_back(std::move(func_ir));

    auto entry_block = std::make_unique<BasicBlockIR>();
    entry_block->basic_block_name = "entry";
    ir->cur_block = entry_block.get();
    ir->cur_func->basic_blocks.push_back(std::move(entry_block));

    block->generate_ir(ir);

    bool need_ret = true;
    if (!ir->cur_block->insts.empty()) {
        BaseIR* last_inst = ir->cur_block->insts.back().get();
        if (dynamic_cast<ReturnIR*>(last_inst)) {
            need_ret = false;
        }
    }

    if (need_ret) {
        auto ret_ir = std::make_unique<ReturnIR>();
        if (func_type->type == "int") {
            // int 函数缺省返回 0
            ret_ir->ret_value = Operand(Operand::IMM, 0);
        } else {
            // 缺省返回 void
            ret_ir->ret_value = Operand();
        }
        ir->cur_block->insts.push_back(std::move(ret_ir));
    }
}

inline void FuncTypeAST::generate_ir(ProgramIR* ir) const {}

inline void BlockAST::generate_ir(ProgramIR* ir) const {
    symbol_table.enter_scope();
    for (const auto& item : body) {
        if (!ir->cur_block->insts.empty()) {
            BaseIR* last_inst = ir->cur_block->insts.back().get();
            if (dynamic_cast<ReturnIR*>(last_inst)) {
                // 如果上一条是 ret，说明当前块已结束
                auto dead_block = std::make_unique<BasicBlockIR>();
                dead_block->basic_block_name = "dead_" + std::to_string(ir->cur_inst_id++);
                // 挂载到函数并更新 cur_block
                ir->cur_block = dead_block.get();
                ir->cur_func->basic_blocks.push_back(std::move(dead_block));
            }
        }
        item->generate_ir(ir);
    }
    symbol_table.exit_scope();
}

inline void ExpStmtAST::generate_ir(ProgramIR* ir) const {
    if (exp) {
        exp->generate_ir(ir);
    }
}

inline void ReturnStmtAST::generate_ir(ProgramIR* ir) const {
    if (exp) {
        // 有返回值
        exp->generate_ir(ir);
        auto ret_ir = std::make_unique<ReturnIR>();
        ret_ir->ret_value = ir->cur_val;
        ir->cur_block->insts.push_back(std::move(ret_ir));
    } else {
        // 无返回值
        auto ret_ir = std::make_unique<ReturnIR>();
        ret_ir->ret_value = Operand();
        ir->cur_block->insts.push_back(std::move(ret_ir));
    }
}
// inline void ReturnStmtAST::generate_ir(ProgramIR* ir) const {
//     exp_ast->generate_ir(ir);
//     auto ret_ir = std::make_unique<ReturnIR>();
//     ret_ir->ret_value = ir->cur_val;
//     ir->cur_block->insts.push_back(std::move(ret_ir));
// }

inline void ExpAST::generate_ir(ProgramIR* ir) const {
	lor_exp->generate_ir(ir);
}

inline void AddExpAST::generate_ir(ProgramIR* ir) const {
    if (mul_exp) {
        mul_exp->generate_ir(ir);
    } else {
        left->generate_ir(ir);
        Operand left_val = ir->cur_val;

        right->generate_ir(ir);
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
    if (unary_exp) {
        unary_exp->generate_ir(ir);
    } else {
        left->generate_ir(ir);
        Operand left_val = ir->cur_val;

        right->generate_ir(ir);
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
    if (land_exp) {
        land_exp->generate_ir(ir);
    } else {
        left->generate_ir(ir);
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

        right->generate_ir(ir);
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
    if (eq_exp) {
        eq_exp->generate_ir(ir);
    } else {
        left->generate_ir(ir);
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


        right->generate_ir(ir);
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
    if (add_exp) {
        add_exp->generate_ir(ir);
    } else {
        left->generate_ir(ir);
        Operand left_val = ir->cur_val;
        right->generate_ir(ir);
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
    if (rel_exp) {
        rel_exp->generate_ir(ir);
    } else {
        left->generate_ir(ir);
        Operand left_val = ir->cur_val;
        right->generate_ir(ir);
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
	if (primary_exp) {
		primary_exp->generate_ir(ir);
	} else if (unary_op && unary_exp) {
		unary_exp->generate_ir(ir);	// 先求出被取负的值/id
		unary_op->generate_ir(ir);
	}
}

inline void PrimaryExpAST::generate_ir(ProgramIR* ir) const {
    if (exp) {
        exp->generate_ir(ir);
    } else if (number) {
        number->generate_ir(ir);
    } else if (lval) {
        lval->generate_ir(ir);
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

inline void BTypeAST::generate_ir(ProgramIR* ir) const {}

inline void ConstDeclAST::generate_ir(ProgramIR* ir) const {
    for (const auto& def : def_list) {
        def->generate_ir(ir);
    }
}

inline void ConstDefAST::generate_ir(ProgramIR* ir) const {
    // 获取编译期常量值
    int val = init_val->calculate_val();
    // 插入符号表
    symbol_table.push(id, true, val);
}

inline void ConstInitValAST::generate_ir(ProgramIR* ir) const {}

inline void ConstExpAST::generate_ir(ProgramIR* ir) const {}

inline void LValAST::generate_ir(ProgramIR* ir) const {
    SymbolInfo* info = symbol_table.lookup(id);
    if (info) {
        if (info->is_const) {
            // 常量直接替换数值
            ir->cur_val = Operand(Operand::IMM, info->const_val);
        } else {
            // 变量需要 load: %new_id = load @x
            int target_id = ir->cur_inst_id++;
            auto load_inst = std::make_unique<LoadIR>(target_id, Operand(info->var_name));
            ir->cur_block->insts.push_back(std::move(load_inst));
            // 当前表达式的值就是 load 出来的那个临时寄存器
            ir->cur_val = Operand(Operand::ID, target_id);
        }
    } else {
        std::cerr << "Error: Undefined identifier " << id << std::endl;
        exit(1);
    }
}

inline void VarDeclAST::generate_ir(ProgramIR* ir) const {
    for (const auto& def : var_def_list) {
        def->generate_ir(ir);
    }
}

inline void VarDefAST::generate_ir(ProgramIR* ir) const {
    std::string unique_name = symbol_table.push(id, false, 0);
    // 生成 alloc 指令
    auto alloc_inst = std::make_unique<AllocIR>(unique_name);
    ir->cur_block->insts.push_back(std::move(alloc_inst));
    // 如果有初始值，生成 store
    if (init_val) {
        init_val->generate_ir(ir);
        Operand rhs_val = ir->cur_val;
        // store %val, @x
        auto store_inst = std::make_unique<StoreIR>(rhs_val, Operand(unique_name));
        ir->cur_block->insts.push_back(std::move(store_inst));
    }
}

inline void InitValAST::generate_ir(ProgramIR* ir) const {
    exp->generate_ir(ir);
}

inline std::unique_ptr<ProgramIR> generate_ir(const std::unique_ptr<BaseAST>& ast) {
    auto ir = std::make_unique<ProgramIR>();
    ast->generate_ir(ir.get());
    return ir;
}

inline void AssignStmtAST::generate_ir(ProgramIR* ir) const {
    // 计算表达式
    exp->generate_ir(ir);
    Operand rhs = ir->cur_val;
    // 查表
    SymbolInfo* info = symbol_table.lookup(lval->id);
    if (info) {
        if (info->is_const) {
            std::cerr << "Error: Assign to const " << lval->id << std::endl;
            exit(1);
        }
        // store %val, @x
        auto store_inst = std::make_unique<StoreIR>(rhs, Operand(info->var_name));
        ir->cur_block->insts.push_back(std::move(store_inst));
    } else {
        std::cerr << "Error: Undefined variable " << lval->id << std::endl;
        exit(1);
    }
}

// calculate_val

inline int NumberAST::calculate_val() const {
    return number;
}

inline int PrimaryExpAST::calculate_val() const {
    if (number) return number->calculate_val();
    if (exp) return exp->calculate_val();
    if (lval) return lval->calculate_val();
    return 0; 
}

inline int LValAST::calculate_val() const {
    SymbolInfo* info = symbol_table.lookup(id);
    if (info) {
        if (info->is_const) {
            return info->const_val;
        } else {
            std::cerr << "Semantic Error: Variable '" << id << "' used in constant expression." << std::endl;
            exit(1); 
        }
    } else {
        std::cerr << "Semantic Error: Undefined identifier: " << id << std::endl;
        exit(1);
    }
}

inline int UnaryExpAST::calculate_val() const {
    if (primary_exp) return primary_exp->calculate_val();
    if (unary_op && unary_exp) {
        int val = unary_exp->calculate_val();
        std::string op = unary_op->op;
        if (op == "+") 
            return val;
        if (op == "-") 
            return -val;
        if (op == "!") 
            return !val;
    }
    return 0;
}

inline int MulExpAST::calculate_val() const {
    if (unary_exp) return unary_exp->calculate_val();
    int l = left->calculate_val();
    int r = right->calculate_val();
    if (op == "*") 
        return l * r;
    if (op == "/") {
        if (r == 0) {
            std::cerr << "Semantic Error: Div by zero at compile time." << std::endl;
            exit(1);
        }
        return l / r;
    }
    if (op == "%") {
        if (r == 0) {
            std::cerr << "Semantic Error: Mod by zero at compile time." << std::endl;
            exit(1);
        }
        return l % r;
    }
    return 0;
}

inline int AddExpAST::calculate_val() const {
    if (mul_exp) return mul_exp->calculate_val();
    int l = left->calculate_val();
    int r = right->calculate_val();
    if (op == "+") return l + r;
    if (op == "-") return l - r;
    return 0;
}

inline int RelExpAST::calculate_val() const {
    if (add_exp) return add_exp->calculate_val();
    int l = left->calculate_val();
    int r = right->calculate_val();
    if (op == "<") return l < r;
    if (op == ">") return l > r;
    if (op == "<=") return l <= r;
    if (op == ">=") return l >= r;
    return 0;
}

inline int EqExpAST::calculate_val() const {
    if (rel_exp) return rel_exp->calculate_val();
    int l = left->calculate_val();
    int r = right->calculate_val();
    if (op == "==") return l == r;
    if (op == "!=") return l != r;
    return 0;
}

inline int LAndExpAST::calculate_val() const {
    if (eq_exp) return eq_exp->calculate_val();
    return left->calculate_val() && right->calculate_val();
}

inline int LOrExpAST::calculate_val() const {
    if (land_exp) return land_exp->calculate_val();
    return left->calculate_val() || right->calculate_val();
}

inline int ExpAST::calculate_val() const {
    return lor_exp->calculate_val();
}

inline int ConstExpAST::calculate_val() const {
    return exp->calculate_val();
}

inline int ConstInitValAST::calculate_val() const {
    return const_exp->calculate_val();
}

inline int InitValAST::calculate_val() const {
    return exp->calculate_val();
}