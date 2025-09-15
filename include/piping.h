#ifndef PIPING_H
#define PIPING_H

void cleanProcesses(int* processes , int processCount , int** pipes , int pipesCount);

int createChildProcesses(char* commands , int numCommands , char** args);

int** createPipes( int numProcesses);

#endif