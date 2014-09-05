
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kadai8.h"
#include "kadai8.tab.h"
#include "kadai8_semantic_analyser.h"
#include "kadai8_print.h"
#include "kadai8_code_generation.h" 


struct code *final_code;
struct code *first_code;
char *label_return_jump;
int label_count=0;
int return_label_count=0;
//int parm_num=0;
int parm_num_esp;

//yaccで宣言したのになぜ必要？


//資料７．２
//1命令のコード生成を行う関数がemit
//emitの引数を一つふやしてラベルも扱う
struct code *emit(char *inst, char *op1, char *op2, char *label){
  char buf[80];
  struct code *c = (struct code *)malloc(sizeof(struct code));


  if (label == NULL){//資料
    if (inst == NULL)
      buf[0] = '\0';
    else if (op1 == NULL)//commonはここにいれる?
      sprintf(buf, "\t%s\n", inst);
    else if (op2 == NULL)
      sprintf(buf, "\t%s\t%s\n", inst, op1);
    else
      sprintf(buf, "\t%s\t%s, %s\n", inst, op1, op2);
  }else{//label!=NULLのときlabelをbufそしてcの先頭にいれる
    if (inst == NULL)
      sprintf(buf, "%s\n", label);
    else if (op1 == NULL)
      sprintf(buf, "%s\t%s\n", label, inst);
    else if (op2 == NULL)
      sprintf(buf, "%s\t%s\t%s\n", label, inst, op1);
    else
      sprintf(buf, "%s\t%s\t%s, %s\n", label, inst, op1, op2);
  }

  c->cd = strdup(buf);
  c->next=NULL;
  //資料p.28 構造体cをコード列リストの最後に追加
  
  if(first_code){//firstコードがまだ定義されているとき
    final_code->next=c;//最終尾につなぎ、つないだものを最終尾とする
    final_code=c;
  }else{ //first,finalコードに最初に定義するとき
    first_code=c;
    final_code=c;
  }
  return c;
}
//一意的なラベルを生成。整数型の大域変数label_countでラベルのカウント。nにたいしてLabel_Nを生成(p.29)
char *make_label(){
  char label_count_string[64];
  char L[64] = "L";
  char *Label_N;

  label_count++;
  
  //int型のlabel_countをcharに変換
  sprintf(label_count_string, "L%d", label_count);

  //  strcat(L,label_count_string);//文字列配列Lの後ろにlabel_count_charを連結(label_countではエラー)
  Label_N = strdup(label_count_string);//mallocとcopyを行いポインタを代入
  // printf("make_counrLabel_N:%s",Label_N);
  
  return Label_N;
}



//"L"+"return_label_count"+"ret"を返す
char *make_label_return(){
  char label_count_char[64];
  char L[64] = "R";
  char *Label_N_ret;

  return_label_count++;
  
  //int型のlabel_countをcharに変換
  //printf("make_label_return\n");
  sprintf(label_count_char, "%d", return_label_count);

  strcat(L,label_count_char);//文字列配列Lの後ろにlabel_count_charを連結(label_countではエラー)
  Label_N_ret = strdup(L);//mallocとcopyを行いポインタを代入

  return Label_N_ret;
}






char *ebp_set(tree p) {
  char *ebp_buf = (malloc(sizeof(char) *4 ));
  if(p->tk.lev == 0) sprintf(ebp_buf, "[%s]", p->tk.name);
  else if(p->tk.offset > 0) sprintf(ebp_buf, "[ebp+%d]", p->tk.offset);
  else if(p->tk.offset < 0) sprintf(ebp_buf, "[ebp%d]", p->tk.offset);
  return ebp_buf;
}


//yacc でつかわれる満数でコード列リストを表示する
void print_code(){
  struct code *now_printing=first_code; 
  while(now_printing){

    printf("%s",now_printing->cd);
    now_printing=now_printing->next;
  }
}



//1
void emit_program(tree p) {
  top_alloc = 0;
  last_alloc = 0;

  // printf("1program\n");
  if (p->n.op != CONS) {
    //external_declaration {$$=$1;}
    emit_external_declaration(p);
  
  } else {
    //|program external_declaration {$$=make_tuple(CONS,$1,$2,NULL,NULL);}
    emit_program(p->tp.a[0]);
    emit_external_declaration(p->tp.a[1]);
  
  }
}
//2
void emit_external_declaration(tree p) {
  //printf("2external_declaration\n");
  if (p->n.op == DECLARATION) { 
    // printf("("); 
    emit_declaration(p);
    // printf(")\n");
  }else if(p->n.op ==FUNDEF1){
    //printf("("); 
    emit_function_definition(p);
    //printf(")\n");
  }
}


//3
void emit_declaration(tree p){
  // printf("3declaration\n");
  //  printf("("); 
  //printf("INT");

  emit_declarator_list(p->tp.a[0]);
  //printf(";");
  //printf(")\n"); 

  
}

//4
void emit_declarator_list(tree p){
  //printf("4declarator_list\n");
  if (p->n.op ==CONS) {
    emit_declarator_list(p->tp.a[0]);
    //printf(",");
    emit_declarator(p->tp.a[1]);
  
  }else{
    emit_declarator(p);
  }
}

//5
void emit_declarator(tree p){
  // printf("5declarator\n"); 
  /* printf("IDENTIFIER "); */

  //大域変数の定義するとき、大域データ領域にデータを割り当てるcommonを使う
  //4バイトのメモリを大域データ領域に割当て、そのアドレスをlabelとする(資料p.27)
  if(p->tk.lev == 0){ 
    char common_label_4[64];
    char str[100];
    strcpy(common_label_4, p->tk.name);

    sprintf(str,"COMMON\t%s\t4\n", common_label_4);
  
    //emit関数を使って表示する
    emit(str,NULL,NULL,NULL);
  }
  

}


//6 資料 7.2関数定義のコード生成
void emit_function_definition(tree p){
  // printf("6function_definition\n");
  if(p->n.op==FUNDEF1){
    //int declarator:a[0](parameter_type_list_opt:a[1]){compound_statement:a[2]}

    //  printf("INT¥n");
    char *function_name;
    struct code *c;
    char label_function_name[64];
    char label_return[64];
    function_name = strdup(p->tp.a[0]->tk.name);
    label_return_jump = make_label_return();
    strcpy(label_return, label_return_jump);
    strcat(label_return, ":");
    strcpy(label_function_name, function_name);
    strcat(label_function_name, ":");
  
    //関数定義のコード生成 p.29
    emit("GLOBAL", function_name, NULL, NULL);
    emit("push", "ebp", NULL, label_function_name);
    emit("mov", "ebp", "esp", NULL);
    c = emit(NULL, NULL, NULL, NULL);
    //関数本体のコード生成 p.29
    emit_parameter_type_list_opt(p->tp.a[1]);
    emit_compound_statement(p->tp.a[2]);
    //Nlocalの部分のコード生成 p.29
    //last_allocは最後にわりあてられまだ開放されていない局所変数の相対番地
    //top_allocはこれまでのtop_allocの最小値で
    //関数本体のコード生成が終了したときtop_allocを符号反転させたものがNlocalとなる（教科書p.168）
    // printf("top_alloc!=0:%d",top_alloc);
    if (top_alloc!=0){//Nlocalが0でなければ
     
      char buf[80];
      sprintf(buf, "\tsub\tesp, %d\n", -top_alloc);
      c->cd = strdup(buf);
      emit("mov", "esp", "ebp", label_return);
      emit("pop", "ebp", NULL, NULL);
      emit("ret", NULL, NULL, NULL);
    } else {//Nlocalが0ならば
      emit("pop", "ebp", NULL, label_return);
      emit("ret", NULL, NULL, NULL);
    }
 
  }

 
}

//6-2
void emit_parameter_type_list_opt(tree p){
  if(p==NULL){}
  else{
    emit_parameter_type_list(p);  
  }

}
//7
void emit_parameter_type_list(tree p){
  //printf("7parameter_type_list\n");
  if(p->n.op!= CONS){
    emit_parameter_declaration(p);
   
  }else{
    emit_parameter_type_list(p->tp.a[0]);
    //  printf(",");
    emit_parameter_declaration(p->tp.a[1]);
   
  }
}


//8
void emit_parameter_declaration(tree p){
  // printf("8parameter_declaration\n");
  //printf("INT¥n");
  //emit_parm(p);
  emit_declarator(p);
}
//9
void emit_statement(tree p){
  // printf("9statement\n");
  if(p==NULL){
    //printf(";\n");
    return;
    
  }
  switch(p->tp.op){
  case STATEMENT_EXPRESSION: { 
    //  if(p->n.op== STATEMENT_EXPRESSION){
    emit_expression(p->tp.a[0]);
    // printf(";");
    // printf("\n");
  
   

    break;
  }
  case STATEMENT_IF:{
 


    //ラベル生成
    char *label_if_jump = make_label();
  
    char label_if[64];
    strcpy(label_if, label_if_jump);
  
    strcat(label_if, ":");
    
    //コード生成
    //条件判定
    emit_expression(p->tp.a[0]);
 
    emit("cmp", "eax", "0", NULL);
    emit("je", label_if_jump, NULL, NULL);
    //trueだった場合の処理
    emit_statement(p->tp.a[1]);
    emit(NULL, NULL, NULL, label_if);

    break;
  }
  case STATEMENT_IF_ELSE:{

    //教科書p.173
    //ラベル生成
    char *label_false_jump = make_label();
    char *label_right_jump = make_label();
    char label_false[64], label_right[64];
    strcpy(label_false, label_false_jump);
    strcpy(label_right, label_right_jump);
    strcat(label_false, ":");
    strcat(label_right, ":");
    //コード生成
    //条件判定
    emit_expression(p->tp.a[0]);
   
    emit("cmp", "eax", "0", NULL);
    emit("je", label_false_jump, NULL, NULL);
    
    emit_statement(p->tp.a[1]);
    emit("jmp", label_right_jump,  NULL, NULL);
    emit(NULL, NULL, NULL, label_false);
    emit_statement(p->tp.a[2]);
    emit(NULL, NULL, NULL, label_right);

    break;
  }
  case STATEMENT_WHILE:{
    // }else if(p->n.op== STATEMENT_WHILE){
    //資料p.30 教科書p.174
    //先頭ラベル作成
    char *label_first_jump = make_label();
    //末尾ラベル作成
    char *label_last_jump = make_label();
    char label_first[64], label_last[64];
    strcpy(label_first, label_first_jump);
    strcpy(label_last, label_last_jump);
    strcat(label_first, ":");
    strcat(label_last, ":");
    //条件式のコード生成
    emit(NULL, NULL, NULL, label_first);
    emit_expression(p->tp.a[0]);
    emit("cmp", "eax", "0", NULL);
    emit("je", label_last_jump, NULL, NULL);
    //true:本体のコード生成
    emit_statement(p->tp.a[1]);
    emit("jmp", label_first_jump, NULL, NULL);
    //false:末尾ラベル挿入
    emit(NULL, NULL, NULL, label_last);
  
    break;
  }
  case STATEMENT_RETURN:{
    //  }else if(p->n.op== STATEMENT_RETURN){
 
    // printf("RETURN¥n");
   
    if (p->tp.a[0] != NULL)
      emit_expression(p->tp.a[0]);
    //FUNDEFで定義したreturnラベルに飛ぶ
    emit("jmp", label_return_jump, NULL, NULL);
 


  
    break;
  }
  default:
    // printf("emit_compound_statement");
    emit_compound_statement(p);
    break;
  }
}

//10
void emit_compound_statement(tree p){
  //printf("10compound_statement\n");
  if(p->n.op== COMP_STATE){
    //printf("{");
    // if(p->tp.a[0]!=NULL){
    // printf("PIYOPIYO\n"); 
    // printf("opt_decl !=NULL\n");
    emit_opt_declaration_list(p->tp.a[0]);
    // }else{
    // printf("opt_decl =NULL\n");
    // }
    // if (p->tp.a[1]!=NULL){
    // printf("opt_state!=NULL\n");
 
    emit_opt_statement_list(p->tp.a[1]);
    // }else{
    // printf("opt_state =NULL\n");
    // }
    // printf("}\n");
   
    // }else{
    //  printf("error:compound statement");
    //}
  }
}
//10-2
void emit_opt_declaration_list(tree p){

  //printf("10-1opt_declaration_list\n");
  // printf("%d\n",p->tp.a[0]) ;

  if(p!=NULL){
    //printf("!=NULL  opt_declaration_list\n");
    emit_declaration_list(p);
  }else{
    // printf("NULL   opt_declaration_list\n");}
  }
}
//10-3
void emit_opt_statement_list(tree p){
  // printf("opt_statement_list\n");
  if(p!=NULL){
    // printf("!=NULL opt_statement_list\n");
    emit_statement_list(p);
  }else{
    // printf("NULL opt_statement_list\n");  
    //emit_statement_list(p/\*->tp.a[1]*\/); 
    
  }
}



//11
void emit_declaration_list(tree p){
  // printf("11declaration_list\n");
  if(p->n.op!= CONS){
    emit_declaration(p);
    allocate_loc();
   
  }else{
    emit_declaration_list(p->tp.a[0]);
       
    emit_declaration(p->tp.a[1]);
    allocate_loc();
    
  }
}
//12
void emit_statement_list(tree p){
  // printf("12statement_list\n");
  if(p->n.op!= CONS){
    // printf("!=cons\n");
    emit_statement(p);
   
  }else{//printf("else");
    emit_statement_list(p->tp.a[0]);
    emit_statement(p->tp.a[1]);
  
  }
}
//13
void emit_expression(tree p){
  // printf("13expression\n");
  if(p->n.op!= CONS){
    emit_assign_expr (p);
  
  }else{
    emit_expression (p->tp.a[0]);
    // printf(", ");
    emit_assign_expr (p->tp.a[1]);
    
  }
}
//14
void emit_assign_expr(tree p){

  // printf("14assign_expr\n");
  if(p->n.op==ASSIGN){//教科書p.176
 
    //コード作成
    emit_assign_expr(p->tp.a[1]);
    //追加(7/25）
    char *identifier = (malloc(sizeof(char) *4 ));
    identifier = ebp_set(p->tp.a[0]);


    emit("mov", identifier, "eax", NULL);
   
  } else{
    emit_logical_OR_expr(p);
   
  }
}

//15
void emit_logical_OR_expr(tree p){
  //教科書p.193
  // printf("15logical_OR_expr\n");
  if(p->n.op!= OR){
    emit_logical_AND_expr(p);
   
  }else{

    
    //準備(一時変数を用意、ラベル生成)
    char temp[64] = "[ebp", address[64];
    char d_word[64] = "dword ";
    char *label_j = make_label();
    char label[64];
    strcpy(label, label_j);
    strcat(label, ":");
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");
    strcat(d_word, temp);

    //コード生成
    emit("mov", d_word, "1", NULL);
    emit_logical_OR_expr(p->tp.a[0]);
    emit("cmp", "eax", "1", NULL);
    emit("je", label_j, NULL, NULL);
    emit_logical_AND_expr(p->tp.a[1]);
    emit("cmp", "eax", "1", NULL);
    emit("je", label_j, NULL, NULL);
    emit("mov", d_word, "0", NULL);
    emit("mov", "eax", temp, label);

    release_loc();
  }
 
   
}
//16
void emit_logical_AND_expr(tree p){
  //教科書p.193
  //printf("16logical_AND_expr\n");
  if(p->n.op!= AND){
    emit_equality_expr(p);
  }else{
    //準備(一時変数を用意、ラベル生成)
    char temp[64] = "[ebp", address[64];
    char d_word[64] = "dword ";
    char *label_j = make_label();
    char label[64];
    strcpy(label, label_j);
    strcat(label, ":");
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");
    strcat(d_word, temp);

    //コード生成 資料p.32 e1&&e2
    //mov dword temp,0
    emit("mov", d_word, "0", NULL);
    emit_logical_AND_expr(p->tp.a[0]);
    //e1が偽ならlabel_jへ
    emit("cmp", "eax", "0", NULL);
    emit("je", label_j, NULL, NULL);
    //e2が偽ならlabel_jへ
    emit_equality_expr(p->tp.a[1]);
    emit("cmp", "eax", "0", NULL);
    emit("je", label_j, NULL, NULL);
    //mov dword temp,1
    emit("mov", d_word, "1", NULL);
    //label_j:mov eax temp
    emit("mov", "eax", temp, label);

    release_loc();
  }

   
}


//17
void emit_equality_expr(tree p){
  //printf("n.op:%d\n",p->n.op);
  // printf("17equality_expr\n");
 
  if(p->n.op==NOT_EQUAL_TO ){
    //  printf("not equal_to");
   
    
    //一時変数の用意
    char temp[64] = "[ebp", address[64];
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");

    //コード生成(資料p.31 e1!=e2)
    //e2の計算
    emit_equality_expr(p->tp.a[0]);//変更
    //mov temp eax 
    emit("mov", temp, "eax", NULL);
    //e1の計算
    emit_relational_expr(p->tp.a[1]);//変更
    //cmp eax,temp
    emit("cmp", "eax", temp, NULL);







    //setne al
    emit("setne", "al", NULL, NULL);
    //movzx eax al
    emit("movzx", "eax", "al", NULL);
    
    release_loc();
     


  }else if(p->n.op==EQUAL_TO ){
    //printf("equal to");
   

    //一時変数を用意
    char temp[64] = "[ebp", address[64];
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");

    //コード生成(資料p.31 e1==e2)
    //e2の計算
    emit_equality_expr(p->tp.a[0]);
    //mov temp,eax
    emit("mov", temp, "eax", NULL);
    //e1の計算
    emit_relational_expr(p->tp.a[1]);
    //cmp eax,temp
    emit("cmp", "eax", temp, NULL);
    //sete al
    emit("sete", "al", NULL, NULL);
    //movzx eax al
    emit("movzx", "eax", "al", NULL);

    release_loc();

  }else{
    // printf("else");



    emit_relational_expr(p);
  
    
  }
}
//18
void emit_relational_expr(tree p){
  //printf("18relational_expr");
  if(p->n.op==GREATER_THAN){

    //一時変数を用意
    char temp[64] = "[ebp", address[64];
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");

    //コード生成 (資料p.31 e1!=e2)
    //e2の計算
   
    emit_relational_expr(p->tp.a[0]);//変更
    //mov temp eax
    emit("mov", temp, "eax", NULL);
    //e1の計算
    emit_add_expr(p->tp.a[1]);//変更
    //cmp eax,temp
    emit("cmp", "eax", temp, NULL);
    //setl al                                                                          
    emit("setl", "al", NULL, NULL);
    //movzx eax al
    emit("movzx", "eax", "al", NULL);

    release_loc(); 

   
  }else if(p->n.op==LESS_THAN){
    //一時変数を用意
    char temp[64] = "[ebp", address[64];
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");
    //コード生成
    emit_relational_expr(p->tp.a[0]);//変更
    //  emit_add_expr(p->tp.a[1]);
    emit("mov", temp, "eax", NULL);
    emit_add_expr(p->tp.a[1]);//変更   
    // emit_relational_expr(p->tp.a[0]);
    emit("cmp", "eax", temp, NULL);
    emit("setg", "al", NULL, NULL);
    emit("movzx", "eax", "al", NULL);

    release_loc();
  
  }else if(p->n.op==GREATER_THAN_EQUAL){

    //一時変数を用意
    char temp[64] = "[ebp", address[64];
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");

    //コード生成
    emit_relational_expr(p->tp.a[0]);

    //   emit_add_expr(p->tp.a[1]);
    emit("mov", temp, "eax", NULL);
    emit_add_expr(p->tp.a[1]);
    //      emit_relational_expr(p->tp.a[0]);
    emit("cmp", "eax", temp, NULL);
    emit("setle", "al", NULL, NULL);
    emit("movzx", "eax", "al", NULL);

    release_loc();
  
  }else if(p->n.op== LESS_THAN_EQUAL){

    //一時変数を用意
    char temp[64] = "[ebp", address[64];
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");

    //コード生成
    emit_relational_expr(p->tp.a[0]);
    // emit_add_expr(p->tp.a[1]);
    emit("mov", temp, "eax", NULL);
    //emit_relational_expr(p->tp.a[0]);
    emit_add_expr(p->tp.a[1]);     
    emit("cmp", "eax", temp, NULL);
    emit("setge", "al", NULL, NULL);
    emit("movzx", "eax", "al", NULL);

    release_loc();

   
  }else{
    emit_add_expr(p);
  }
}

//19
void emit_add_expr(tree p){
  // printf("19add_expr");
  if(p->n.op== ADD){
   
    //一時変数を用意
    char temp[64] = "[ebp", address[64];
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");

    //コード生成
    emit_mult_expr(p->tp.a[1]);
    //  emit_add_expr(p->tp.a[0]);
    //右オペランドのコード生成
    //emit("mov",一時変数,"eax");
    emit("mov", temp, "eax", NULL);
    emit_add_expr(p->tp.a[0]);
    //    emit_mult_expr(p->tp.a[1]);
    //左オペランドのコード生成
    //emit("命令","eax",一時変数);
    emit("add", "eax", temp, NULL);
    //一時変数の解放
    release_loc();
 
  } else if(p->n.op== SUB){


    //引き算のマイナス
    //一時変数を用意
    char temp[64] = "[ebp", address[64];
    sprintf(address, "%d", allocate_loc());
    strcat(temp, address);
    strcat(temp, "]");

    //コード生成
    // emit_add_expr(p->tp.a[1]);
    emit_mult_expr(p->tp.a[1]);
  
    emit("mov", temp, "eax", NULL);
    //引き算
    emit_add_expr(p->tp.a[0]);
    //  emit_mult_expr(p->tp.a[0]);
   
    emit("sub", "eax", temp, NULL);
    release_loc();
  
    
  }else{
    emit_mult_expr(p);}
  
}

//20
void emit_mult_expr(tree p){
  // printf("20mult_expr");
  if(p!=NULL){

   

  
    if(p->n.op== MULTI){

      //一時変数を用意
      char temp[64] = "[ebp", address[64];
      sprintf(address, "%d", allocate_loc());
      strcat(temp, address);
      strcat(temp, "]");

      //コード生成
      //   emit_unary_expr(p->tp.a[1]);
      emit_mult_expr(p->tp.a[0]);
      //    emit_mult_expr(p->tp.a[0]);
      emit("mov", temp, "eax", NULL);
      // emit_mult_expr(p->tp.a[0]);
      emit_unary_expr(p->tp.a[1]);
      emit("imul", "eax", temp, NULL);

      release_loc();
     
    }else if(p->n.op== DIV){
      //資料p.31 e1/e2
      //一時変数を用意
      char temp[64] = "[ebp", address[64];
      char d_word[64] = "dword ";
      sprintf(address, "%d", allocate_loc());
      strcat(temp, address);
      strcat(temp, "]");
      strcat(d_word, temp);

      //コード生成
      //e2の計算
      //  emit_unary_expr(p->tp.a[0]);    
      emit_mult_expr(p->tp.a[0]);
      //mov temp,eax
      emit("mov", temp, "eax", NULL);
      //e1の計算
      emit_unary_expr(p->tp.a[1]);
      // emit_mult_expr(p->tp.a[1]);
      //cdq
      emit("cdq", NULL, NULL, NULL);
      //idiv dword temp
      emit("idiv", d_word, NULL, NULL);

      release_loc();
    
    }else{
      emit_unary_expr(p);
    }
  }else {
    // printf("p=NULL");
  }
}
//21
void emit_postfix_expr(tree p){
  // printf("21postfix_expr");
  //教科書p.160 資料p.32 関数呼び出し
  if(p->n.op== POSTFIX){
    parm_num = 0;
    // printf("\n\npostfix bigin\nemit_postfix_expression/parm_num:%d,%s",parm_num, p->tp.a[0]->tk.name);
    char address[64];
    //未定義の関数を呼び出すときには、callするたびにexternを指示してもよい
    emit_argument_expression_list_opt(p->tp.a[1]);
    parm_num=0;

  
      
    // printf("\n\n\nemit_postfix_expression/parm_num:%d",parm_num);
    if(p->tp.a[0]->tk.kind == UNDEFFUN)
      emit("EXTERN", p->tp.a[0]->tk.name, NULL, NULL);
    //引数がある場合、引数の中身を計算してpushする
    if (p->tp.a[1]){//argument_exp_listがnullでない場合
      tree t = p->tp.a[1];
      while(t->tp.op == CONS){



	parm_num++;
	t = t->tp.a[0];
      }
     

      parm_num++;
      parm_num_esp=parm_num;
    
    }else{
      parm_num_esp=0;//引数ないとき
    }
    //関数呼び出し
    // printf("\n\n\nemit_postfix_expression/parm_num:%d",parm_num);
    emit("call", p->tp.a[0]->tk.name, NULL, NULL);
    sprintf(address, "%d", parm_num_esp * 4);
    emit("add", "esp", address, NULL);
    
 
  }else{
    emit_primary_expr(p);
    
  }
}


void emit_argument_expression_list_opt(tree p){
  if(p==NULL){}
  else{  parm_num=0;
    emit_argument_expression_list(p); 
 
    //   printf("\n\n\nemit_argument_exp_list/parm_num:%d",parm_num);
  }

}
//22
void emit_unary_expr(tree p){
  //  printf("22unary_expr");
  if(p->n.op== UNARY){
    emit_unary_expr(p->tp.a[0]);
    emit("imul","eax","-1",NULL);


  }else{
    emit_postfix_expr(p);
   
  }

}
//23
void emit_primary_expr(tree p){
  // printf("23primary_expr");
  if(p->tk.op==TOKEN){
   
    //  printf("IDENTIFIER");
    //showのidentifier の局所変数の場合
    char loc[256] = "[ebp", offset[64];
    sprintf(offset, "%+d", p->tk.offset);//%+が符号つきの数字を表示
    strcat(loc, offset);
    strcat(loc, "]");

    //コード生成
    emit("mov", "eax", loc, NULL);

    //    emit_parm(p);
    // printf("%s",p->tk.name);//make_token_node
  }else if(p->c.op ==CONSTANT){


    char value[64];
    sprintf(value, "%d", p->c.v);
    emit("mov", "eax", value, NULL);
    /*  printf("make_constant_node");
	printf("%d",p->c.v);//make_constant_node
    */
  }else{
    //printf("(");
    emit_expression(p);
    //printf(")\n");

  }


}
//24
void emit_argument_expression_list(tree p){
  //  printf("24argument_expression_list");
  if(p->n.op!= CONS){
    emit_assign_expr(p);
    emit("push", "eax", NULL, NULL);
    parm_num++;
    parm_num_esp=parm_num++;
    // printf("\n\nemit_argument_expression/parm_num:%d",parm_num);
  }else{
    emit_assign_expr(p->tp.a[1]);
    emit("push", "eax", NULL, NULL);
    parm_num++; 
    emit_argument_expression_list(p->tp.a[0]);
      	
    //  printf("\n\nemit_argument_expression/parm_num:%d",parm_num);
  
  }
}



