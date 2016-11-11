/*
// Projeto SO - exercicio 1, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2016-17
*/

#include "commandlinereader.h"
#include "comandos.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_AGORA "agora"
#define COMANDO_TRANSFERENCIA "transferir"

//

#define MAXPROCC 20
#define MAXARGS 4
#define BUFFER_SIZE 100
#define TH_BUFFER 3



int main (int argc, char** argv) {

    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    
    inicializarContas();
    
    if(initComandos() == -1) {
        printf("Inicializacao dos comandos correu mal\n");
        return 0;
    }

    int curProcess = 0;

    printf("Bem-vinda/o ao i-banco\n\n");

    

    while (1) {
        int numargs;
        numargs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);

        /* EOF (end of file) do stdin ou comando "sair" */
                
        if (numargs < 0 ||
	        (numargs > 0 && numargs == 1 && (strcmp(args[0], COMANDO_SAIR)==0)))  {
            int i;
            for (i = 0; i < MAX_THREADS; i++)
                criaComando(OP_SAIR, 0, 0, 0);
            closeThreads(NORMAL);
            exit(EXIT_SUCCESS);
        }
        
        else if (numargs > 0 && numargs == 2 && (strcmp(args[0], COMANDO_SAIR) == 0) && (strcmp(args[1], COMANDO_AGORA) == 0)) {
            int i;
            for (i = 0; i < MAX_THREADS; i++)
                criaComando(OP_SAIR, 0, 0, 0);
            closeThreads(NOFORCE);
            exit(EXIT_SUCCESS);
        }

       

        else if (numargs == 0)
            /* Nenhum argumento; ignora e volta a pedir */
            continue;

        /* Debitar */
        else if (strcmp(args[0], COMANDO_DEBITAR) == 0) {
            int idConta, valor;
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_DEBITAR);
	           continue;
            }

            idConta = atoi(args[1]);
            valor = atoi(args[2]);

            criaComando(OP_DEBITAR, idConta, 0, valor);
        }

        /* Creditar */
        else if (strcmp(args[0], COMANDO_CREDITAR) == 0) {
            int idConta, valor;
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_CREDITAR);
                continue;
            }

            idConta = atoi(args[1]);
            valor = atoi(args[2]);

            criaComando(OP_CREDITAR, idConta, 0, valor);
        }

        /* Ler Saldo */
        else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
            int idConta;

            if (numargs < 2) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
                continue;
            }
            idConta = atoi(args[1]);
            criaComando(OP_LER_SALDO, idConta, 0, 0);
        }

        /* Simular */
        else if (strcmp(args[0], COMANDO_SIMULAR) == 0) {

            int i;
            if ( ( ( i = atoi(args[1]) ) == 0 && strcmp(args[1], "0") ) ) {
                printf("Simular apenas aceita numeros nao negativos %s\n", args[1]);
                continue;
            }
            if(curProcess > MAXPROCC){
                printf("Maximo de processos atingido\n");
                continue;
            }

            curProcess++;
            criaComando(OP_SIMULAR, 0, 0, i);
        }

        //Tranferencia
        else if (strcmp(args[0], COMANDO_TRANSFERENCIA) == 0) {
            int idConta, idConta2, valor;
            if (numargs < 4) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_TRANSFERENCIA);
               continue;
            }

            idConta = atoi(args[1]);
            idConta2 = atoi(args[2]);
            valor = atoi(args[3]);

            criaComando(OP_TRANSFERIR, idConta, idConta2, valor);
        }

        else {
          printf("Comando desconhecido. Tente de novo.\n");
        }
    }
}