#ifndef LL_H
#define LL_H

#include <sys/types.h>

#define FORCE 0
#define NOFORCE 1
#define NORMAL 2



typedef struct node * Node;

Node createNode(Node, pid_t);
Node deleteNode(Node, pid_t);
void closeNodes(Node);
pid_t runNodes(Node);
void stopNodes(Node, int);
void listNodes(Node);
const char* TODO(int status);

#endif