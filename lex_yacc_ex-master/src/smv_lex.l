
%{
#include "node.h"
#include "yacc.tab.h"

%}

%x COMMENT

DIGIT   [0-9]
LETTER  [a-zA-Z_]

%%
"--" { BEGIN COMMENT; }

"MODULE"  { return MODULE; }

"integer" { return INTEGER; }
"boolean" { return BOOLEAN; }

"VAR"       { return VAR; }
"IVAR"      { return IVAR; }
"FROZENVAR" { return FROZENVAR; }

"DEFINE" { return DEFINE; }
"ASSIGN" { return ASSIGN; }
"TRANS"  { return TRANS; }
"init"   { return INIT; }
"next"   { return NEXT; }
"case"   { return CASE; }
"esac"   { return ESAC; }

"TRUE"  { return TRUE; }
"FALSE" { return FALSE; }
","  { return (','); }
"."  { return ('.'); }
":"  { return (':'); }
";"  { return (';'); }
":=" { return INIT_OP; }
"["  { return ('['); }
"]"  { return (']'); }
"("  { return ('('); }
")"  { return (')'); }

"="  { return ('='); }
"!=" { return NE_OP; }
"<"  { return ('<'); }
">"  { return ('>'); }
"+"  { return ('+'); }
".." { return RANGE_OP; }

{DIGIT}+ { yylval.m_int = stoi(yytext); return NUMBERS; }
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
    cout << "---the file is end" << endl;
    return 1;
}