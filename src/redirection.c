#include "lexer.h"
#include "path_search.h"
#include "redirection.h"
#include <sys/types.h>
#include <sys/wait.h>
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
        // if token is '<', input file stores next element 
        if(strcmp(tokens->items[i], "<") == 0){
            inputFile = tokens->items[i + 1];
            i++;
        }
        // if token is '>', output file stores next element
        else if (strcmp(tokens->items[i], ">")==0){
            outputFile = tokens->items[i +1];
            i++;
        }
        else {
            argv[argc] = tokens->items[i];
            argc++;
        }
    }
    argv[argc] = NULL;

    pid_t pid = fork();
    //if in child process
    if(pid == 0){ 
        // if inpitfile not null, dup it do stdin
        if(inputFile) {
            int fd = open(inputFile, O_RDONLY );
            if(fd < 0){
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        // if outputfile not null, dup it do stdout
        if(outputFile){
            int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            if(fd < 0){
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        // get path for command to pass into execv
        char * path = search_path_for_command(argv[0]);
        if(!path){
            free(argv);
            return -1;
        }

        argv[0] = path;
        execv(argv[0], argv);
    // wait for child process if in parent 
    } else if (pid > 0){
        wait(NULL);
    }else{
        exit(1);
    }

    free(argv);
    return 0;

}

