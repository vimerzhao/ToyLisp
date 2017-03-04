#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048

#ifdef _WIN32
#include <string.h>

static char buffer[BUFFER_SIZE];

/* Fake readline function */
char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, BUFFER_SIZE, stdin);
    char* cpy = malloc(strlen(buffer) + 1);
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
