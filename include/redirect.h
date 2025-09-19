#ifndef REDIRECT_H
#define REDIRECT_H

void cleanRedirect();
int redirectInput(const char** input , int inputLength);
struct redirectCommnad** parseInput(char** input);

#endif