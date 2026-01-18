#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include "ir.hpp"
#include "symtab.hpp"
#include <functional>

// =========================================================
// 前向声明
// =========================================================

extern SymbolTable symbol_table;
extern std::map<std::string, std::string> global_func_type_table;

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

class AssignStmtAST;
class ReturnStmtAST;
class ExpStmtAST;
class IfStmtAST;
class WhileStmtAST;
class BreakStmtAST;
class ContinueStmtAST;

class FuncFParamAST;


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
	std::vector<std::unique_ptr<BaseAST>> func_defs;
	void generate_ir(ProgramIR* ir) const override;
};

class FuncFParamAST : public BaseAST {
public:
    std::string b_type;
    std::string name;
    std::vector<std::unique_ptr<BaseAST>> dims;
    void generate_ir(ProgramIR* ir) const override; // 不需要单独生成，在 FuncDef 中处理
};

class FuncDefAST : public BaseAST {
public:
	std::unique_ptr<FuncTypeAST> func_type;
	std::string func_name;
    std::vector<std::unique_ptr<FuncFParamAST>> params;
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

class IfStmtAST : public BaseAST {
public:
    std::unique_ptr<ExpAST> cond;
    std::unique_ptr<BaseAST> then_stmt;
    std::unique_ptr<BaseAST> else_stmt; // 如果没有 else，则为 nullptr
    void generate_ir(ProgramIR* ir) const override;
};

class WhileStmtAST : public BaseAST {
public:
    std::unique_ptr<ExpAST> cond;
    std::unique_ptr<BaseAST> stmt;
    void generate_ir(ProgramIR* ir) const override;
};

class BreakStmtAST : public BaseAST {
public:
    void generate_ir(ProgramIR* ir) const override;
};

class ContinueStmtAST : public BaseAST {
public:
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
    std::string func_name;
    std::vector<std::unique_ptr<ExpAST>> call_args;

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
    std::vector<std::unique_ptr<ConstExpAST>> dims;
    void generate_ir(ProgramIR* ir) const override;
};

class ConstInitValAST : public BaseAST {
public:
    std::unique_ptr<ConstExpAST> const_exp;
    std::vector<std::unique_ptr<ConstInitValAST>> values;
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
    std::vector<std::unique_ptr<ExpAST>> indices;
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
    std::vector<std::unique_ptr<ConstExpAST>> dims;
    void generate_ir(ProgramIR* ir) const override;
};

class InitValAST : public BaseAST {
public:
    std::unique_ptr<ExpAST> exp;
    std::vector<std::unique_ptr<InitValAST>> values;
    void generate_ir(ProgramIR* ir) const override;
    int calculate_val() const override;
};

// =========================================================
// 方法实现
// =========================================================

// generate_ir
inline void CompUnitAST::generate_ir(ProgramIR* ir) const {
    for (const auto& def : func_defs) {
        def->generate_ir(ir);
    }
}

inline void FuncFParamAST::generate_ir(ProgramIR* ir) const {}

inline void FuncDefAST::generate_ir(ProgramIR* ir) const {
    auto func_ir = std::make_unique<FunctionIR>();
    func_ir->func_name = func_name;
    func_ir->func_type = (func_type->type == "int") ? "i32" : "void";

    for (const auto& param : params) {
        func_ir->params.push_back(param->name);
        if (param->dims.empty()) {
            func_ir->param_is_array.push_back(false);
        } else {
            func_ir->param_is_array.push_back(true);
        }
    }
    global_func_type_table[func_name] = func_ir->func_type;

    ir->cur_func = func_ir.get();
    ir->funcs.push_back(std::move(func_ir));

    auto entry_block = std::make_unique<BasicBlockIR>();
    entry_block->basic_block_name = "entry";
    ir->cur_block = entry_block.get();
    ir->cur_func->basic_blocks.push_back(std::move(entry_block));

    symbol_table.enter_scope();

    for (size_t i = 0; i < params.size(); ++i) {
        std::vector<int> param_dims;
        for (size_t k = 1; k < params[i]->dims.size(); ++k) {
             if (auto cp = dynamic_cast<ConstExpAST*>(params[i]->dims[k].get()))
                param_dims.push_back(cp->calculate_val());
        }

        bool is_ptr = !params[i]->dims.empty();
        std::string unique_name = symbol_table.push(params[i]->name, false, 0, is_ptr, is_ptr, param_dims);
        
        auto alloc_inst = std::make_unique<AllocIR>(unique_name, 1, is_ptr);
        ir->cur_block->insts.push_back(std::move(alloc_inst));
        
        Operand arg_val = Operand::make_arg(i, params[i]->name);
        auto store_inst = std::make_unique<StoreIR>(arg_val, Operand(unique_name));
        ir->cur_block->insts.push_back(std::move(store_inst));
    }

    block->generate_ir(ir); 
    symbol_table.exit_scope();

    // 处理缺省 return
    bool need_ret = true;
    if (!ir->cur_block->insts.empty()) {
        BaseIR* last_inst = ir->cur_block->insts.back().get();
        if (dynamic_cast<ReturnIR*>(last_inst)) need_ret = false;
    }
    if (need_ret) {
        auto ret_ir = std::make_unique<ReturnIR>();
        if (func_type->type == "int") 
            ret_ir->ret_value = Operand(Operand::IMM, 0);
        else ret_ir->ret_value = Operand(); // void
        ir->cur_block->insts.push_back(std::move(ret_ir));
    }
    ir->cur_func = nullptr;
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
        // 短路求值逻辑: left || right
        // 1. 分配一个临时变量保存结果，初始化为 1 (true)
        //    假设 left 为真，直接跳到 end，结果就是 1
        std::string res_var = "or_res_" + std::to_string(ir->cur_inst_id++);
        auto alloc = std::make_unique<AllocIR>(res_var);
        ir->cur_block->insts.push_back(std::move(alloc));
        
        auto store_init = std::make_unique<StoreIR>(Operand(Operand::IMM, 1), Operand(res_var));
        ir->cur_block->insts.push_back(std::move(store_init));

        // 2. 计算左操作数
        left->generate_ir(ir);
        Operand left_val = ir->cur_val;

        // 3. 准备标号
        std::string false_label = "or_false_" + std::to_string(ir->cur_inst_id++);
        std::string end_label = "or_end_" + std::to_string(ir->cur_inst_id++);

        // 4. 判断 left 是否为真
        int cmp_id = ir->cur_inst_id++;
        auto cmp_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::NE, cmp_id, left_val, Operand(Operand::IMM, 0));
        ir->cur_block->insts.push_back(std::move(cmp_inst));

        // 5. 分支：如果 left 为真，跳到 end (短路)；否则跳到 false_label (计算 right)
        auto br_inst = std::make_unique<BranchIR>(Operand(Operand::ID, cmp_id), end_label, false_label);
        ir->cur_block->insts.push_back(std::move(br_inst));

        // 6. False Block: 计算 right
        auto false_block = std::make_unique<BasicBlockIR>();
        false_block->basic_block_name = false_label;
        ir->cur_block = false_block.get();
        ir->cur_func->basic_blocks.push_back(std::move(false_block));

        right->generate_ir(ir);
        Operand right_val = ir->cur_val;

        // 计算 right != 0 并存入结果
        int cmp_r_id = ir->cur_inst_id++;
        auto cmp_r_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::NE, cmp_r_id, right_val, Operand(Operand::IMM, 0));
        ir->cur_block->insts.push_back(std::move(cmp_r_inst));

        auto store_r = std::make_unique<StoreIR>(Operand(Operand::ID, cmp_r_id), Operand(res_var));
        ir->cur_block->insts.push_back(std::move(store_r));

        // 跳到 end
        auto jump_inst = std::make_unique<JumpIR>(end_label);
        ir->cur_block->insts.push_back(std::move(jump_inst));

        // 7. End Block: 读取结果
        auto end_block = std::make_unique<BasicBlockIR>();
        end_block->basic_block_name = end_label;
        ir->cur_block = end_block.get();
        ir->cur_func->basic_blocks.push_back(std::move(end_block));

        int final_reg = ir->cur_inst_id++;
        auto load_inst = std::make_unique<LoadIR>(final_reg, Operand(res_var));
        ir->cur_block->insts.push_back(std::move(load_inst));

        ir->cur_val = Operand(Operand::ID, final_reg);
    }
}

inline void LAndExpAST::generate_ir(ProgramIR* ir) const {
    if (eq_exp) {
        eq_exp->generate_ir(ir);
    } else {
        // 短路求值逻辑: left && right
        // 分配结果变量，初始化为 0 (false)
        std::string res_var = "and_res_" + std::to_string(ir->cur_inst_id++);
        auto alloc = std::make_unique<AllocIR>(res_var);
        ir->cur_block->insts.push_back(std::move(alloc));

        auto store_init = std::make_unique<StoreIR>(Operand(Operand::IMM, 0), Operand(res_var));
        ir->cur_block->insts.push_back(std::move(store_init));

        // 计算左操作数
        left->generate_ir(ir);
        Operand left_val = ir->cur_val;

        std::string true_label = "and_true_" + std::to_string(ir->cur_inst_id++);
        std::string end_label = "and_end_" + std::to_string(ir->cur_inst_id++);

        // 判断 left 是否为真
        int cmp_id = ir->cur_inst_id++;
        auto cmp_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::NE, cmp_id, left_val, Operand(Operand::IMM, 0));
        ir->cur_block->insts.push_back(std::move(cmp_inst));

        // 分支：如果 left 为真，跳到 true_label (计算 right)；否则跳到 end (短路)
        auto br_inst = std::make_unique<BranchIR>(Operand(Operand::ID, cmp_id), true_label, end_label);
        ir->cur_block->insts.push_back(std::move(br_inst));

        // True Block: 计算 right
        auto true_block = std::make_unique<BasicBlockIR>();
        true_block->basic_block_name = true_label;
        ir->cur_block = true_block.get();
        ir->cur_func->basic_blocks.push_back(std::move(true_block));

        right->generate_ir(ir);
        Operand right_val = ir->cur_val;

        // 计算 right != 0 并存入结果
        int cmp_r_id = ir->cur_inst_id++;
        auto cmp_r_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::NE, cmp_r_id, right_val, Operand(Operand::IMM, 0));
        ir->cur_block->insts.push_back(std::move(cmp_r_inst));

        auto store_r = std::make_unique<StoreIR>(Operand(Operand::ID, cmp_r_id), Operand(res_var));
        ir->cur_block->insts.push_back(std::move(store_r));

        auto jump_inst = std::make_unique<JumpIR>(end_label);
        ir->cur_block->insts.push_back(std::move(jump_inst));

        // End Block
        auto end_block = std::make_unique<BasicBlockIR>();
        end_block->basic_block_name = end_label;
        ir->cur_block = end_block.get();
        ir->cur_func->basic_blocks.push_back(std::move(end_block));

        int final_reg = ir->cur_inst_id++;
        auto load_inst = std::make_unique<LoadIR>(final_reg, Operand(res_var));
        ir->cur_block->insts.push_back(std::move(load_inst));

        ir->cur_val = Operand(Operand::ID, final_reg);
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
    if (!func_name.empty()) { // 函数调用
        std::vector<Operand> args;
        for (const auto& arg_ast : call_args) {
            arg_ast->generate_ir(ir);
            args.push_back(ir->cur_val);
        }

        int target_id = -1;
        if (global_func_type_table.find(func_name) != global_func_type_table.end()) {
            if (global_func_type_table[func_name] != "void") {
                target_id = ir->cur_inst_id++;
            }
        } else {
            // 外部函数，假设返回值为 int
            target_id = ir->cur_inst_id++;
        }
        auto call_inst = std::make_unique<CallIR>(func_name, args, target_id);
        ir->cur_block->insts.push_back(std::move(call_inst));
            
        if (target_id != -1)
            ir->cur_val = Operand(Operand::ID, target_id);
        else
            ir->cur_val = Operand();
    }
    else if (primary_exp) {
		primary_exp->generate_ir(ir);
	} 
    else if (unary_op && unary_exp) {
		unary_exp->generate_ir(ir);
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
    std::vector<int> dim_vals;
    for (const auto& d : dims) {
        dim_vals.push_back(d->calculate_val());
    }
    int total_len = 1;
    std::vector<int> strides; // strides[i] 表示第 i 层一个元素包含多少个整数
    for (int val : dim_vals) total_len *= val;

    if (!dim_vals.empty()) {
        strides.resize(dim_vals.size());
        int current_stride = 1;
        for (int i = dim_vals.size() - 1; i >= 0; --i) {
            strides[i] = current_stride;
            current_stride *= dim_vals[i];
        }
    }

    std::vector<int> init_vals;

    std::function<void(ConstInitValAST*, int)> process;
    process = [&](ConstInitValAST* node, int depth) {
        if (node->const_exp) {
            init_vals.push_back(node->const_exp->calculate_val());
        } else {
            int start_pos = init_vals.size();
            int current_container_size = (depth == 0) ? total_len : strides[depth - 1];
            
            for (const auto& child : node->values) {
                if (!child->const_exp) {
                    int target_depth = depth;
                    for (int d = depth; d < strides.size(); ++d) {
                        int current_offset = init_vals.size() - start_pos;
                        if (current_offset % strides[d] == 0) {
                            target_depth = d;
                            break;
                        }
                    }
                    int stride = (target_depth < strides.size()) ? strides[target_depth] : 1;
                    int current_offset = init_vals.size() - start_pos;
                    while (current_offset % stride != 0) {
                        init_vals.push_back(0);
                        current_offset++;
                    }
                    
                    process(child.get(), target_depth + 1);
                }
                else{
                    process(child.get(), depth + 1);
                }
            }
            while (init_vals.size() - start_pos < current_container_size) {
                init_vals.push_back(0);
            }
        }
    };

    if (init_val) {
        process(init_val.get(), 0);
        while (init_vals.size() < total_len) init_vals.push_back(0);
        if (init_vals.size() > total_len) {
            init_vals.resize(total_len);
        }
    } else {
        init_vals.resize(total_len, 0);
    }

    std::string unique_name = symbol_table.push(id, true, 0, !dims.empty(), false, dim_vals);

    if (ir->cur_func == nullptr) {
        auto global_ir = std::make_unique<GlobalAllocIR>(unique_name, total_len, init_vals);
        ir->globals.push_back(std::move(global_ir));

        if (dims.empty()) {
             SymbolInfo* info = symbol_table.lookup(id);
             if (info) info->const_val = init_vals[0];
        }
    } else {
        // 局部常量
        if (!dims.empty()) {
            auto alloc_inst = std::make_unique<AllocIR>(unique_name, total_len);
            ir->cur_block->insts.push_back(std::move(alloc_inst));
            for (int i = 0; i < total_len; ++i) {
                if (init_vals[i] != 0) {
                    int addr_reg = ir->cur_inst_id++;
                    auto gep_inst = std::make_unique<GetElementPtrIR>(
                        addr_reg, Operand(unique_name), Operand(Operand::IMM, i)
                    );
                    ir->cur_block->insts.push_back(std::move(gep_inst));
                    auto store_inst = std::make_unique<StoreIR>(
                        Operand(Operand::IMM, init_vals[i]), 
                        Operand(Operand::ID, addr_reg)
                    );
                    ir->cur_block->insts.push_back(std::move(store_inst));
                } else {
                    int addr_reg = ir->cur_inst_id++;
                    auto gep_inst = std::make_unique<GetElementPtrIR>(
                        addr_reg, Operand(unique_name), Operand(Operand::IMM, i)
                    );
                    ir->cur_block->insts.push_back(std::move(gep_inst));
                    auto store_inst = std::make_unique<StoreIR>(
                        Operand(Operand::IMM, 0), 
                        Operand(Operand::ID, addr_reg)
                    );
                    ir->cur_block->insts.push_back(std::move(store_inst));
                }
            }
        } else {
            symbol_table.push(id, true, init_vals[0], false, false, {});
        }
    }
}

inline void ConstInitValAST::generate_ir(ProgramIR* ir) const {}

inline void ConstExpAST::generate_ir(ProgramIR* ir) const {}

inline void LValAST::generate_ir(ProgramIR* ir) const {
    SymbolInfo* info = symbol_table.lookup(id);
    if (!info) {
        std::cerr << "Error: Undefined identifier " << id << std::endl;
        exit(1);
    }

    if (info->is_const && !info->is_array) {
        ir->cur_val = Operand(Operand::IMM, info->const_val);
        return;
    }

    Operand base_op;
    if (info->is_pointer) {
        int ptr_val_id = ir->cur_inst_id++;
        auto load_ptr = std::make_unique<LoadIR>(ptr_val_id, Operand(info->var_name));
        ir->cur_block->insts.push_back(std::move(load_ptr));
        base_op = Operand(Operand::ID, ptr_val_id);
    } else {
        base_op = Operand(info->var_name);
    }

    if (indices.empty()) {
        if (info->is_array && !info->is_pointer) {
            int addr_reg = ir->cur_inst_id++;
            auto gep_inst = std::make_unique<GetElementPtrIR>(
                addr_reg, base_op, Operand(Operand::IMM, 0)
            );
            ir->cur_block->insts.push_back(std::move(gep_inst));
            ir->cur_val = Operand(Operand::ID, addr_reg);
        } else {
            int target_id = ir->cur_inst_id++;
            auto load_inst = std::make_unique<LoadIR>(target_id, Operand(info->var_name));
            ir->cur_block->insts.push_back(std::move(load_inst));
            ir->cur_val = Operand(Operand::ID, target_id);
        }
        return;
    }

    Operand final_offset = Operand(Operand::IMM, 0);
    for (size_t i = 0; i < indices.size(); ++i) {
        indices[i]->generate_ir(ir);
        Operand idx_val = ir->cur_val;

        int stride = 1;
        size_t start_dim = info->is_pointer ? i : i + 1;
        for (size_t k = start_dim; k < info->dims.size(); ++k) {
            stride *= info->dims[k];
        }

        int mul_res = ir->cur_inst_id++;
        auto mul_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::MUL, mul_res, idx_val, Operand(Operand::IMM, stride)
        );
        ir->cur_block->insts.push_back(std::move(mul_inst));

        int add_res = ir->cur_inst_id++;
        auto add_inst = std::make_unique<BinaryArithmeticIR>(
            BinaryOpType::ADD, add_res, final_offset, Operand(Operand::ID, mul_res)
        );
        ir->cur_block->insts.push_back(std::move(add_inst));
        final_offset = Operand(Operand::ID, add_res);
    }

    int addr_reg = ir->cur_inst_id++;
    if (info->is_pointer) {
        auto ptr_inst = std::make_unique<GetPtrIR>(
            addr_reg, base_op, final_offset
        );
        ir->cur_block->insts.push_back(std::move(ptr_inst));
    } else {
        auto gep_inst = std::make_unique<GetElementPtrIR>(
            addr_reg, base_op, final_offset
        );
        ir->cur_block->insts.push_back(std::move(gep_inst));
    }

    size_t needed_dims = info->dims.size();
    if (info->is_pointer) needed_dims += 1;
    if (indices.size() == needed_dims) {
        int val_reg = ir->cur_inst_id++;
        auto load_inst = std::make_unique<LoadIR>(val_reg, Operand(Operand::ID, addr_reg));
        ir->cur_block->insts.push_back(std::move(load_inst));
        ir->cur_val = Operand(Operand::ID, val_reg);
    } else {
        ir->cur_val = Operand(Operand::ID, addr_reg);
    }
}

inline void VarDeclAST::generate_ir(ProgramIR* ir) const {
    for (const auto& def : var_def_list) {
        def->generate_ir(ir);
    }
}

inline void VarDefAST::generate_ir(ProgramIR* ir) const {
    std::vector<int> dim_vals;
    for (const auto& d : dims) dim_vals.push_back(d->calculate_val());
    int total_len = 1;
    for (int val : dim_vals) total_len *= val;

    std::vector<int> strides;
    if (!dim_vals.empty()) {
        strides.resize(dim_vals.size());
        int current_stride = 1;
        for (int i = dim_vals.size() - 1; i >= 0; --i) {
            strides[i] = current_stride;
            current_stride *= dim_vals[i];
        }
    }

    if (ir->cur_func == nullptr) {
        std::vector<int> init_vals;
        if (init_val) {
            std::function<void(InitValAST*, int)> process;
            process = [&](InitValAST* node, int depth) {
                if (node->exp) {
                    init_vals.push_back(node->exp->calculate_val());
                } else {
                    int start_pos = init_vals.size();
                    int current_container_size = (depth == 0) ? total_len : strides[depth - 1];

                    for (const auto& child : node->values) {
                        if (!child->exp) { 
                            int target_depth = depth;
                            for (int d = depth; d < strides.size(); ++d) {
                                int current_offset = init_vals.size() - start_pos;
                                if (current_offset % strides[d] == 0) {
                                    target_depth = d;
                                    break;
                                }
                            }
                            
                            int stride = (target_depth < strides.size()) ? strides[target_depth] : 1;
                            int current_offset = init_vals.size() - start_pos;
                            while (current_offset % stride != 0) {
                                init_vals.push_back(0);
                                current_offset++;
                            }

                            process(child.get(), target_depth + 1);
                        } else {
                            process(child.get(), depth + 1);
                        }
                    }
                    while (init_vals.size() - start_pos < current_container_size) {
                        init_vals.push_back(0);
                    }
                }
            };
            process(init_val.get(), 0);
        }
        while (init_vals.size() < total_len) init_vals.push_back(0);
        if (init_vals.size() > total_len) {
            init_vals.resize(total_len);
        }

        std::string unique_name = symbol_table.push(id, false, 0, !dims.empty(), false, dim_vals);
        auto global_ir = std::make_unique<GlobalAllocIR>(unique_name, total_len, init_vals);
        ir->globals.push_back(std::move(global_ir));

    } else {
        std::string unique_name = symbol_table.push(id, false, 0, !dims.empty(), false, dim_vals);
        auto alloc_inst = std::make_unique<AllocIR>(unique_name, total_len);
        ir->cur_block->insts.push_back(std::move(alloc_inst));

        if (init_val) {
            if (!dims.empty()) {
                std::vector<ExpAST*> flat_exps;
                std::function<void(InitValAST*, int)> process_exp;
                process_exp = [&](InitValAST* node, int depth) {
                    if (node->exp) {
                        flat_exps.push_back(node->exp.get());
                    } else {
                        int start_pos = flat_exps.size();
                        int current_container_size = (depth == 0) ? total_len : strides[depth - 1];

                        for (const auto& child : node->values) {
                            if (!child->exp) { 
                                int target_depth = depth;
                                for (int d = depth; d < strides.size(); ++d) {
                                    int current_offset = flat_exps.size() - start_pos;
                                    if (current_offset % strides[d] == 0) {
                                        target_depth = d;
                                        break;
                                    }
                                }

                                int stride = (target_depth < strides.size()) ? strides[target_depth] : 1;
                                int current_offset = flat_exps.size() - start_pos;
                                while (current_offset % stride != 0) {
                                    flat_exps.push_back(nullptr);
                                    current_offset++;
                                }
                                process_exp(child.get(), target_depth + 1);
                            } else {
                                process_exp(child.get(), depth + 1);
                            }
                        }
                        while (flat_exps.size() - start_pos < current_container_size) {
                            flat_exps.push_back(nullptr); 
                        }
                    }
                };
                process_exp(init_val.get(), 0);
                
                while (flat_exps.size() < total_len) flat_exps.push_back(nullptr);

                for (int i = 0; i < total_len; ++i) {
                    Operand rhs_val;
                    if (flat_exps[i]) {
                        flat_exps[i]->generate_ir(ir);
                        rhs_val = ir->cur_val;
                    } else {
                        rhs_val = Operand(Operand::IMM, 0);
                    }

                    int addr_reg = ir->cur_inst_id++;
                    auto gep_inst = std::make_unique<GetElementPtrIR>(
                        addr_reg, Operand(unique_name), Operand(Operand::IMM, i)
                    );
                    ir->cur_block->insts.push_back(std::move(gep_inst));

                    auto store_inst = std::make_unique<StoreIR>(rhs_val, Operand(Operand::ID, addr_reg));
                    ir->cur_block->insts.push_back(std::move(store_inst));
                }
            } else {
                init_val->exp->generate_ir(ir);
                auto store_inst = std::make_unique<StoreIR>(ir->cur_val, Operand(unique_name));
                ir->cur_block->insts.push_back(std::move(store_inst));
            }
        }
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
    exp->generate_ir(ir);
    Operand rhs = ir->cur_val;

    SymbolInfo* info = symbol_table.lookup(lval->id);
    if (!info) {
        std::cerr << "Error: Undefined variable " << lval->id << std::endl;
        exit(1);
    }
    if (info->is_const) {
        std::cerr << "Error: Assign to const " << lval->id << std::endl;
        exit(1);
    }

    Operand target_addr;
    if (lval->indices.empty()) {
        target_addr = Operand(info->var_name);
    } else {
        Operand base_op;
        if (info->is_pointer) {
            int ptr_val_id = ir->cur_inst_id++;
            auto load_ptr = std::make_unique<LoadIR>(ptr_val_id, Operand(info->var_name));
            ir->cur_block->insts.push_back(std::move(load_ptr));
            base_op = Operand(Operand::ID, ptr_val_id);
        } else {
            base_op = Operand(info->var_name);
        }

        Operand final_offset = Operand(Operand::IMM, 0);
        for (size_t i = 0; i < lval->indices.size(); ++i) {
            lval->indices[i]->generate_ir(ir);
            Operand idx_val = ir->cur_val;

            int stride = 1;
            size_t start_dim = info->is_pointer ? i : i + 1;
            for (size_t k = start_dim; k < info->dims.size(); ++k) {
                stride *= info->dims[k];
            }

            int mul_res = ir->cur_inst_id++;
            auto mul_inst = std::make_unique<BinaryArithmeticIR>(
                BinaryOpType::MUL, mul_res, idx_val, Operand(Operand::IMM, stride)
            );
            ir->cur_block->insts.push_back(std::move(mul_inst));

            int add_res = ir->cur_inst_id++;
            auto add_inst = std::make_unique<BinaryArithmeticIR>(
                BinaryOpType::ADD, add_res, final_offset, Operand(Operand::ID, mul_res)
            );
            ir->cur_block->insts.push_back(std::move(add_inst));
            final_offset = Operand(Operand::ID, add_res);
        }

        int addr_reg = ir->cur_inst_id++;
        if (info->is_pointer) {
            auto ptr_inst = std::make_unique<GetPtrIR>(
                addr_reg, base_op, final_offset
            );
            ir->cur_block->insts.push_back(std::move(ptr_inst));
        } else {
            auto gep_inst = std::make_unique<GetElementPtrIR>(
                addr_reg, base_op, final_offset
            );
            ir->cur_block->insts.push_back(std::move(gep_inst));
        }
        target_addr = Operand(Operand::ID, addr_reg);
    }

    auto store_inst = std::make_unique<StoreIR>(rhs, target_addr);
    ir->cur_block->insts.push_back(std::move(store_inst));
}

inline void IfStmtAST::generate_ir(ProgramIR* ir) const {
    if (cond) {
        cond->generate_ir(ir);
    }
    Operand cond_val = ir->cur_val;

    int cmp_id = ir->cur_inst_id++;
    auto cmp_inst = std::make_unique<BinaryArithmeticIR>(
        BinaryOpType::NE, 
        cmp_id, 
        cond_val, 
        Operand(Operand::IMM, 0)
    );
    ir->cur_block->insts.push_back(std::move(cmp_inst));
    Operand is_true = Operand(Operand::ID, cmp_id);

    std::string then_label = "then_" + std::to_string(ir->cur_inst_id++);
    std::string else_label = "else_" + std::to_string(ir->cur_inst_id++);
    std::string end_label = "end_" + std::to_string(ir->cur_inst_id++);

    std::string true_target = then_label;
    std::string false_target = else_stmt ? else_label : end_label;

    auto br_inst = std::make_unique<BranchIR>(is_true, true_target, false_target);
    ir->cur_block->insts.push_back(std::move(br_inst));

    auto then_block = std::make_unique<BasicBlockIR>();
    then_block->basic_block_name = then_label;
    ir->cur_block = then_block.get();
    ir->funcs.back()->basic_blocks.push_back(std::move(then_block));

    if (then_stmt) {
        then_stmt->generate_ir(ir);
    }

    bool then_has_term = false;
    if (!ir->cur_block->insts.empty()) {
        auto* last = ir->cur_block->insts.back().get();
        if (dynamic_cast<ReturnIR*>(last) || 
            dynamic_cast<BranchIR*>(last) || 
            dynamic_cast<JumpIR*>(last)) {
            then_has_term = true;
        }
    }
    if (!then_has_term) {
        auto jump = std::make_unique<JumpIR>(end_label);
        ir->cur_block->insts.push_back(std::move(jump));
    }

    if (else_stmt) {
        auto else_block = std::make_unique<BasicBlockIR>();
        else_block->basic_block_name = else_label;
        ir->cur_block = else_block.get();
        ir->funcs.back()->basic_blocks.push_back(std::move(else_block));

        else_stmt->generate_ir(ir);

        bool else_has_term = false;
        if (!ir->cur_block->insts.empty()) {
            auto* last = ir->cur_block->insts.back().get();
            if (dynamic_cast<ReturnIR*>(last) || 
                dynamic_cast<BranchIR*>(last) || 
                dynamic_cast<JumpIR*>(last)) {
                else_has_term = true;
            }
        }
        if (!else_has_term) {
            auto jump = std::make_unique<JumpIR>(end_label);
            ir->cur_block->insts.push_back(std::move(jump));
        }
    }

    auto end_block = std::make_unique<BasicBlockIR>();
    end_block->basic_block_name = end_label;
    ir->cur_block = end_block.get();
    ir->funcs.back()->basic_blocks.push_back(std::move(end_block));
}

inline void WhileStmtAST::generate_ir(ProgramIR* ir) const {
    // 生成标号
    std::string label_entry = "while_entry_" + std::to_string(ir->cur_inst_id++);
    std::string label_body = "while_body_" + std::to_string(ir->cur_inst_id++);
    std::string label_end = "while_end_" + std::to_string(ir->cur_inst_id++);

    // 将 entry 和 end 压入栈中
    ir->loop_entry_stack.push_back(label_entry);
    ir->loop_end_stack.push_back(label_end);

    // 当前块跳转到 entry
    auto jump_inst = std::make_unique<JumpIR>(label_entry);
    ir->cur_block->insts.push_back(std::move(jump_inst));

    // Entry 块：计算条件并分支
    auto entry_block = std::make_unique<BasicBlockIR>();
    entry_block->basic_block_name = label_entry;
    ir->cur_block = entry_block.get();
    ir->funcs.back()->basic_blocks.push_back(std::move(entry_block));

    cond->generate_ir(ir);
    Operand cond_val = ir->cur_val;

    int cmp_id = ir->cur_inst_id++;
    auto cmp_inst = std::make_unique<BinaryArithmeticIR>(
        BinaryOpType::NE, cmp_id, cond_val, Operand(Operand::IMM, 0)
    );
    ir->cur_block->insts.push_back(std::move(cmp_inst));
    Operand is_true = Operand(Operand::ID, cmp_id);

    // 如果真 -> body，假 -> end
    auto br_inst = std::make_unique<BranchIR>(is_true, label_body, label_end);
    ir->cur_block->insts.push_back(std::move(br_inst));

    // Body 块：执行语句
    auto body_block = std::make_unique<BasicBlockIR>();
    body_block->basic_block_name = label_body;
    ir->cur_block = body_block.get();
    ir->funcs.back()->basic_blocks.push_back(std::move(body_block));

    stmt->generate_ir(ir);

    // Body 结束后跳回 entry（如果没有被 return/break/continue 终止）
    bool has_term = false;
    if (!ir->cur_block->insts.empty()) {
        auto* last = ir->cur_block->insts.back().get();
        if (dynamic_cast<ReturnIR*>(last) || 
            dynamic_cast<BranchIR*>(last) || 
            dynamic_cast<JumpIR*>(last)) {
            has_term = true;
        }
    }
    if (!has_term) {
        auto jump_back = std::make_unique<JumpIR>(label_entry);
        ir->cur_block->insts.push_back(std::move(jump_back));
    }

    // End 块
    auto end_block = std::make_unique<BasicBlockIR>();
    end_block->basic_block_name = label_end;
    ir->cur_block = end_block.get();
    ir->funcs.back()->basic_blocks.push_back(std::move(end_block));

    // 弹出栈
    ir->loop_entry_stack.pop_back();
    ir->loop_end_stack.pop_back();
}

inline void BreakStmtAST::generate_ir(ProgramIR* ir) const {
    if (ir->loop_end_stack.empty()) {
        std::cerr << "Semantic Error: 'break' outside of loop." << std::endl;
        exit(1);
    }
    // 跳转到当前循环的 end 标号
    std::string target = ir->loop_end_stack.back();
    auto jump = std::make_unique<JumpIR>(target);
    ir->cur_block->insts.push_back(std::move(jump));

    // 后续生成死代码块，防止后续指令写入已终结的块
    auto dead_block = std::make_unique<BasicBlockIR>();
    dead_block->basic_block_name = "dead_" + std::to_string(ir->cur_inst_id++);
    ir->cur_block = dead_block.get();
    ir->funcs.back()->basic_blocks.push_back(std::move(dead_block));
}

inline void ContinueStmtAST::generate_ir(ProgramIR* ir) const {
    if (ir->loop_entry_stack.empty()) {
        std::cerr << "Semantic Error: 'continue' outside of loop." << std::endl;
        exit(1);
    }
    // 跳转到当前循环的 entry 标号
    std::string target = ir->loop_entry_stack.back();
    auto jump = std::make_unique<JumpIR>(target);
    ir->cur_block->insts.push_back(std::move(jump));

    // 后续生成死代码块
    auto dead_block = std::make_unique<BasicBlockIR>();
    dead_block->basic_block_name = "dead_" + std::to_string(ir->cur_inst_id++);
    ir->cur_block = dead_block.get();
    ir->funcs.back()->basic_blocks.push_back(std::move(dead_block));
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