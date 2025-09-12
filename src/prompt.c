#include "../include/prompt.h"
#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

void expand_env_var(tokenlist * tokens) {
    for (int i = 0; i < tokens->size; i++) {
        char *token = tokens->items[i]; // pointer to the current token

        if (token[0] == '$') {  // Check if it starts with '$'
            char * env_var = &token[1]; // pointer to the string after the '$' (IE - USER)

            char * value = getenv(env_var); // get the environment of the environment variable 

            if(value != NULL) {
                free(tokens->items[i]);

                tokens->items[i] = malloc(strlen(value) + 1);

                strcpy(tokens->items[i], value);
            }
        }
}

}

void prompt() {
    char *env = getenv("PWD");
    char *name = getenv("USER"); 
    char *machine = getenv("MACHINE");

    printf("%s@%s:%s> ", name, machine, env);

    char input[1024];
    fgets(input, sizeof(input), stdin);
}