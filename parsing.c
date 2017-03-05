#include "redefine.h"
#include "mpc.h"

#define BUFFER_SIZE 2048

#ifdef _WIN32

static char buffer[BUFFER_SIZE];

/* Fake readline function */
string readline(string prompt) {
    fputs(prompt, stdout);
    fgets(buffer, BUFFER_SIZE, stdin);
    string cpy = malloc(strlen(buffer) + 1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char const* argv[]) {
    /* Create Some Parse */
    mpc_parser_t* Number    = mpc_new("number");
    mpc_parser_t* Operator  = mpc_new("operator");
    mpc_parser_t* Expr      = mpc_new("expr");
    mpc_parser_t* ToyLisp   = mpc_new("toylisp");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
      number   : /-?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' ;                  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      toylisp  : /^/ <operator> <expr>+ /$/ ;             \
    ", Number, Operator, Expr, ToyLisp);

    puts("ToyLisp Version 0.1");
    puts("Press Ctrl+c to Exit\n");

    while (1) {
        string input = readline("ToyLisp>");
        add_history(input);

        /* Attempt to Parse the user Input */
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, ToyLisp, &r)) {
            /* On Success Print the AST */
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            /* Otherwise Print the Error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    /* Undefine and Delete our Parsers */
    mpc_cleanup(4, Number, Operator, Expr, ToyLisp);
    return 0;
}