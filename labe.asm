 5 declarator
6function_fefinition/count_parm:0
10 compound_statement

     5 declarator
4declaration_list
declaratorでmake_decl
allocate_loc/top_alloc:-4,last_alloc:-4make_decl/tk.offset:-4,tk.kinf:1 5 declarator
declaratorでmake_decl
allocate_loc/top_alloc:-8,last_alloc:-8make_decl/tk.offset:-8,tk.kinf:13declaration

      9 statement

      9 statement

      10 compound_statement

           9 statement
   9 statement
9 statement

    9 statement

9 statement
6 function_definition
2external_declaration


 5 declarator
6function_fefinition/count_parm:0
10 compound_statement

    argument_expression/count_parm:[1] argument_expression/count_parm::2GS:chk
9 statement

6 function_definition
2external_declaration
1program

1program
1program
2external_declaration
6function_definition
10compound_statement
10-1opt_declaration_list
11declaration_list
3declaration
4declarator_list
4declarator_list
5declarator
5declarator
allocate_loc/top_alloc:-4,last_alloc:-4opt_statement_list
12statement_list
12statement_list
12statement_list
9statement
13expression
14assign_expr
14assign_expr
15logical_OR_expr
16logical_AND_expr
n.op:299
17equality_expr
else18relational_expr19add_expr20mult_expr22unary_expr21postfix_expr23primary_expr9statement
13expression
14assign_expr
14assign_expr
15logical_OR_expr
16logical_AND_expr
n.op:299
17equality_expr
else18relational_expr19add_expr20mult_expr22unary_expr21postfix_expr23primary_expr9statement
13expression
14assign_expr
15logical_OR_expr
16logical_AND_expr
n.op:300
17equality_expr
else18relational_expr19add_expr20mult_expr22unary_expr21postfix_expr23primary_expr9statement
10compound_statement
10-1opt_declaration_list
opt_statement_list
12statement_list
9statement
13expression
14assign_expr
15logical_OR_expr
16logical_AND_expr
n.op:300
17equality_expr
else18relational_expr19add_expr20mult_expr22unary_expr21postfix_expr23primary_expr9statement
13expression
14assign_expr
15logical_OR_expr
16logical_AND_expr
n.op:299
17equality_expr
else18relational_expr19add_expr20mult_expr22unary_expr21postfix_expr23primary_expr2external_declaration
6function_definition
10compound_statement
10-1opt_declaration_list
opt_statement_list
12statement_list
9statement
13expression
14assign_expr
15logical_OR_expr
16logical_AND_expr
n.op:295
17equality_expr
else18relational_expr19add_expr20mult_expr22unary_expr21postfix_expr

postfix bigin
emit_postfix_expression/parm_num:0,chk24argument_expression_list14assign_expr
15logical_OR_expr
16logical_AND_expr
n.op:299
17equality_expr
else18relational_expr19add_expr20mult_expr22unary_expr21postfix_expr23primary_expr24argument_expression_list14assign_expr
15logical_OR_expr
16logical_AND_expr
n.op:295
17equality_expr
else18relational_expr19add_expr20mult_expr22unary_expr21postfix_expr

postfix bigin
emit_postfix_expression/parm_num:0,lab


emit_postfix_expression/parm_num:0


emit_postfix_expression/parm_num:0

emit_argument_expression/parm_num:2

emit_argument_expression/parm_num:2


emit_argument_exp_list/parm_num:2


emit_postfix_expression/parm_num:0


emit_postfix_expression/parm_num:2	GLOBAL	lab
lab:	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 1
	mov	[ebp-4], eax
	mov	eax, 0
	mov	[ebp-8], eax
	mov	eax, [ebp-4]
	cmp	eax, 0
	je	L1
	mov	eax, [ebp-8]
	cmp	eax, 0
	je	L3
	mov	eax, 0
	jmp	R1
L3:
L1:
R1:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 2
	push	eax
	call	lab
	add	esp, 0
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
R2:	mov	esp, ebp
	pop	ebp
	ret
