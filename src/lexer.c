#include "lexer.h"
#include "prompt.h"
#include "path_search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <unistd.h>          
#include <sys/wait.h>        
#include <errno.h>           

int main()
{
	prompt();
	while (1) {
		printf("> ");

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}

		expand_env_var(tokens);

		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}

        //turns ~ -> $HOME and ~/dir -> $HOME/dir.
        expand_tilde(tokens);

        //Show tokens again after tilde expansion (debugging stuff)
        for (int i = 0; i < tokens->size; i++) { 
            printf("token %d: (%s)\n", i, tokens->items[i]); 
        }


        /* [Part 4 - ADDED] PATH search
        If the command is not a built-in and has no /, try to
        resolve it against $PATH and show what would be executed. */
        if (tokens->size > 0 && tokens->items[0]) {               
            const char *cmd = tokens->items[0];                   
            if (!is_builtin(cmd)) {                               
                char *resolved = search_path_for_command(cmd);    
                if (resolved) {                                   
                    printf("[resolve] %s\n", resolved);           
                    free(resolved);                               
                } else {
                    // If it's a built-in we print nothing here; if not found in PATH, say so.
                    if (strchr(cmd, '/') == NULL)                 
                        printf("[resolve] command not found: %s\n", cmd); 
                }
            }
        }

        //External command execution
         if (tokens->size > 0 && tokens->items[0]) {                     
            const char *cmd = tokens->items[0];                         

            if (!is_builtin(cmd)) {                                    
                char *prog_path = search_path_for_command(cmd);         

                if (!prog_path) {                                       
                    if (strchr(cmd, '/') == NULL)                        
                        fprintf(stderr, "command not found: %s\n", cmd); 
                    // If user provided a direct path that isn't executable,
                    // search_path_for_command returns NULL as well.       
                } else {
                    pid_t pid = fork();                                  
                    if (pid < 0) {
                        perror("fork");                                  
                    } else if (pid == 0) {
                        // Child: replace process image with the program  
                        execv(prog_path, tokens->items);                 
                        // If execv returns, it failed                    
                        perror("execv");                                 
                        _exit(127);                                   
                    } else {
                        // Parent: wait for the child to finish           
                        int status = 0;                                 
                        if (waitpid(pid, &status, 0) < 0)                
                            perror("waitpid");                           
                    }
                    free(prog_path);                                    
                }
            }
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

