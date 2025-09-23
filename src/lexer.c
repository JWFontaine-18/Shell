#include "lexer.h"
#include "prompt.h"
#include "path_search.h"
#include "ExternalCommand.h"
#include "command_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <unistd.h>  
#include <redirect.h>      	  
#include <sys/wait.h>        
#include <errno.h>      
#include "piping.h"     

int main()
{

	while (1) {
		prompt();
		//printf("> ");

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();

		tokenlist *tokens = get_tokens(input);

		expand_env_var(tokens);

        //turns ~ -> $HOME and ~/dir -> $HOME/dir.
        expand_tilde(tokens);


        /* [Part 4 - ADDED] PATH search
        If the command is not a built-in and has no /, try to
        resolve it against $PATH and show what would be executed. */
        if (tokens->size > 0 && tokens->items[0]) {               
            const char *cmd = tokens->items[0];                   
            if (!is_builtin(cmd)) {                               
                char *resolved = search_path_for_command(cmd);    
                if (resolved) {                                            
                    free(resolved);                               
                } else {
                    // If it's a built-in we print nothing here; if not found in PATH, say so.
                    if (strchr(cmd, '/') == NULL)                 
                        printf("[resolve] command not found: %s\n", cmd); 
						continue;
                }
            }
        }

		int hasPiping = 0;
		int hasRedirects = 0;

		for(int i = 0 ; i < tokens->size ; i++) {
			if(strcmp(tokens->items[i] , "<" ) == 0 || strcmp(tokens->items[i] , ">") == 0) {
				hasRedirects = 1;
			}

			if(strcmp(tokens->items[i] , "|") == 0) {
				hasPiping = 1;
			}
		}

		if(hasPiping) {
			
			int numCommands = 0;
			char** commands = (char**) malloc(sizeof(char*) * tokens->size); //array of strings, each one is a command
			char*** args = (char***) malloc(sizeof(char**) * tokens->size); //array of array of strings in format execv expects

			for(int k = 0 ; k < tokens->size ; k++) {
				args[k] = NULL;
			}

			//get all commands
			for(int i = 0 ; i < tokens->size ; i++) {

				char* command;

				if( i == 0 ) {

					command = get_command_path(tokens->items[i]);
					
					commands[i] = command;

					numCommands++; 

					continue;
				}

				if(strcmp(tokens->items[i] , "|") == 0 ) { //next token is command assumed

					command = get_command_path(tokens->items[i + 1]);

					if(command == NULL) {
						printf("command not found...\n");
						goto end;
					}
					
					commands[numCommands] = command;
					numCommands++;
				}

			}

			//get arguments
			int startIndex = 0;

			for(int i = 0 ; i < numCommands ; i++){

				char** argArr = (char**) malloc(sizeof(char*) * tokens->size);

				for(int k = 0 ; k < tokens->size; k++){ //init to null
					argArr[k] = NULL;
				}

				for(int k = startIndex ; k < tokens->size ; k++) { //copy arguments

					if(k - startIndex == 0 ) { //use command path as first for execv
						argArr[k - startIndex] = commands[i];
						continue;
					}

					if(strcmp(tokens->items[k] , "|") == 0 ) { //stop when pipe
						startIndex = ++k;
						break;
					}

					argArr[k - startIndex] = tokens->items[k]; //copy reference to arg
				}

				args[i] = argArr; //arguments for command go in parrallel array
				
			}

			createChildProcesses(commands , numCommands , args);

			end:for(int i = 0 ; i < numCommands ; i++) {
				free(commands[i]);
			}

			free(commands);

			for(int i = 0 ; i < tokens->size ; i++) {
				free(args[i]); //all strings themselves are freed at end of loop
			}

			free(args);
			
		}
		else if (hasRedirects) {
			redirectInput(tokens->items , tokens->size);
		}
		else {
        	//External command execution
			ExternalCommand(tokens);
		}

		free(input);
		free_tokens(tokens);
		
	}

	return 0;
}

char *get_input(void) {
	char *buffer = NULL;
	int bufsize = 0;
	char line[5];
	while (fgets(line, 5, stdin) != NULL)
	{
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;
		buffer = (char *)realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;
		if (newln != NULL)
			break;
	}
	buffer = (char *)realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;
	return buffer;
}

tokenlist *new_tokenlist(void) {
	tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **)malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item) {
	int i = tokens->size;

	tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *)malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

tokenlist *get_tokens(char *input) {
	char *buf = (char *)malloc(strlen(input) + 1);
	strcpy(buf, input);
	tokenlist *tokens = new_tokenlist();
	char *tok = strtok(buf, " ");
	while (tok != NULL)
	{
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}
	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens) {
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}

