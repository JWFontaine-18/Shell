#ifndef REDIRECT_H
#define REDIRECT_H

struct redirectCommand;
void freeRedirectCommand(struct redirectCommand* comm);
void cleanRedirect(struct redirectCommand* redirects);
int redirectInput(char** input , int inputLength);
struct redirectCommand* parseInput(char** input , int inputLength , int numCommands);

#endif