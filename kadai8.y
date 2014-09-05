%{
#include <stdio.h>

#include<string.h>
  //#include<stdag.h>
  //#include "struct.c"
  //同じフォルダのヘッダファイルは""で囲む
 #include "kadai8.h"


#include "kadai8_semantic_analyser.h"
#include "kadai8_print.h"
#include "kadai8_code_generation.h" 
//大域変数make.hに定義
  int yylex(void);
  tree temp_fun,temp_ref;
 
  token symtab;//資料どおりsymtabはtoken型

  %}
%error_verbose
%union{
  int i;
  char *str;
  tree n;

 }


%token  ADD SUB MULTI DIV OR AND   EQUAL_TO NOT_EQUAL_TO GREATER_THAN LESS_THAN GREATER_THAN_EQUAL LESS_THAN_EQUAL LP RP SEMICOLON COMMA RIGHT_BRACKET LEFT_BRACKET OPT ASSIGN COMMMA INT RETURN WHILE ELSE IF CONS DECLARATION  FUNDEF1  STATEMENT STATEMENT_EXPRESSION STATEMENT_IF STATEMENT_IF_ELSE STATEMENT_WHILE STATEMENT_RETURN   COMP_STATE  ASSIGN_EXPR POSTFIX POSTFIX2 DECLARATOR  UNARY
%token <i> CONSTANT

/*終端記号Identifierはchar*型*/
%token <str>  TOKEN
%token <n> IDENTIFIER
 /*tokenは末端の記号*/

 /*
   yylval.i = atoi(yytext);
   yylval.str = strdup(yytext);
 */
   /*非終端記号はtree型*/
%type<n> program external_declaration declaration declarator declarator_list function_definition parameter_type_list parameter_type_list_opt parameter_declaration statement compound_statement  declaration_list  statement_list opt_declaration_list  opt_statement_list expression  assign_expr logical_OR_expr logical_AND_expr equality_expr relational_expr postfix_expr unary_expr primary_expr argument_expression_list argument_expression_list_opt  add_expr mult_expr
 /*
   yylval.i = atoi(yytext);
   yylval.str = strdup(yytrext);*/
%%

main:
{symtab = NULL; cur_lev = 0;parm_num=0;}
program
{ if (yynerrs==0 && semnerrs==0)
    
    { // print_program($2);
      emit_program($2);
      print_code();
    
    }
}
;

//$$は１からはじまる。0からではない
//1
program:

external_declaration {$$=$1;/*printf("1program\n");*/}
|program external_declaration {$$=make_tuple(CONS,$1,$2,NULL,NULL);/*printf("1program\n");*/}
;

//2
external_declaration:

declaration  {/*printf("2external_declaration\n");*/$$=$1;}
|function_definition {last_alloc=4;top_alloc=4;$$=$1;/*printf("2external_declaration\n");*/} 
;
//3
declaration:
INT declarator_list  SEMICOLON{/*printf("3declaration\n");*/$$=make_tuple(DECLARATION,$2,NULL,NULL,NULL);}//INTは考えない
;
//4
declarator_list:
declarator {/*printf("4declaration_list\n");printf("declaratorでmake_decl\n");*/$$=make_decl($1);}//$1はyylval.nの値
| declarator_list  COMMA declarator {/*printf("declaratorでmake_decl\n");*/$$=make_tuple(CONS,$1,make_decl($3),NULL,NULL);}//COMMAは考えない
;
//5
declarator:
IDENTIFIER {$$=$1;/*printf("5 declarator\n");*/}
;
//6
function_definition:
INT  declarator{temp_fun=make_fun_def($2);count_parm=0;/*printf("6function_fefinition/count_parm:%d",count_parm);*/}
LP {cur_lev++;offset=4;/*parm_alloc=4;*/} 
parameter_type_list_opt
RP{offset=0;temp_fun->tk.offset=count_parm;last_alloc=0;top_alloc=0;} 
compound_statement 
{$$=make_tuple(FUNDEF1,temp_fun,$6,$9,NULL);cur_lev--;symtab = lookup_num(cur_lev);/*pop_sym();*//*release_loc();*//*printf("6 function_definition\n");*/}//INTは考えない

;
//7-2
parameter_type_list_opt:
/*empty*/
{$$=NULL;}
|parameter_type_list{$$=$1;}
;


//7
parameter_type_list:
//複文compound-statementの変数宣言の解析を始めるときに1増やす
parameter_declaration  //curl--する時そのブロックで宣言されたオブジェクトはそのスコープを終えるので，それらのオブジェクト構造体をスタックからポップ(p.19)
{/*printf("7 parameter_type_list");*/$$=$1;count_parm++;/*parm_num++;parm_alloc=4;*//*printf("%d",count_parm);*/}
|  parameter_type_list  COMMA parameter_declaration {/*printf("7 parameter_type_list");*/$$=make_tuple(CONS,$1,$3,NULL,NULL);/*parm_num++;*/count_parm++;/*printf("%d",count_parm);*/}
;
//8
parameter_declaration:
INT  declarator{/*printf("8 parameter_declaration\n");*/$$=make_parm_decl($2);/*make_tuple(DECLARATOR,$2,NULL,NULL,NULL);*/}
//INTは考えない
;
//9
statement:
SEMICOLON{$$=NULL;}
|expression  SEMICOLON{/*printf("9 statement\n");*/$$=make_tuple(STATEMENT_EXPRESSION,$1,NULL,NULL,NULL);}
|compound_statement  {/*printf("9 statement\n");*/$$=$1;}
|IF LP expression RP statement  {/*printf("9 statement\n");*/$$=make_tuple(STATEMENT_IF,$3,$5,NULL,NULL);}
|IF LP expression RP statement ELSE statement  {/*printf("9 statement\n");*/$$=make_tuple(STATEMENT_IF_ELSE,$3,$5,$7,NULL);}
|WHILE LP expression RP statement  {/*printf("9 statement\n");*/$$=make_tuple(STATEMENT_WHILE,$3,$5,NULL,NULL);}
|RETURN expression SEMICOLON {/*printf("9 statement\n");*/$$=make_tuple(STATEMENT_RETURN,$2,NULL,NULL,NULL);}
;
//10
compound_statement:
LEFT_BRACKET  
{/*printf("10 compound_statement\n");*/cur_lev++;/*last_alloc=0;top_alloc=0;*/}//複文compound-statementの変数宣言の解析を始めるときに1増やす
opt_declaration_list 
opt_statement_list 
RIGHT_BRACKET
{$$=make_tuple(COMP_STATE,$3,$4,NULL,NULL);cur_lev--; symtab = lookup_num(cur_lev);/*pop_sym();*/offset=symtab->offset;}
;

opt_declaration_list :
declaration_list 
{$$=$1;}
|/*NULL*/
{$$=NULL;}
;
opt_statement_list :
statement_list 
{$$=$1;}
|/*NULL*/
{$$=NULL;}
;


//11
declaration_list:
declaration{$$=$1;}
| declaration_list declaration {$$=make_tuple(CONS,$1,$2,NULL,NULL);}
;
//12
statement_list:
statement{$$=$1;}
| statement_list statement {$$=make_tuple(CONS,$1,$2,NULL,NULL);}
;
//13
expression:
assign_expr {$$=$1;}
|expression COMMA assign_expr {$$=make_tuple(CONS,$1,$3,NULL,NULL);}
;
//14
assign_expr:
logical_OR_expr {$$=$1;}
|IDENTIFIER ASSIGN assign_expr{$$=make_tuple(ASSIGN,ref_var($1),$3,NULL,NULL);}
  //printf("IDENTIFIER=%s\n",$1);}
;
//15

logical_OR_expr:
logical_AND_expr {$$=$1;}
|logical_OR_expr OR logical_AND_expr{$$=make_tuple(OR,$1,$3,NULL,NULL);}
;
//16
logical_AND_expr:
equality_expr  {$$=$1;}
|logical_AND_expr AND equality_expr{$$=make_tuple(AND,$1,$3,NULL,NULL);}
;



//17

equality_expr:
relational_expr {$$=$1;}
|equality_expr EQUAL_TO relational_expr {$$=make_tuple(EQUAL_TO,$1,$3,NULL,NULL);}
|equality_expr NOT_EQUAL_TO relational_expr {$$=make_tuple(NOT_EQUAL_TO,$1,$3,NULL,NULL);}
;


//18


relational_expr:
add_expr  {$$=$1;}
|relational_expr GREATER_THAN add_expr  {$$=make_tuple(GREATER_THAN,$1,$3,NULL,NULL);}
|relational_expr LESS_THAN add_expr  {$$=make_tuple(LESS_THAN,$1,$3,NULL,NULL);}
|relational_expr GREATER_THAN_EQUAL add_expr  {$$=make_tuple(GREATER_THAN_EQUAL,$1,$3,NULL,NULL);}
|relational_expr LESS_THAN_EQUAL add_expr  {$$=make_tuple(LESS_THAN_EQUAL,$1,$3,NULL,NULL);}

;

//19

add_expr:
mult_expr  {$$=$1;}
|add_expr ADD mult_expr {$$=make_tuple(ADD,$1,$3,NULL,NULL);}
|add_expr SUB  mult_expr {$$=make_tuple(SUB,$1,$3,NULL,NULL);}


;
//20
mult_expr:
unary_expr {$$=$1;}
| mult_expr MULTI  unary_expr{$$=make_tuple(MULTI,$1,$3,NULL,NULL);}
| mult_expr DIV unary_expr {$$=make_tuple(DIV,$1,$3,NULL,NULL);}
;
//21
postfix_expr:
primary_expr{$$=$1;}
|IDENTIFIER {count_arg=count_parm;count_parm=0;/*parm_num=0;*//*printf("postfix_expr count_parm:%d",count_parm);*/}
LP argument_expression_list_opt RP{temp_ref=ref_fun($1);$$=make_tuple(POSTFIX,temp_ref,$4,NULL,NULL);/*check_parm_num($$);*//*変更*/check_parm_num(count_parm/*count_argument_expression($4)*/,temp_ref);count_parm=count_arg;}
  //printf("IDENTIFIER=%s\n",$1);}

;
//22
unary_expr:
postfix_expr {$$=$1;}
| SUB unary_expr {$$=make_tuple(UNARY,$2,NULL,NULL,NULL);}
;

//23
primary_expr:

IDENTIFIER  {$$=ref_var($1);/*printf("\n\njn\n\n\n");*/}
  //printf("IDENTIFIER=%s\n",$1);}
|CONSTANT  {$$=make_constant_node($1);}
    //printf("CONSTANT=%d\n",$1);}
|LP expression RP  {$$=$2;}//make_tupleする？
;
//24

argument_expression_list:
assign_expr{$$=$1;/*parm_num++*/;count_parm++;/*printf("argument_expression/count_parm:[%d]",count_parm);*/}
|  argument_expression_list COMMA assign_expr   {/*parm_num++*/;$$=make_tuple(CONS,$1,$3,NULL,NULL);count_parm++;/*printf("argument_expression/count_parm::%d",count_parm);*/}
;


argument_expression_list_opt:
/*empty*/
{$$=NULL;}
|argument_expression_list{$$=$1;}
;
%%
extern  int  yylineno;

int   yyerror(char *s) {
  fprintf(stderr, "%d:%s\n",yylineno,s);
  return  0;
}

int main() {
  yyparse();


  return 0;
}





