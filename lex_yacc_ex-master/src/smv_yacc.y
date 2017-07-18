%{
#include "node.h"
%}
%token MODULE
%token TRUE FALSE
%token INTEGER BOOLEAN
%token VAR IVAR FROZENVAR DEFINE ASSIGN TRANS INIT NEXT CASE ESAC

%token INIT_OP NE_OP RANGE_OP
%token <m_string>IDENTIFIER
%token <m_int>NUMBERS

%start module

%%
module
	: MODULE IDENTIFIER '[' '(' module_parameters ')' ']' '[' module_body ']'
	{
		cout << "Module-type: " << $2 << endl; 
	}
	;

module_parameters
	: IDENTIFIER
	{
		cout << "Module-para: " << $1 << endl;
	}
	| module_parameters ',' IDENTIFIER
	;

module_body
	: module_element
	| module_body module_element
	;
	
module_element
	: define_declaration  //关联符号和表达式
	| var_declaration  //状态变量声明
	| ivar_declaration //输入变量用于标记有限状态机的转换
	| frozenvar_declaration //冻结变量是在整个状态机演化过程中保持其初始值的变量;
	| assign_constraint //分配约束
	| trans_constraint  //状态转移
	;

define_declaration
	: DEFINE define_body

define_body
	: IDENTIFIER INIT_OP simple_expr ';'
	| define_body IDENTIFIER INIT_OP simple_expr ';'
	;
	
var_declaration 
	: VAR var_list
	;
	
ivar_declaration 
	: IVAR simple_var_list
	;

frozenvar_declaration
	: FROZENVAR simple_var_list
	;

assign_constraint
	: ASSIGN assign_list
	;

trans_constraint
	: TRANS next_expr
	
var_list
	: IDENTIFIER ':' type_specifier ';'
	{
		cout << "Identifier: " << $1 << endl;
	}
	| var_list IDENTIFIER ':' type_specifier ';'
	;	
	
simple_var_list
	: IDENTIFIER ':' type_specifier ';'
	{
		cout << "Identifier: " << $1 << endl;
	}
	| simple_var_list IDENTIFIER ':' type_specifier ';'
	;

assign_list
	: assign ';'
	| assign_list assign ';'
	;

assign
	: complex_identifier INIT_OP simple_expr
	| INIT '(' complex_identifier ')' INIT_OP simple_expr
	| NEXT '(' complex_identifier ')' INIT_OP next_expr
	;
	
simple_expr
	: basic_expr
	;
	
next_expr
	: basic_expr
	;
	
case_expr
	: CASE case_body ESAC
	;

case_body
	: basic_expr ':' basic_expr ';'
	| case_body basic_expr ':' basic_expr ';'
	;

complex_identifier
	: IDENTIFIER
	| complex_identifier '.' IDENTIFIER
	| complex_identifier '[' simple_expr ']'
	;
	
type_specifier
	: BOOLEAN { cout << "match Boolean" << endl; }
	| INTEGER { cout << "match Integer" << endl; }
	;
	
basic_expr
	: constant
	| case_expr
	| basic_expr operator basic_expr
	;

operator
	: '='
	| NE_OP
	| '<'
	| '>'
	| '+'
	| RANGE_OP
	;
	
constant
	: symoblic_constant
	| boolean_constant
	| integer_constant
	;
	
symoblic_constant
	: IDENTIFIER { cout << "Identifier: " << $1 << endl; }
	;	
	
boolean_constant
	: TRUE { cout << "match Ture" << endl; }
	| FALSE { cout << "match False" << endl; }
	;

integer_constant
	: NUMBERS {cout << "Number:" << $1 << endl;}
	;

%%
void yyerror(const char* s) {
    cerr << s << endl;
}

// 程序主函数，这个函数也可以放到其他.c, .cpp文件中
int main() {
    const char* sFile = "../examples/bmc_tutorial.smv";
    FILE* fp = fopen(sFile, "r");
    if (fp == NULL) {
        printf("cannot open %s\n", sFile);
        return -1;
    }
    extern FILE* yyin;  // yyin和yyout都是FILE*类型
    yyin = fp;  // yacc会从yyin读取输入，yyin默认是标准输入，这里改为磁盘文件。yacc默认向yyout输出，可修改yyout改变输出目的

    printf("-----begin parsing %s\n", sFile);
    yyparse();  // 使yacc开始读取输入和解析，它会调用lex的yylex()读取记号
    puts("-----end parsing");

    fclose(fp);

    return 0;
}