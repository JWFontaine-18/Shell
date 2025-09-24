#ifndef PIPING_H
#define PIPING_H

#include "lexer.h"

void cleanProcesses(int* processes , int processCount , int** pipes , int pipesCount);

void createChildProcesses(char** commands , int numCommands , char*** args);

int** createPipes( int numProcesses);

void pipeCommands(tokenlist* tokens);

#endif