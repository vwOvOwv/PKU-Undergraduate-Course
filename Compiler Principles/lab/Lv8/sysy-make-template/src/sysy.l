/* 描述 EBNF 终结符, 由 Flex 读取并用于生成词法分析器 */

%option noyywrap
%option nounput
%option noinput

%x IN_BLOCK_COMMENT

%{

#include <cstdlib>
#include <string>

// 因为 Flex 会用到 Bison 中关于 token 的定义
// 所以需要 include Bison 生成的头文件
#include "sysy.tab.hpp"

using namespace std;

%}

/* 空白符和注释 */
WhiteSpace    [ \t\n\r]*
LineComment   "//".*

/* 标识符 */
Identifier    [a-zA-Z_][a-zA-Z0-9_]*

/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+

%%

{WhiteSpace}    { /* 忽略, 不做任何操作 */ }
{LineComment}   { /* 忽略, 不做任何操作 */ }

"/*"        { BEGIN(IN_BLOCK_COMMENT); }  /* 进入注释状态 */
<IN_BLOCK_COMMENT>"*/" { BEGIN(INITIAL); }      /* 回到正常状态 */
<IN_BLOCK_COMMENT>\n   { /* 忽略换行 */ }
<IN_BLOCK_COMMENT>.    { /* 忽略注释里的其他所有字符 */ }

"int"           { return INT; }
"void"          { return VOID; }
"return"        { return RETURN; }
"const"         { return CONST; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"break"         { return BREAK; }
"continue"      { return CONTINUE; }

    /* yytext 代表 lexer 当前匹配到的字符串的内容, 它的类型是 char *
     * yylval 用来向 parser 传递 lexer 读取到的内容, str_val 和之后的 int_val 是我们在 Bison 文件中定义的字段
     */

"<="            { return LE; }
">="            { return GE; }
"=="            { return EQ; }
"!="            { return NE; }
"&&"            { return AND; }
"||"            { return OR; }

{Identifier}    { yylval.str_val = new string(yytext); return IDENT; }

{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }

    /* . 这个正则表达式会匹配任意单个字符, 而 yytext[0] 实际上读取了目前匹配到的这个字 */
.               { return yytext[0]; }

%%
