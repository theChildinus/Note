//
// Created by yingzi on 2017/4/29.
// lex.l和yacc.y共同使用的头文件
//

#ifndef LEX_YACC_EX_MAIN_H
#define LEX_YACC_EX_MAIN_H

#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

/*
 * 1. lex识别出一个记号后，是通过变量yylval向yacc传递数据的。默认情况下是int类型
 * 2. yylval是用YYSTYPE宏定义的，通过重定义YYSTYPE宏来重新指定yylval的类型
 */

struct Type {
    // 结构体内成员每次只会用一个，一般会定义成union节省空间，但这里用了string复杂类型造成不可以
    string m_sId;   // 保存标识符等类型的字符串
    int m_nInt;     // 保存整数类型的数
    char m_cOp;     // 保存单个字符类型
};

#define YYSTYPE Type    // 把YYSTYPE(即yylval变量)重定义为struct Type类型

#endif //LEX_YACC_EX_MAIN_H