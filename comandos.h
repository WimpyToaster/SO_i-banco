#ifndef _COM_
#define _COM_

#include "LinkedList.h"
#include "contas.h"
#include "wrappers.h"


#define OP_DEBITAR 0
#define OP_CREDITAR 1
#define OP_LER_SALDO 2
#define OP_SIMULAR 3
#define OP_SAIR 4
#define OP_TRANSFERIR 5

#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_AGORA "agora"
#define COMANDO_TRANSFERENCIA "transferir"

#define MAX_THREADS 3
#define MAX_COMANDOS (MAX_THREADS * 2)

typedef struct com comando_t;

int initComandos();
void termComandos(int force);
void closeThreads(int force);
void * buscaComando();
void trataComando(comando_t com, int * curProcess);
void criaComando(int command, int conta, int conta2, int valor);
void aumentaContador();
void decrementaContador();


#endif