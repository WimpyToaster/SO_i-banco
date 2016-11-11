#ifndef LL_H
#define LL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FORCE 0
#define NOFORCE 1
#define NORMAL 2


typedef struct node * Node;

Node createNode(Node, pid_t);
Node deleteNode(Node, pid_t);
void closeNodes(Node);
void runNodes(Node);
void stopNodes(Node, int);
void listNodes(Node);
const char* trataEstado(int status);

#endif