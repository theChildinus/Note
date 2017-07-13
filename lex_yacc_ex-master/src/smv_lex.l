
%{
#include "node.h"
#include "yacc.tab.h"

%}

%x COMMENT

DIGIT   [0-9]
LETTER  [a-zA-Z_]

%%

"--" { BEGIN COMMENT; }

"integer" { return INTEGER; }
"boolean" { return BOOLEAN; }

"TRUE" { return TRUE; }
"FALSE" { return FALSE; }


{DIGIT}+ { yylval.m_int = stoi(yytext); return NUMBER; }
{LETTER}({LETTER}|{DIGIT})* { yylval.m_string = yytext; return IDENTIFIER; }

[ \f\r\t\v\n]+ { /* 忽略 */ }
. { /* 忽略 */ }

<COMMENT>\n {   // 匹配注释状态下的换行
    BEGIN INITIAL;
}

<COMMENT>. {    // 匹配注释状态下的其他字符
    // 忽略
}

%%

int yywrap(void) {
    cout << "---the file is end";
    return 1;
}