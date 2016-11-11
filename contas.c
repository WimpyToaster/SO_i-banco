#include "contas.h"

int FLAG_SIM = 0;
int contasSaldos[NUM_CONTAS];


int contaExiste(int idConta) {
    return (idConta > 0 && idConta <= NUM_CONTAS);
}

void inicializarContas() {
    int i;
    for (i=0; i<NUM_CONTAS; i++)
        contasSaldos[i] = 0;
}

int debitar(int idConta, int valor) {
    atrasar();
    if (!contaExiste(idConta))
        return -1;
    if (contasSaldos[idConta - 1] < valor)
        return -1;
    atrasar();
    contasSaldos[idConta - 1] -= valor;
    return 0;
}

int creditar(int idConta, int valor) {
    atrasar();
    if (!contaExiste(idConta))
        return -1;
    contasSaldos[idConta - 1] += valor;
    return 0;
}

int lerSaldo(int idConta) {
    atrasar();
    if (!contaExiste(idConta))
        return -1;
    return contasSaldos[idConta - 1];
}


/****************************************
* Muda a flag FLAG para 1               *
* NOTA: Usada no tratamento de sinais   *
*       da funcao simular               *
****************************************/
void mudaFlag() {
    FLAG_SIM = 1;
}

void activaSinal() {
    signal (SIGUSR1, mudaFlag);
}

/****************************************
* Simula o valor de conta ao final de   *
* "numAnos"                             *
* Recebe: Numero de anos a simular      *
* NOTA: A comentario esta implementada  *
*       uma maneira alternativa de      *
*       tratar as simuacoes (ficheiros) *
****************************************/
void simular(int numAnos) {
    if (numAnos < 0) {
        printf("Numero de anos nao pode ser negativo!\n");
        return;
    }

    //FILE * file;
    //char path[20];
    //sprintf(path, "temp%d.txt", getpid());
    //file = fopen(path, "w");
    
    int i,j, contas[NUM_CONTAS];

    for(i=0; i <= numAnos; i++){
        if(FLAG_SIM) {
                printf("Simulacao terminou por sinal\n");
                exit(EXIT_SUCCESS);
        }
        //fprintf(file, "SIMULACAO: Ano %d\n=================\n", i);
        printf("SIMULACAO: Ano %d\n=================\n", i);
        for(j=0; j < NUM_CONTAS; j++) {
            if(i == 0) {
                contas[j] = lerSaldo(j+1);
                printf("Conta %d, Saldo %d\n", j+1, contas[j]);
                //fprintf(file, "Conta %d, Saldo %d\n", j+1, contasSaldos[j]);
            }
            else {
                contas[j] = MAX(FORMULA(contas[j]), 0);
                printf("Conta %d, Saldo %d\n", j+1, contas[j]);
                sleep(1);
                //fprintf(file, "Conta %d, Saldo %d\n", j+1, contas[j]);
            }
            
        }
        printf("\n");
    }
    //printf("ficheiro %s acabado\n", path);
    //fclose(file);
}