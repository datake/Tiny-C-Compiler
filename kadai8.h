#ifndef _TREE_ //もし_STRUCTが定義されていなければ#endifまでのコードを残す
#define _TREE_
/*構造体の定義*/
typedef struct c{ /*constant node*/
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
  int op;/*ノードの種類を表す*/
  union nd *a[4];/*枝を表すポインタ*/
} *tuple;

typedef union nd {//tree
  struct{/*opを共用体に属する任意の構造体のメンバとして参照することが許されている．共用体のメンバnはこのopを参照するためのラベル*/
    int op;
  } n;
  struct tp tp;//
  struct tk tk;//
  struct c c;//
}*tree;


/*データ生成関数のプロトタイプ宣言*/
/*extern tree make_tuple(int, tree, tree, tree, tree);
  extern tree make_token_node(char *);
  extern tree make_constant_node(int);

  extern void pop_token_node();
  extern tree lookup_syn(char *);
*/
/*
  extern tree make_decl(tree p);
  extern tree make_fundef(tree p);
  extern tree ref_var(tree p);
  extern tree ref_fun(tree p);

  extern int yylineno;
  extern int yyerror(char *s);

*/

int top_alloc;
int last_alloc;
int count_parm;
int offset;
int cur_lev;
int parm_alloc;
int parm_num;
int count_arg;
#endif
