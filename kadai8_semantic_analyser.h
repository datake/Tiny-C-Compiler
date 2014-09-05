#ifndef _SEMANTIC_ANALYSER
#define _SEMANTIC_ANALYSER

//大域変数　宣言いらない？
//extern int cur_lev;
//extern token symtab;
extern int semnerrs;
extern int yylineno;

extern void error(char *fmt,...);
extern void warn (char *fmt,...);

extern tree make_decl(tree p);
extern tree make_fun_def(tree p);
extern tree make_parm_decl(tree p);
extern tree ref_var(tree p);
extern tree ref_fun(tree p);
extern int offset_();


extern void release_loc();
extern void release_one_loc();
extern int offset_parm();

extern void check_parm_num(int c,tree n);//変更
extern int count_argument_expression(tree n);
extern tree lookup_num(int num);

extern int count_parm;

extern int cur_lev;
extern token symtab;
extern int offset;

extern tree make_tuple(int, tree, tree, tree, tree);
extern tree make_token_node(char *);
extern tree make_constant_node(int);

extern void pop_sym();

extern tree lookup_sym(char *);
extern void globalize_sym(tree );
extern int allocate_loc();
extern int allocate_one_loc();


#endif
