// 描述 EBNF 本身，由 Bison 读取并用于生成语法分析器

%code requires {
  #include <memory>
  #include <string>
  #include "ast.hpp"
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "ast.hpp"

// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<BaseAST> &ast }

// yylval 的定义, 我们把它定义成了一个联合体 (union)
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val 和 int_val 就是在这里被定义的
// 至于为什么要用字符串指针而不直接用 string 或者 unique_ptr<string>? 
// 请自行 STFW 在 union 里写一个带析构函数的类会出现什么情况（答：易造成内存泄漏）
%union {
  std::string *str_val;
  int int_val;
  // 记录每个 yylval 所属的 AST 节点
  BaseAST *ast_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token INT RETURN 
%token <str_val> IDENT
%token <int_val> INT_CONST
%token EQ NE LE GE AND OR

// 非终结符的类型定义
%type <ast_val> FuncDef FuncType Block Stmt Exp UnaryExp PrimaryExp UnaryOp MulExp AddExp LOrExp LAndExp EqExp RelExp
%type <int_val> Number

%%

// 开始符, CompUnit ::= FuncDef, 大括号后声明了解析完成后 parser 要做的事情
// 之前我们定义了 FuncDef 会返回一个 str_val, 也就是字符串指针
// 而 parser 一旦解析完 CompUnit, 就说明所有的 token 都被解析了, 即解析结束了
// 此时我们应该把 FuncDef 返回的结果收集起来, 作为 AST 传给调用 parser 的函数
// $1 指代规则里第一个符号的返回值, 也就是 FuncDef 的返回值
CompUnit
  : FuncDef {
	auto comp_unit_ast = make_unique<CompUnitAST>();
	comp_unit_ast->func_def_ast = std::unique_ptr<FuncDefAST>(static_cast<FuncDefAST*>($1));
	ast = std::move(comp_unit_ast);
  }
  ;

// FuncDef ::= FuncType IDENT '(' ')' Block;
// 我们这里可以直接写 '(' 和 ')', 因为之前在 lexer 里已经处理了单个字符的情况
// 解析完成后, 把这些符号的结果收集起来, 然后拼成一个新的字符串, 作为结果返回
// $$ 表示非终结符的返回值, 我们可以通过给这个符号赋值的方法来返回结果
// 你可能会问, FuncType, IDENT 之类的结果已经是字符串指针了
// 为什么还要用 unique_ptr 接住它们, 然后再解引用, 把它们拼成另一个字符串指针呢
// 因为所有的字符串指针都是我们 new 出来的, new 出来的内存一定要 delete
// 否则会发生内存泄漏, 而 unique_ptr 这种智能指针可以自动帮我们 delete
// 虽然此处你看不出用 unique_ptr 和手动 delete 的区别, 但当我们定义了 AST 之后
// 这种写法会省下很多内存管理的负担
FuncDef
  : FuncType IDENT '(' ')' Block {
	// $1      $2   $3 $4   $5
	auto ast = new FuncDefAST();
	ast->func_type_ast = unique_ptr<FuncTypeAST>(static_cast<FuncTypeAST*>($1));
	ast->func_name = *unique_ptr<string>($2);
	ast->block_ast = unique_ptr<BlockAST>(static_cast<BlockAST*>($5));
	$$ = ast;
  }
  ;

// 同上, 不再解释
FuncType
  : INT {
	auto ast = new FuncTypeAST();
	ast->type = "int";
	$$ = ast;
  }
  ;

Block
  : '{' Stmt '}' {
	auto ast = new BlockAST();
	ast->stmt_ast = unique_ptr<StmtAST>(static_cast<StmtAST*>($2));
	$$ = ast;
  }
  ;

Stmt
  : RETURN Exp ';' {
	auto ast = new StmtAST();
	ast->exp_ast = unique_ptr<ExpAST>(static_cast<ExpAST*>($2));
	$$ = ast;
  }
  ;

Exp
  : LOrExp {
    auto ast = new ExpAST();
    ast->lor_exp_ast = unique_ptr<LOrExpAST>(static_cast<LOrExpAST*>($1));
    $$ = ast;
  }
  ;

UnaryExp
  : PrimaryExp {
	auto ast = new UnaryExpAST();
	ast->primary_exp_ast = unique_ptr<PrimaryExpAST>(static_cast<PrimaryExpAST*>($1));
	$$ = ast;
  }
  | UnaryOp UnaryExp {
	auto ast = new UnaryExpAST();
	ast->unary_op_ast = unique_ptr<UnaryOpAST>(static_cast<UnaryOpAST*>($1));
	ast->unary_exp_ast = unique_ptr<UnaryExpAST>(static_cast<UnaryExpAST*>($2));
	$$ = ast;
  }
  ;

PrimaryExp
  : '(' Exp ')' {
	auto ast = new PrimaryExpAST();
	ast->exp_ast = unique_ptr<ExpAST>(static_cast<ExpAST*>($2));
	$$ = ast;
  }
  | Number {
	auto ast = new PrimaryExpAST();
	ast->number_ast = make_unique<NumberAST>();
	ast->number_ast->number = $1;
	$$ = ast;
  }
  ;

UnaryOp
  : '+' {
	auto ast = new UnaryOpAST();
	ast->op = "+";
	$$ = ast;
  }
  | '-' {
	auto ast = new UnaryOpAST();
	ast->op = "-";
	$$ = ast;
  }
  | '!' {
	auto ast = new UnaryOpAST();
	ast->op = "!";
	$$ = ast;
  }
  ;

Number
  : INT_CONST {
	$$ = $1;
  }
  ;

MulExp
  : UnaryExp {
    auto ast = new MulExpAST();
    ast->unary_exp_ast = unique_ptr<UnaryExpAST>(static_cast<UnaryExpAST*>($1));
    $$ = ast;
  }
  | MulExp '*' UnaryExp {
    auto ast = new MulExpAST();
    ast->left_ast = unique_ptr<MulExpAST>(static_cast<MulExpAST*>($1));
    ast->op = "*"; // 直接赋值，不要去读 $2
    ast->right_ast = unique_ptr<UnaryExpAST>(static_cast<UnaryExpAST*>($3)); // 注意这里是 $3
    $$ = ast;
  }
  | MulExp '/' UnaryExp {
    auto ast = new MulExpAST();
    ast->left_ast = unique_ptr<MulExpAST>(static_cast<MulExpAST*>($1));
    ast->op = "/";
    ast->right_ast = unique_ptr<UnaryExpAST>(static_cast<UnaryExpAST*>($3));
    $$ = ast;
  }
  | MulExp '%' UnaryExp {
    auto ast = new MulExpAST();
    ast->left_ast = unique_ptr<MulExpAST>(static_cast<MulExpAST*>($1));
    ast->op = "%";
    ast->right_ast = unique_ptr<UnaryExpAST>(static_cast<UnaryExpAST*>($3));
    $$ = ast;
  }
  ;

AddExp
  : MulExp {
    auto ast = new AddExpAST();
    ast->mul_exp_ast = unique_ptr<MulExpAST>(static_cast<MulExpAST*>($1));
    $$ = ast;
  }
  | AddExp '+' MulExp {
    auto ast = new AddExpAST();
    ast->left_ast = unique_ptr<AddExpAST>(static_cast<AddExpAST*>($1));
    ast->op = "+";
    ast->right_ast = unique_ptr<MulExpAST>(static_cast<MulExpAST*>($3));
    $$ = ast;
  }
  | AddExp '-' MulExp {
    auto ast = new AddExpAST();
    ast->left_ast = unique_ptr<AddExpAST>(static_cast<AddExpAST*>($1));
    ast->op = "-";
    ast->right_ast = unique_ptr<MulExpAST>(static_cast<MulExpAST*>($3));
    $$ = ast;
  }
  ;

LOrExp
  : LAndExp {
    auto ast = new LOrExpAST();
    ast->land_exp_ast = unique_ptr<LAndExpAST>(static_cast<LAndExpAST*>($1));
    $$ = ast;
  }
  | LOrExp OR LAndExp {
    auto ast = new LOrExpAST();
    ast->left_ast = unique_ptr<LOrExpAST>(static_cast<LOrExpAST*>($1));
    ast->op = "||";
    ast->right_ast = unique_ptr<LAndExpAST>(static_cast<LAndExpAST*>($3));
    $$ = ast;
  }
  ;

LAndExp
  : EqExp {
    auto ast = new LAndExpAST();
    ast->eq_exp_ast = unique_ptr<EqExpAST>(static_cast<EqExpAST*>($1));
    $$ = ast;
  }
  | LAndExp AND EqExp {
    auto ast = new LAndExpAST();
    ast->left_ast = unique_ptr<LAndExpAST>(static_cast<LAndExpAST*>($1));
    ast->op = "&&";
    ast->right_ast = unique_ptr<EqExpAST>(static_cast<EqExpAST*>($3));
    $$ = ast;
  }
  ;

EqExp
  : RelExp {
    auto ast = new EqExpAST();
    ast->rel_exp_ast = unique_ptr<RelExpAST>(static_cast<RelExpAST*>($1));
    $$ = ast;
  }
  | EqExp EQ RelExp {
    auto ast = new EqExpAST();
    ast->left_ast = unique_ptr<EqExpAST>(static_cast<EqExpAST*>($1));
    ast->op = "==";
    ast->right_ast = unique_ptr<RelExpAST>(static_cast<RelExpAST*>($3));
    $$ = ast;
  }
  | EqExp NE RelExp {
    auto ast = new EqExpAST();
    ast->left_ast = unique_ptr<EqExpAST>(static_cast<EqExpAST*>($1));
    ast->op = "!=";
    ast->right_ast = unique_ptr<RelExpAST>(static_cast<RelExpAST*>($3));
    $$ = ast;
  }
  ;

RelExp
  : AddExp {
    auto ast = new RelExpAST();
    ast->add_exp_ast = unique_ptr<AddExpAST>(static_cast<AddExpAST*>($1));
    $$ = ast;
  }
  | RelExp '<' AddExp {
    auto ast = new RelExpAST();
    ast->left_ast = unique_ptr<RelExpAST>(static_cast<RelExpAST*>($1));
    ast->op = "<";
    ast->right_ast = unique_ptr<AddExpAST>(static_cast<AddExpAST*>($3));
    $$ = ast;
  }
  | RelExp '>' AddExp {
    auto ast = new RelExpAST();
    ast->left_ast = unique_ptr<RelExpAST>(static_cast<RelExpAST*>($1));
    ast->op = ">";
    ast->right_ast = unique_ptr<AddExpAST>(static_cast<AddExpAST*>($3));
    $$ = ast;
  }
  | RelExp LE AddExp {
    auto ast = new RelExpAST();
    ast->left_ast = unique_ptr<RelExpAST>(static_cast<RelExpAST*>($1));
    ast->op = "<=";
    ast->right_ast = unique_ptr<AddExpAST>(static_cast<AddExpAST*>($3));
    $$ = ast;
  }
  | RelExp GE AddExp {
    auto ast = new RelExpAST();
    ast->left_ast = unique_ptr<RelExpAST>(static_cast<RelExpAST*>($1));
    ast->op = ">=";
    ast->right_ast = unique_ptr<AddExpAST>(static_cast<AddExpAST*>($3));
    $$ = ast;
  }
  ;

%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}
