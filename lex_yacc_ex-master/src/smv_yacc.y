%{
#include "node.h"
%}

%token TRUE FALSE
%token INTEGER BOOLEAN
%token VAR
%token <m_string>IDENTIFIER
%token <m_int>NUMBERS

%start file

%%
file
	: basic_expr_list
	| var_declaration
	| file var_declaration
	;

var_declaration
	: VAR var_list
	;

var_list
	: IDENTIFIER ':' type_specifier ';'
	{
		cout << "Identifier: " << $1 << endl;
	}
	| var_list IDENTIFIER ':' type_specifier ';'
	;

type_specifier
	: BOOLEAN { cout << "match BOOLEAN" << endl; }
	| INTEGER { cout << "match INTEGER" << endl; }
	;

basic_expr_list
	: basic_expr
	| basic_expr_list ',' basic_expr
	;
	
basic_expr
	: constant
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
	: TRUE { cout << "match ture" << endl; }
	| FALSE { cout << "match false" << endl; }
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