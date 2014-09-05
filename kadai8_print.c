#include <stdio.h>

#include<string.h>
#include<stdarg.h>
#include "kadai8.h"
#include "kadai8.tab.h"
#include "kadai8_semantic_analyser.h"
#include "kadai8_print.h"
#include "kadai8_code_generation.h" 





int print_parm(tree p){
  printf("int %s:%d:%d", p->tk.name, p->tk.lev, p->tk.offset);
  return 0;
}




//構文木の表示
//$$=$1ならprint_?(p)
//$$=make_?ならp->tp.a[?]

//1
void print_program(tree p) {
  printf("1program\n");
  if (p->n.op != CONS) {
    //external_declaration {$$=$1;}
    print_external_declaration(p);
  
  } else {
    //|program external_declaration {$$=make_tuple(CONS,$1,$2,NULL,NULL);}
    print_program(p->tp.a[0]);
    print_external_declaration(p->tp.a[1]);
  
  }
}
//2
void print_external_declaration(tree p) {
  printf("2external_declaration\n");
  if (p->n.op == DECLARATION) { 
    printf("("); 
    print_declaration(p);
    printf(")\n");
  }else if(p->n.op ==FUNDEF1){
    printf("("); 
    print_function_definition(p);
    printf(")\n");
  }
}


//3
void print_declaration(tree p){
  printf("3declaration\n");
  printf("("); 
  //printf("INT");

  print_declarator_list(p->tp.a[0]);
  printf(";");
  printf(")\n"); 

  
}

//4
void print_declarator_list(tree p){
  printf("4declarator_list\n");
  if (p->n.op ==CONS) {
    print_declarator_list(p->tp.a[0]);
    printf(",");
    print_declarator(p->tp.a[1]);
  
  }else{
    print_declarator(p);
  }
}

//5
void print_declarator(tree p){
  printf("5declarator\n"); 
  /* printf("IDENTIFIER "); */
  print_parm(p);
  /* printf("%s",p->tk.name); */
  //make_token_node

  
}



//6
void print_function_definition(tree p){
  printf("6function_definition\n");
  if(p->n.op==FUNDEF1){
    //  printf("INT%*s",1,"");
    //  print_parm(p->tp.a[0]);

    print_declarator(p->tp.a[0]);
    printf("(");
    print_parameter_type_list_opt(p->tp.a[1]);
    printf("){\n");
    print_compound_statement(p->tp.a[2]);
    printf("}\n");
  }
}

void print_parameter_type_list_opt(tree p){
  if(p==NULL){}
  else{
    print_parameter_type_list(p);  
  }

}

//7
void print_parameter_type_list(tree p){
  printf("7parameter_type_list\n");
  if(p->n.op!= CONS){
    print_parameter_declaration(p);
   
  }else{
    print_parameter_type_list(p->tp.a[0]);
    printf(",");
    print_parameter_declaration(p->tp.a[1]);
   
  }
}

//8
void print_parameter_declaration(tree p){
  printf("8parameter_declaration\n");
  //printf("INT%*s",1,"");
  //print_parm(p);
  print_declarator(p);


}
//9
void print_statement(tree p){
  printf("9statement\n");
  if(p==NULL){
    printf(";\n");
    return;
    
  }
  if(p->n.op== STATEMENT_EXPRESSION){
    print_expression(p->tp.a[0]);
    printf(";");
    printf("\n");
  }else if(p->n.op== STATEMENT_IF){
    printf("IF");
    printf("(");
    
    print_expression(p->tp.a[0]);
    printf(")\n");
    print_statement(p->tp.a[1]);
    printf("\n");
  }else if(p->n.op== STATEMENT_IF_ELSE){
    printf("IF");
    printf("(");
    
    print_expression(p->tp.a[0]);
    printf(")\n");
    print_statement(p->tp.a[1]);
    printf("ELSE%*s",1,"");
    print_statement(p->tp.a[2]);
    printf("\n");
  }else if(p->n.op== STATEMENT_WHILE){
    printf("WHILE (");
    print_expression(p->tp.a[0]);
    printf(")\n");
    print_statement(p->tp.a[1]);
    printf("\n");
  }else if(p->n.op== STATEMENT_RETURN){
    printf("\n");
    printf("("); 
    printf("RETURN%*s",1,"");
    print_expression(p->tp.a[0]);
    printf(";)\n");

  }else{
    // printf("print_compound_statement");
    print_compound_statement(p);
  }
}
//10
void print_compound_statement(tree p){
  printf("10compound_statement\n");
  if(p->n.op== COMP_STATE){
    printf("{");
    if(p->tp.a[0]!=NULL){
      // printf("opt_decl !=NULL\n");
      print_opt_declaration_list(p->tp.a[0]);
    }else{
      printf("opt_decl =NULL\n");
    }
    if (p->tp.a[1]!=NULL){
      // printf("opt_state!=NULL\n");
 
      print_opt_statement_list(p->tp.a[1]);
    }else{
      // printf("opt_state =NULL\n");
    }
    printf("}\n");
   
  }else{
    printf("error:compound statement");
  }
}
//10-2
void print_opt_declaration_list(tree p){

  printf("10-1opt_declaration_list\n");
  // printf("%d\n",p->tp.a[0]) ;

  if(p!=NULL){
    //printf("!=NULL  opt_declaration_list\n");
    print_declaration_list(p);
  }else{
    printf("NULL   opt_declaration_list\n");}
}
//10-3
void print_opt_statement_list(tree p){
  printf("opt_statement_list\n");
  if(p!=NULL){
    // printf("!=NULL opt_statement_list\n");
    print_statement_list(p);
  }else{
    // printf("NULL opt_statement_list\n");  
    /* print_statement_list(p/\*->tp.a[1]*\/); */
    
  }
}

 
//11
void print_declaration_list(tree p){
  printf("11declaration_list\n");
  if(p->n.op!= CONS){
    print_declaration(p);
   
  }else{
    print_declaration_list(p->tp.a[1]);
       
    print_declaration(p->tp.a[0]);
    
  }
}
//12
void print_statement_list(tree p){
  printf("12statement_list\n");
  if(p->n.op!= CONS){
    // printf("!=cons\n");
    print_statement(p);
   
  }else{//printf("else");
    print_statement_list(p->tp.a[0]);
    print_statement(p->tp.a[1]);
  
  }
}
//13
void print_expression(tree p){
  printf("13expression\n");
  if(p->n.op!= CONS){
    print_assign_expr (p);
  
  }else{
    print_expression (p->tp.a[0]);
    printf(", ");
    print_assign_expr (p->tp.a[1]);
    
  }
}
//14
void print_assign_expr(tree p){
  printf("14assign_expr\n");
  if(p->n.op==ASSIGN){
    printf("IDENTIFIER");
    printf("%s",(p->tp.a[0])->tk.name);
   
 
    
    print_assign_expr(p->tp.a[1]);
 
   
  } else{
    print_logical_OR_expr(p);
   
  }
}
//15
void print_logical_OR_expr(tree p){
  // printf("15logical_OR_expr\n");
  if(p->n.op!= OR){
    print_logical_AND_expr(p);
   
  }else{
    print_logical_OR_expr(p->tp.a[0]);
    printf("||");
    print_logical_AND_expr(p->tp.a[1]);
  
   
  }
}
//16
void print_logical_AND_expr(tree p){
  //  printf("16logical_AND_expr\n");
  if(p->n.op!= AND){
    print_equality_expr(p);
   
  }else{
    print_logical_AND_expr(p->tp.a[0]);
    printf("&&");
    print_equality_expr(p->tp.a[1]);
   
   
  }
}
//17
void print_equality_expr(tree p){
  // printf("17equality_expr\n");
  if(p->n.op==NOT_EQUAL_TO ){
    print_equality_expr(p->tp.a[0]);
    printf("!=");
    print_relational_expr(p->tp.a[1]);
   



  }else if(p->n.op==EQUAL_TO ){
    print_equality_expr(p->tp.a[0]);
    printf("==");
    print_relational_expr(p->tp.a[1]);

  }else{




    print_relational_expr(p);
  
    
  }
}
//18
void print_relational_expr(tree p){
  //  printf("18relational_expr");
  if(p->n.op== GREATER_THAN){
    print_relational_expr(p->tp.a[0]);
    printf("GREATER_THAN%*s",1,"");
    print_add_expr(p->tp.a[1]);
  
  }else if(p->n.op==LESS_THAN){
    print_relational_expr(p->tp.a[0]);
    printf("LESS_THAN%*s",1,"");
    print_add_expr(p->tp.a[1]);
  }else if(p->n.op==GREATER_THAN_EQUAL){
    print_relational_expr(p->tp.a[0]);
    printf("GREATER_THAN_EQUAL%*s",1,"");
    print_add_expr(p->tp.a[1]);
  }else if(p->n.op== LESS_THAN_EQUAL){
    print_relational_expr(p->tp.a[0]);
    printf(" LESS_THAN_EQUAL%*s",1,"");
    print_add_expr(p->tp.a[1]);
 
   
  }else{
    print_add_expr(p);
  }
}

//19
void print_add_expr(tree p){
  //  printf("19add_expr");
  if(p->n.op== ADD){
   
    printf("(+%*s",1," ");
    print_add_expr(p->tp.a[0]);
    printf("%*s",1,"");
    print_mult_expr(p->tp.a[1]);
    printf(")");
  } else if(p->n.op== SUB){
    print_add_expr(p->tp.a[0]);
    printf("-");
    print_mult_expr(p->tp.a[1]);
   
  }else{
    print_mult_expr(p);}
  
}

//20
void print_mult_expr(tree p){
  // printf("20mult_expr");
  if(p->n.op== MULTI){
    print_mult_expr(p->tp.a[0]);
    printf("*");
    print_unary_expr(p->tp.a[1]);
    // printf("\n"); 
  }else if(p->n.op== DIV){
    print_mult_expr(p->tp.a[0]);
    printf("/");
    print_unary_expr(p->tp.a[1]);
  
  }else{
    print_unary_expr(p);
  }}
//21
void print_postfix_expr(tree p){
  // printf("21postfix_expr");
  if(p->n.op== POSTFIX){
    //printf("IDENTIFIER");
    printf("FCALL:%s",(p->tp.a[0])->tk.name);//make_token_node
    print_parm(p->tp.a[0]);
    printf("(");
    print_argument_expression_list_opt(p->tp.a[1]);
    printf(")\n");
 
    
  }else{
    print_primary_expr(p);
    
  }
}

//21-2
void print_argument_expression_list_opt(tree p){
  if(p==NULL){
  }  else{
    print_argument_expression_list(p);  
  }

}
//22
void print_unary_expr(tree p){
  printf("22unary_expr");
  if(p->n.op== SUB){

    printf("SUB%*s",1,"");
    print_unary_expr(p->tp.a[0]);  
  }else{
    print_postfix_expr(p);
   
  }

}
//23
void print_primary_expr(tree p){
  // printf("23primary_expr");
  if(p->tk.op==TOKEN){
    //  printf("IDENTIFIER");
    print_parm(p);
    // printf("%s",p->tk.name);//make_token_node
  }else if(p->c.op ==CONSTANT){
    printf("make_constant_node");
    printf("%d",p->c.v);//make_constant_node
  
  }else{
    printf("(");
    print_expression(p);
    printf(")\n");

  }


}


//24
void print_argument_expression_list(tree p){
  // printf("24argument_expression_list");
  if(p->n.op!= CONS){
    print_assign_expr(p);
   
  }else{
    print_assign_expr(p->tp.a[0]);
    printf(",");
    print_argument_expression_list(p->tp.a[1]);
 
  
  }
}


