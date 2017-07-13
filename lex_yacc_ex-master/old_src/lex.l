/* lex程序文件
 * 本lex的生成文件是lex.yy.c
 * 1. 声明段，包括
 *    1.1 C代码部分：头文件、函数、类型等声明，会原样拷贝到生成的.c文件中
 *    1.2 状态声明，如%x COMMENT
 *    1.3 正则式定义，如digit([0-9])
 * 2. 规则段，是lex文件的主体，包括每个规则是如何匹配的，以及匹配后要执行的C代码动作
 * 3. C函数定义段，如yywrap()的定义，这些C代码会原样拷贝到生成的.c文件中。该段可以为空
 */


%{
// 1. 声明段

#include "main.h"   // lex和yacc共用的头文件，重定义了YYSTYPE
#include "yacc.tab.h"

// 声明外部的C语言函数，可以放置到main.h中
extern "C" {
    int yywrap(void);
    int yylex(void);    // lex生成的词法分析函数,yacc的yyparse()里会调用
}
%}

/* 状态声明，lex启动时是INITIAL状态，需要切换状态才能区别不同状态下的记号 */
%x COMMENT

/* 非数字 */
nondigit    ([_A-Za-z])

/* 单个数字 */
digit   ([0-9])

/* 整数 */
integer ({digit}+)

/* 标识符 */
identifier  ({nondigit}({nondigit}|{digit})*)

/* 空白符 */
blank_chars ([ \f\r\t\v]+)


/* 2. 规则段 */

%%

{identifier} {  // 匹配标识符串，此时串值由yytext保存
    yylval.m_sId = yytext;  // 通过赋值到yylval里实现向yacc传递记号值，在yacc里就可以用$n的方式来引用了
    return IDENTIFIER;  // 向yacc返回：识别出的记号类型为IDENTIFIER
}

{integer} { // 匹配整数
    yylval.m_nInt = atoi(yytext);
    return INTEGER;
}

{blank_chars} { // 匹配空白符
    // 忽略即可
}

\n {    // 匹配换行
    // 忽略即可
}

"//" {  // 匹配注释符
    cout << "(comment)" << endl;    // 提示遇到了注释
    BEGIN COMMENT;  // 用BEGIN切换到注释状态，去过滤这段注释，下一次lex将只匹配前面带有<COMMENT>的正则式
}

. { // 匹配.
    // .表示除\n以外的其他任意字符，这个规则要放在最后（以其它<>开头的规则除外）
    yylval.m_cOp = yytext[0];   // 只匹配一个字符，对应yytext[0]
    return OPERATOR;
}

<COMMENT>\n  {  // 匹配注释状态下的规则
    BEGIN INITIAL;  // 在注释状态下，当遇到换行符时，表明注释结束了，返回初始态
}

<COMMENT>.  {   // 匹配注释状下的其他字符
    // 注释状态下其他字符都忽略了
}

%%


/* 3. C函数定义段 */

int yywrap(void) {
    puts("-----the file is end");
    return 1;   // 返回1表示读取全部结束。如果要接着读其他文件，可以这里fopen该文件，文件指针赋给yyin，并返回0
}
