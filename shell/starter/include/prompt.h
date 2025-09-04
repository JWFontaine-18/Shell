#ifndef PROMPT_H
#define PROMPT_H
#include "lexer.h"

void prompt();

void expand_env_var(tokenlist * tokens);

#endif
