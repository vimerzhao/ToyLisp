#ifndef TOYLISP_H_
#define TOYLISP_H_
#include "mpc.h"

/* cross-platform code */
#ifdef _WIN32
static char buffer[2048];
char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}
void add_history(char* unused) {}
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

/* Parser Declariations */
mpc_parser_t* Number;
mpc_parser_t* Symbol;
mpc_parser_t* String;
mpc_parser_t* Comment;
mpc_parser_t* Sexpr;
mpc_parser_t* Qexpr;
mpc_parser_t* Expr;
mpc_parser_t* ToyLisp;


/* Lisp Value Types */
enum {  LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_STR,
        LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

/* Forward Declarations */
struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;
typedef lval*(*lbuiltin)(lenv*, lval*);

/* Lisp Value Structure */
struct lval {
    int type;
    /* Basic */
    long num;
    char* err;
    char* sym;
    char* str;

    /* Function */
    lbuiltin builtin;
    lenv* env;
    lval* formals;
    lval* body;

    /* Expression */
    int count;
    lval** cell;
};

/* Lisp Environment */
struct lenv {
    lenv* par;
    int count;
    char** syms;
    lval** vals;
};

/******************lisp_value.c****************************/
lval* lval_num(long);
lval* lval_err(char* fmt, ...);
lval* lval_sym(char*);
lval* lval_str(char*);
lval* lval_fun(lbuiltin);
lval* lval_sexpr(void);
lval* lval_qexpr(void);
void lval_del(lval*);
lval* lval_copy(lval*);
int lval_eq(lval*, lval*);
lval* lval_call(lenv*, lval*, lval*);
lval* lval_lambda(lval*, lval*);
lval* lval_join(lval*, lval*);
char* ltype_name(int);
/**********************END*****************************/


/******************environment.c*****************************/
lenv* lenv_new(void);
lval* lenv_get(lenv*, lval*);
void lenv_put(lenv*, lval*, lval*);
lenv* lenv_copy(lenv*);
void lenv_def(lenv*, lval*, lval*);
void lenv_del(lenv*);
void lenv_add_builtin(lenv*, char*, lbuiltin);
void lenv_add_builtins(lenv*);
/**********************END*****************************/


/*****************format_output.c****************************/
void lval_print(lval*);
void lval_print_expr(lval*, char, char);
void lval_print_str(lval*);
void lval_println(lval*);
/**********************END*****************************/


/*****************format_input.c****************************/
lval* lval_add(lval*, lval*);
lval* lval_read_num(mpc_ast_t*);
lval* lval_read_str(mpc_ast_t*);
lval* lval_read(mpc_ast_t*);
/*********************END****************************/


/*****************evaluate.c****************************/
lval* lval_take(lval*, int);
lval* lval_eval_sexpr(lenv*, lval*);
lval* lval_eval(lenv*, lval*);
lval* lval_pop(lval*, int);
/*********************END****************************/

/*******************builtin.c****************************/
lval* builtin_list(lenv*, lval*);
lval* builtin_head(lenv*, lval*);
lval* builtin_tail(lenv*, lval*);
lval* builtin_eval(lenv*, lval*);
lval* builtin_join(lenv*, lval*);

lval* builtin_op(lenv*, lval*, char*);
lval* builtin_add(lenv*, lval*);
lval* builtin_sub(lenv*, lval*);
lval* builtin_mul(lenv*, lval*);
lval* builtin_div(lenv*, lval*);

lval* builtin_def(lenv*, lval*);
lval* builtin_var(lenv*, lval*, char*);
lval* builtin_put(lenv*, lval*);

lval* builtin_lambda(lenv*, lval*);
lval* builtin_ord(lenv*, lval*, char*);
lval* builtin_le(lenv*, lval*);
lval* builtin_ge(lenv*, lval*);
lval* builtin_lt(lenv*, lval*);
lval* builtin_gt(lenv*, lval*);
lval* builtin_cmp(lenv*, lval*, char*);
lval* builtin_eq(lenv*, lval*);
lval* builtin_ne(lenv*, lval*);

lval* builtin_if(lenv*, lval*);

lval* builtin_load(lenv*, lval*);
lval* builtin_print(lenv*, lval*);
lval* builtin_error(lenv*, lval*);
/*********************END****************************/

/* check macro */
#define LASSERT(args, cond, fmt, ...) \
    if (!(cond)) { lval* err = lval_err(fmt, ##__VA_ARGS__); lval_del(args); return err; }

#define LASSERT_TYPE(func, args, index, expect) \
    LASSERT(args, args->cell[index]->type == expect, \
            "Function '%s' passed incorrect type for argument %i. Got %s, Expected %s.", \
            func, index, ltype_name(args->cell[index]->type), ltype_name(expect))

#define LASSERT_NUM(func, args, num) \
    LASSERT(args, args->count == num, \
            "Function '%s' passed incorrect number of arguments. Got %i, Expected %i.", \
            func, args->count, num)

#define LASSERT_NOT_EMPTY(func, args, index) \
    LASSERT(args, args->cell[index]->count != 0, \
            "Function '%s' passed {} for argument %i.", func, index);

#endif
