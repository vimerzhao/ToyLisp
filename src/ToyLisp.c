#include "ToyLisp.h"

/* Main */
int main(int argc, char** argv) {

    Number = mpc_new("number");
    Symbol = mpc_new("symbol");
    String = mpc_new("string");
    Comment= mpc_new("comment");
    Sexpr  = mpc_new("sexpr");
    Qexpr  = mpc_new("qexpr");
    Expr   = mpc_new("expr");
    ToyLisp  = mpc_new("ToyLisp");

    mpca_lang(MPCA_LANG_DEFAULT,
            "                                                   \
            number : /-?[0-9]+/ ;                               \
            symbol : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;         \
            string : /\"(\\\\.|[^\"])*\"/ ;                     \
            comment : /;[^\\r\\n]*/ ;                           \
            sexpr  : '(' <expr>* ')' ;                          \
            qexpr  : '{' <expr>* '}' ;                          \
            expr   : <number> | <symbol> | <sexpr>              \
                   | <qexpr>  | <string> | <comment>;           \
            ToyLisp  : /^/ <expr>* /$/ ;                          \
            ",
            Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, ToyLisp);

    lenv* e = lenv_new();
    lenv_add_builtins(e);


    /* Load Library */
    lval* std_lib = lval_add(lval_sexpr(), lval_str("lib/standard_lib.tl"));
    builtin_load(e, std_lib);

    if (1 == argc) {
        puts("ToyLisp Version0.7");
        puts("Press Ctrl+C to Exit\n");
        while (1) {

            char* input = readline("ToyLisp> ");
            add_history(input);

            mpc_result_t r;
            if (mpc_parse("<stdin>", input, ToyLisp, &r)) {
                lval* x = lval_eval(e, lval_read(r.output));
                lval_println(x);
                lval_del(x);
                mpc_ast_delete(r.output);
            } else {
                mpc_err_print(r.error);
                mpc_err_delete(r.error);
            }

            free(input);

        }
    }
    if (argc >= 2) {

        /* loop over each supplied filename (starting from 1) */
        for (int i = 1; i < argc; i++) {

            /* Argument list with a single argument, the filename */
            lval* args = lval_add(lval_sexpr(), lval_str(argv[i]));

            /* Pass to builtin load and get the result */
            lval* x = builtin_load(e, args);

            /* If the result is an error be sure to print it */
            if (x->type == LVAL_ERR) { lval_println(x); }
            lval_del(x);
        }
    }
    lenv_del(e);

    mpc_cleanup(8, Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, ToyLisp);

    return 0;
}
