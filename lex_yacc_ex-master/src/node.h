//
// Created by yingzi on 2017/5/1.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_NODE_H
#define RUNTIME_VERIFICATION_SYSTEM_NODE_H

#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

// 树结点类型
struct Node {
    enum type {
        Boolean,
        Char,
        Int,
        Double,
        String
    };
    bool m_bool;
    char m_char;
    int m_int;
    double m_double;
    string m_string;
};

#define YYSTYPE Node

extern "C" {
    int yywrap(void);
    extern int yylex(void);    // lex生成的词法分析函数,yacc的yyparse()里会调用
    void yyerror(const char* s);
}

#endif //RUNTIME_VERIFICATION_SYSTEM_NODE_H

