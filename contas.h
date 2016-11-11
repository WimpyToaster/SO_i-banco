/*
// Operações sobre contas, versao 1
// Sistemas Operativos, DEI/IST/ULisboa 2016-17
*/

#ifndef CONTAS_H
#define CONTAS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define NUM_CONTAS 10
#define TAXAJURO 0.1
#define CUSTOMANUTENCAO 1
#define FORMULA(A) ( (A * (1 + TAXAJURO) - CUSTOMANUTENCAO) )
#define MAX(X,Y) ((X > Y) ? X : Y)
#define atrasar() sleep(ATRASO)

#define ATRASO 1

void inicializarContas();
int contaExiste(int idConta);
int debitar(int idConta, int valor);
int creditar(int idConta, int valor);
int lerSaldo(int idConta);
void simular(int numAnos);


#endif
