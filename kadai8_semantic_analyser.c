#include <stdio.h>

#include<string.h>
#include<stdarg.h>
#include <stdlib.h>
#include <assert.h>

#include "kadai8.h"
#include "kadai8.tab.h"


#include "kadai8_semantic_analyser.h"
#include "kadai8_print.h"
#include "kadai8_code_generation.h" 
//講義資料２１
int semnerrs;
int yylineno;

//parm_numはcheck_parm_numで使う
int parm_num;

int last_alloc ;
int top_alloc;


//教科書p.169
int allocate_loc(){
  last_alloc=last_alloc -4;
  if(last_alloc<top_alloc){
    top_alloc=last_alloc;
    //printf("allocate_loc/top_alloc:%d,last_alloc:%d",top_alloc,last_alloc);
  }
  return last_alloc;

}
/*
  void release_one_loc(){
  last_alloc += 4;
  }*/
void release_loc(){
  last_alloc+=4;

}
/*
  int allocate_parm_loc(){
  parm_alloc += 4;
  return parm_alloc;
  }*/

void error(char *fmt,...)
{
  va_list argp;
  va_start(argp, fmt);
  semnerrs++;
  fprintf(stderr, "%d: ", yylineno);
  vfprintf(stderr, fmt, argp);
  fprintf(stderr, "\n");
  va_end(argp);
}
void warn(char *fmt,...)
{
  va_list argp;
  va_start(argp, fmt);
  fprintf(stderr, "%d: warning: ", yylineno);
  vfprintf(stderr, fmt, argp);
  fprintf(stderr, "\n");
  va_end(argp);
}



//講義資料２０　変数宣言を行うmake_decl
//構文解析において，変数宣言部に対して次の関数をdeclarator-list へ還元するときのアクションとして実行する．

tree make_decl(tree n)
{
  //  printf("make_decl\n");
  switch (n->tk.kind) {
  case VAR: // すでに変数として名前が宣言されている
    /*p.19 変数宣言や関数定義によって新しいオブジェクトを作成するとき，同一レベルで同じ名前のオブジェクトが
      すでに存在していれば二重宣言/二重定義エラーである

    */

    if (n->tk.lev == cur_lev)
      // 同一レベルであれば二重宣言であるからエラー
      {error("redeclaration of ‘%s’", n->tk.name);}
    //そうでなければ，変数として新たなオブジェクトを作成し，kind をVAR にして，名前をそのオブジェクトに対応させる
    allocate_loc();
    n->tk.offset = last_alloc;
    //printf("make_decl/tk.offset:%d",n->tk.offset);
    //n = make_token_node(n->tk.name);
    /*7/22   offset=offset - 4;
      n->tk.offset=offset;*/
    break;//breakしてからkindをVARにしてreturn



  case FUN: // すでに関数として名前が定義されている
  case UNDEFFUN: // すでに未定義関数として名前が使用されている
    if (n->tk.lev == cur_lev){//レベル0（すなわち大域変数として宣言されている）ならば，二重宣言であるからエラー
      error("‘%s’ redeclared as different kind of symbol",n->tk.name);
    } 
    //そうでなければ
    n = make_token_node(n->tk.name);
 
    break;//breakしてからkindをVARにしてreturn

  case PARM: // すでにパラメータとして名前が宣言されている
    warn("declaration of ‘%s’ shadows a parameter", n->tk.name);
    allocate_loc();
    n->tk.offset = last_alloc;

    //    n = make_token_node(n->tk.name);
    /*7/22 offset=offset - 4;
      n->tk.offset=offset;   */
    break;

  case FRESH://（kind がFRESH のとき），kind をVAR に変更するだけでよい
    //n = make_token_node(n->tk.name);
    allocate_loc();
    n->tk.offset = last_alloc;
    /* 7/22 offset-=4;
       n->tk.offset=offset;  */ 
    break;
  }
  n->tk.kind = VAR;
  //printf("make_decl/tk.offset:%d,tk.kinf:%d",n->tk.offset,n->tk.kind);
  return n;
}

//パラメータ宣言
tree make_parm_decl(tree n)
{//printf("make_parm_decl");
  switch (n->tk.kind) {
  case VAR:
    n = make_token_node(n->tk.name);
    offset= offset+4;
    n->tk.offset=offset;
    break;
  case FUN:
  case UNDEFFUN:
    n = make_token_node(n->tk.name);
    break;
  case PARM://二重宣言がおこるのはパラメータ同士の場合に限られる
    error("redeclaration of ‘%s’", n->tk.name);
    return n;
  case FRESH:
    // n = make_token_node(n->tk.name);
    offset=offset+4;
    n->tk.offset=offset;
    break;
  }
  n->tk.kind = PARM;
  //変更7/22
  //n->tk.offset = allocate_parm_loc();
  //  n->tk.offset=offset_parm();
  //printf("tk.offset:%d",n->tk.offset);
  return n;
}



//関数定義


tree make_fun_def(tree n)
{//printf("make_fun_def");
  switch (n->tk.kind) {
  case VAR://関数同士または大域変数宣言との間であるとき二重宣言
    error("‘%s’ redeclared as different kind of symbol",
	  n->tk.name);
    break;
  case FUN:
    error("redefinition of ‘%s’", n->tk.name);
    break;
  case UNDEFFUN:
    if(n->tk.offset != parm_num)
      error("function '%s'; the number of parameter is wrong", n->tk.name);
 
  case FRESH:
    // printf(" n->tk.kind = FUN,%s;",n->tk.name);
    
    n->tk.kind = FUN;
    break;
  case PARM:
    break;
 
  }
  //変更7/22
  /*n->tk.offset = parm_num;
    parm_num = 0;
    return n;*/
  return n;
}

//変数参照
tree ref_var(tree n){
  //printf("ref_var\n");
  switch (n->tk.kind) {
  case VAR:
  case PARM:
    break;
  case FUN:
  case UNDEFFUN:
    error("function ‘%s’(n->tk.name) is used as variable(ref_var)", n->tk.name);
    break;
  case FRESH:
    error("‘%s’(n->tk.name) undeclared variable(ref_var)", n->tk.name);
    n->tk.kind = VAR; /* エラーリカバリ*/
    break;
  }
  return n;
}
//関数呼び出し
tree ref_fun(tree n)
{//printf("ref_fun%s\n",n->tk.name);
  switch (n->tk.kind) {
  case VAR:
  case PARM:
    error("variable ‘%s’ is used as function", n->tk.name);
    break;
  case FUN:
  case UNDEFFUN:
    
    break;
  case FRESH:
    warn("‘%s’ undeclared function", n->tk.name);
    n->tk.kind = UNDEFFUN;
    if (n->tk.lev > 0) globalize_sym(n);
    else //printf("ref_fun(): unglobalized_sym()\n");
    break;
  }
  return n;
}


//  変更

void check_parm_num(int c, tree n) {
	if(n->tk.kind != UNDEFFUN && n->tk.offset < c) error("too many arguments to function call '%s', expected(temp_ref->tk.offset): %d, have(count_parm): %d", n->tk.name, n->tk.offset, c);
	if(n->tk.kind != UNDEFFUN && n->tk.offset > c) error("too few arguments to function call '%s', expected %d, have %d", n->tk.name, n->tk.offset, c);
}


int count_argument_expression(tree n){
  if(n==NULL){
    return 0;  
  }
  if(n->tp.op==CONS){
    return   1+count_argument_expression(n->tp.a[1]);
  }else{
    return 1;
  }
}




//p.19 pop?
void pop_sym(){
 
  //スコープを終えるレベル値を持つ構造体をたどり，たどり終えた構造体の次の構造体をsymtab で指すようにすればよい

  if(symtab==NULL){

    error("symrtab must not be null\n");
  }
  while(symtab->lev > cur_lev){
    // printf("pop_sym{symtab->lev: %d, cur_lev:%d, symtab->name:%s}\n", symtab->lev, cur_lev, symtab->name);
    symtab =(token)symtab->next;
  }

 



}
void globalize_sym(tree n)//引数で与えられるオブジェクト構造体を大域関数を表すオブジェクトとして登録し直す関数
//n->tk.kindがFRESHであるnがこの関数にわたされる
//１UNDEFにさされているポインタをUNDEFの次のポインタをさすようにする
//2 最初のオブジェクト（nextがNULL）のnextの先にUNDEFをもっていく
{
  token findout_undeffun = symtab;
  token findout_undeffun_before = NULL;
  token looking = symtab;


  //undeffunのトークンを探す
  while(findout_undeffun != &n->tk){//nはundefのオブジェクトであり、みつけるまで走査
    findout_undeffun_before = findout_undeffun;
    findout_undeffun = findout_undeffun->next;
  }

  assert(strcmp(findout_undeffun->name, n->tk.name) == 0);
  //printf("GS:%s\n", n->tk.name);

  //undeffunを抜く分の修正
  if(findout_undeffun_before)
    findout_undeffun_before->next = findout_undeffun->next;//beforeがあればundeffunをとびこしてつなぐ
  else{
    symtab = findout_undeffun->next;
    looking = symtab;
  }


  //findout_undeffunをレベル０にして最初にもってくる
  findout_undeffun->lev = 0;
  findout_undeffun->next = NULL;


  //symtabのトークンを探す。
  while(looking->next){
    looking = looking->next;
  }
  //挿入する
  looking->next = findout_undeffun;



}



//同じ名前のオブジェクトがオブジェクト構造体のスタック上に存在するか調べる関数
//.lで使用する
tree lookup_sym(char *yytext){
  //printf("lookup_sym\n");
  //名前を引数にとり、スタックを上から順に走査して同名のオブジェクトがみつかればそれをtree型の値として返す
  
  //token tmp=symtab;
  /*デバッグ用
    printf("lookup[%s]:", yytext);
    while(tmp!=NULL){
    printf("/%s:%d",tmp->name, tmp->lev);
    tmp=tmp->next;
    }
    printf("\n");
  */
  token looking_stack=symtab;
 

  while(looking_stack!=NULL){//最後のオブジェクトでなければ繰り返す(symtabの初期値はNULL)

    if(strcmp(looking_stack->name,yytext)==0){//同名のオブジェクトがあればそれをtree型の値として返す
      //   printf("hit/%s:%d\n",looking_stack->name, looking_stack->lev);
      return (tree)looking_stack;
    }else{//次のスタックを走査してwhileに戻る
      //printf("lookup_sym{symtab->lev: %d, cur_lev:%d, symtab->name:%s}\n", symtab->lev, cur_lev, symtab->name);
      looking_stack=looking_stack->next;
    }
  }//最後まで同名のオブジェクトがみつからなければNULLを返す(symtabの初期値はNULL)
  return NULL  ;
}
 

tree lookup_num(int num) {
  tree  t =(tree) symtab;
  while(t){
    if(num >= t->tk.lev) return t;
    else t = (tree)t->tk.next;
    offset = offset + 4;
  }
  return t;
}




//make_token_node呼び出されたということはスタックに積むことがきまった事後
tree make_token_node(char *str){
  //treeのtokenのnameにidentifierをいれる
 

  //新しく生成された構造体をスタックに積む（p。１８）
  //スタックにむ。tree_p->nextをいじくる


  //構造体のメモリ割り当て
  tree tree_p=(tree) malloc (sizeof(*tree_p));

  tree_p->tk.op =TOKEN;
  tree_p->tk.name= strdup(str);//strのmallocとcopyするstrdup
  
  tree_p->tk.lev=cur_lev;//現在のブロックレベルを保持するint 型の大域変数cur_lev を導入しておき，make_token_node()でオブジェクト構造体を生成するときに，cur_lev の値をlev の値として記憶する（p.18）
  // printf("FUGAFUGA %s : %d\n", tree_p->tk.name, tree_p->tk.lev);
  tree_p->tk.kind=FRESH;
  tree_p->tk.next=symtab;//symtab はtoken 型の大域変数であり，リストの先頭すなわちスタックのトップを指すのに使用する．
  // tree_p->tk.offset=offset_();//関数offsetはsemantic_analyserで定義する
  symtab= (token)tree_p;//symtabの初期値はNULL
  return tree_p;
}

tree make_constant_node(int op){
  //tree

  //構造体のメモリ割り当て
  //printf("make_constant_node\n");
  tree tree_p=(tree) malloc (sizeof(*tree_p));
  tree_p->c.op =CONSTANT;
  tree_p->c.v =op;
  //  printf("make_constant_node,op:%d\n",op);
  return tree_p;
}

tree make_tuple(int op,tree a0, tree a1,tree a2, tree a3 ){
  //  printf("make_tuple\n");
  //構造体のメモリ割り当て
  tree tree_p=(tree) malloc (sizeof(*tree_p));
 
 
  tree_p->tp.op=op;//ノードの種類をtreeのノードに入れる
  tree_p->tp.a[0] =a0;
  tree_p->tp.a[1] =a1;
  tree_p->tp.a[2] =a2;
  tree_p->tp.a[3] =a3;
  //printf("make_tuple,op:%d\n",op);
  return tree_p;
}



