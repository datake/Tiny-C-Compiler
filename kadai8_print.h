#ifndef _PRINT_
#define _PRINT_

/*
//構造体の定義
typedef struct c{ //constant node
  int op;//ノードの種類
  int v;//value
} *constant;//cという構造体の型があって、constantを焼く。ポインタを使うと宣言


typedef struct tk {
  int op;//ノードの種類
  struct tk *next;//自己参照構造体。スタックをリストとして実装するための次のリストとして使う
char *name;
  //levは宣言されたブロックのレベル
int lev;
  //kindはオブジェクトの種類を表す

enum {FRESH, VAR, FUN, PARM, UNDEFFUN} kind;
  //oddset:局所変数VAR及びパラメータPARMの場合は，その値の格納場所（関数フレーム内の相対番地; ベースポインタebp を基準とした番地）を保持
  //offset:オブジェクトが関数FUNの場合は，パラメータの数を記憶
int offset;
} *token;


typedef struct tp {
  //n-tuple (n=4)
  int op;//ノードの種類を表す
  union nd *a[4];//枝を表すポインタ
} *tuple;

typedef union nd {
  //tree
  struct {//opを共用体に属する任意の構造体のメンバとして参照することが許されている．共用体のメンバnはこのopを参照するためのラベル
int op;
} n;
  struct tp tp;//
  struct tk tk;//
  struct c c;//
}*tree;
*/

//大域変数
//extern int cur_lev;
//extern token symtab;

extern int print_parm(tree p);

extern void print_program(tree p);//1
extern void print_external_declaration(tree p);//2
extern void print_declaration(tree p);//3
extern void print_declarator_list(tree p);//4
extern void print_declarator(tree p);//5
extern void print_function_definition(tree p);//6
extern void print_parameter_type_list(tree p);//7
extern void print_parameter_type_list_opt(tree p);//7

extern void print_parameter_declaration(tree p);//8
extern void print_statement(tree p);//9
extern void print_compound_statement(tree p);//10
extern void print_declaration_list(tree p);//11
extern void print_opt_statement_list(tree p);//12
extern void print_opt_declaration_list(tree p);//11
extern void print_statement_list(tree p);//12
extern void print_expression(tree p);//13
extern void print_assign_expr(tree p);//14
extern void print_logical_OR_expr(tree p);//15
extern void print_logical_AND_expr(tree p);//16
extern void print_equality_expr(tree p);//17
extern void print_relational_expr(tree p);//18
extern void print_add_expr(tree p);//19
extern void print_mult_expr(tree p);//20
extern void print_postfix_expr(tree p);//21
extern void print_unary_expr(tree p);//22
extern void print_primary_expr(tree p);//23
extern void print_argument_expression_list(tree p);//24

extern void print_argument_expression_list_opt(tree p);


#endif
