#include "../include/lexer.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int redirection(tokenlist * tokens){
    char * inputFile = NULL;
    char * outputFile = NULL;

    char ** argv = malloc((tokens -> size + 1) * sizeof(char *));
    int argc = 0; // counter of commands in argv

    for(int i=0; i < tokens->size; i++){
        if(strcmp(tokens->items[i], "<") == 0){
            inputFile = tokens->items[i + 1];
            i++;
        }
        else if (strcmp(tokens->items[i], ">")==0){
            outputFile = tokens->items[i +1];
            i++;
        }
        else {
            argv[argc] = tokens->items[i];
            argc++;
        }
    }
}

