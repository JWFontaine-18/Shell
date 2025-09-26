#pragma once
#include "lexer.h"
#include "background.h"
#include <stdbool.h>

// returns 1 if first token is built-in we handle here (cd/exit) 
bool is_internal_command(tokenlist *tokens);

// runs built-in (cd/exit). returns -1 for exit, 0 otherwise 
int run_internal_command(tokenlist *tokens, backgroundProcs *procs);

// record a valid command.
void ic_history_record(const char *line);
