#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

int main(int argc, char const* argv[])
{
    puts("ToyLisp Version 0.1");
    puts("Press Ctrl+c to Exit\n");

    while (1) {
        char* input = readline("ToyLisp>");

        add_history(input);

        printf("%s\n", input);

        free(input);
    }
    return 0;
}
