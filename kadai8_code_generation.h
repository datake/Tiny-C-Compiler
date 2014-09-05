#ifndef _CODE_
#define _CODE_

//アセンブリコードを保持する構造体
struct code{
char *cd;
struct code *next;
};
struct code *emit(char*,char*,char*,char*);

extern char  *make_label();
extern char  *make_label_return();

extern void print_code();
extern void emit_program(tree p);//1
extern void emit_external_declaration(tree p);//2
extern void emit_declaration(tree p);//3
extern void emit_declarator_list(tree p);//4
extern void emit_declarator(tree p);//5
extern void emit_function_definition(tree p);//6
extern void emit_parameter_type_list(tree p);//7
extern void emit_parameter_type_list_opt(tree p);//7

extern void emit_parameter_declaration(tree p);//8
extern void emit_statement(tree p);//9
extern void emit_compound_statement(tree p);//10
extern void emit_declaration_list(tree p);//11
extern void emit_opt_statement_list(tree p);//12
extern void emit_opt_declaration_list(tree p);//11
extern void emit_statement_list(tree p);//12
extern void emit_expression(tree p);//13
extern void emit_assign_expr(tree p);//14
extern void emit_logical_OR_expr(tree p);//15
extern void emit_logical_AND_expr(tree p);//16
extern void emit_equality_expr(tree p);//17
extern void emit_relational_expr(tree p);//18
extern void emit_add_expr(tree p);//19
extern void emit_mult_expr(tree p);//20
extern void emit_postfix_expr(tree p);//21
extern void emit_unary_expr(tree p);//22
extern void emit_primary_expr(tree p);//23
extern void emit_argument_expression_list(tree p);//24

extern void emit_argument_expression_list_opt(tree p);

#endif
