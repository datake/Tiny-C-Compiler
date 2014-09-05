%option noyywrap
%option yylineno
%{
#include "kadai8.h"
#include "kadai8.tab.h"


#include "kadai8_semantic_analyser.h"
#include "kadai8_print.h"
#include "kadai8_code_generation.h"

/*int yylval;*/
%}

%%


[0-9]+ {
/*定数を表す正規表現*/
yylval.i=atoi(yytext); /*yylvalに整数を代入*/
return CONSTANT;}
"+" {return ADD;}
"*" {return MULTI;}
"-" {return  SUB;}
"/" {return DIV ;}
"||" {return  OR;}
"&&" {return AND ;}
"else" {return ELSE;}
"if" {return IF;}
"int" {return INT;}
"opt" {return OPT;}

"return" {return RETURN;}
"while" {return WHILE;}
"=" {return ASSIGN;}
"==" {return EQUAL_TO;}
"!=" {return NOT_EQUAL_TO;}
">" {return GREATER_THAN;}
"<" {return LESS_THAN;}
">=" {return GREATER_THAN_EQUAL;}
"<=" {return LESS_THAN_EQUAL;}
"(" {return LP;}
")" {return RP;}
"," {return COMMA;}
";" {return SEMICOLON;}
"}" {return RIGHT_BRACKET;}
"{" {return LEFT_BRACKET;}


[A-Za-z][A-Za-z_0-9]* { 
  if (!(yylval.n = lookup_sym(strdup(yytext))))
    yylval.n = make_token_node(strdup(yytext));
  return IDENTIFIER;
}
[ \t\n]; /*スペース，タブ，改行は無視*/

%%
