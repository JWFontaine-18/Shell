#include "command_utils.h"
#include "redirect.h"
#include "stdlib.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//allows redirect commands to be in defined format
struct redirectCommand {
    char* command;
    char* infilePath; //freed by caller - derived from caLLLER
    char* outfilePath; //freed by caller - derived from caller
};

void freeRedirectCommand(struct redirectCommand* comm) {
    if(comm->command != NULL) free(comm->command);
}

//free allocated memory
void cleanRedirect(struct redirectCommand** redirects , int redirectsLength) {

    for(int i = 0 ; i < redirectsLength ; i++) {
        freeRedirectCommand(redirects[i]);
    }

    free(redirects);

}

//you can just pass the direct command sequence here - we will parse it
//we do assume it has at least ONE I/O redirect in it tho
//we will validate command structure
int redirectInput( char** input , int inputLength) {

    int numCommands = 1; //TODO make accurate
    struct redirectCommand** redirects = parseInput(input , inputLength , numCommands);

    for( int i = 0 ; i < numCommands ; i++) { //TODO: generalize and make work

        struct redirectCommand* command = redirects[i];

        char** args = (char**) malloc(sizeof(char**) * inputLength);
        int argCount = 0;

        args[0] = (char*) malloc(sizeof(char) * strlen(input[0]) + 1);
        strcpy(args[0] , input[0]);

        for(int j = 0 ; j < inputLength ; j++) { //TODO: BAD

            if(j == 0) {
                argCount++;
                continue;
            }

            if(strcmp(input[j], ">") == 0 || strcmp(input[j] , "<") == 0 || strcmp(input[j] , "|") == 0) {
                j++;
                continue;
            }

            args[argCount] = (char*) malloc(sizeof(char) * strlen(input[j]) + 1);

            args[argCount] = strcpy(args[argCount] , input[j]);

            argCount++;
        }

        args[argCount] = NULL;
        args = realloc(args , ( argCount * sizeof(char**) ) + 1);

        int procId = fork();

        if(procId < 0) {
            printf("fatal error executing command");
            exit(1);
        }
        else if(procId == 0 ) { //inside child

            int infileDesc = -1;
            int outfileDesc = -1;

            if(command->infilePath != NULL) {

                infileDesc = open(command->infilePath , O_RDONLY); //TODO: error if file does not exist

                //TODO: error check

                dup2(infileDesc , STDIN_FILENO);

                
            }

            if(command->outfilePath != NULL) {
                
                outfileDesc = open(command->outfilePath , O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );

                //TODO: error check

                dup2(outfileDesc , STDOUT_FILENO);

            }

            execv(command->command , args);
            
        }
        else {
            waitpid(procId , NULL , 0); //TODO: refactor for background execution
            cleanRedirect(redirects , numCommands); //free mem
            for(int c = 0 ; c < argCount ; c++) {
                printf("%s\n", args[c]); 
            }
            for(int c = 0 ; c < argCount ; c++) {
                free(args[c]);
            }

            free(args);
        }
        
    }

    return 1; //TODO: adjust for background proccessing
    
}

//expects commands to be in order of input
//expects first element in input to be a command
struct redirectCommand** parseInput(char** input , int inputLength , int numCommands) {

    //we use an array of structs as a placeholder in case we need to support multiple commands
    struct redirectCommand** redirectCommands = (struct redirectCommand**) malloc(sizeof( struct redirectCommand* ) * numCommands);
    char* firstCommandPath;


    for( int i = 0 ; i < numCommands ; i++) { //initialize commands with empty structs
        redirectCommands[i] = (struct redirectCommand*) malloc(sizeof(struct redirectCommand));
        redirectCommands[i]->command = NULL;
        redirectCommands[i]->infilePath = NULL;
        redirectCommands[i]->outfilePath = NULL;
    }

    if(!command_exists(input[0])) {
        printf("fatal parse error"); //we should never get here, it is expected that input contains a starting command when passed
        exit(1);
    }


    firstCommandPath = get_command_path(input[0]); //first argument should be a command

    //TODO: generalize
    redirectCommands[0]->command = firstCommandPath;

    for(int i = 0 ; i < inputLength ; i++) { //assumes only one command in the pipe for now TODO: expand
        
        if( i == 0){
            continue;
        }


        if( strcmp(input[i], ">") == 0 || strcmp(input[i] , "<") == 0 ) {

            if(i + 1 >= inputLength) { //error state
                return NULL;
            }

            if(strcmp(input[i], ">") == 0) { //TODO: logic here breaks if i add a > || < at the end of input
                
                i++;

                redirectCommands[0]->outfilePath = input[i]; //TODO: make support multiple commands

                i++;
            }
            else if(strcmp(input[i] , "<") == 0) {

                i++;

                redirectCommands[0]->infilePath = input[i]; //TODO: make support multiple commands

                i++;
            }
        }
        else {
        }
    }

    return redirectCommands;
}

