#include "comandos.h"

//Estrutura
struct com {
	int command;
	int idConta;
	int idConta2;
	int valor;
};

/****************************************
* Declaracao das variaveis globais      *
****************************************/
// Lista de tarefas
pthread_t th_list[MAX_THREADS];

// Buffer de comandos
comando_t com_buffer[MAX_COMANDOS];

// Semaforos
sem_t pode_criar, pode_tratar;

// Indices do buffer (produtor / consumidor)
int ptr_tratacoms, ptr_criacoms;

// Trincos (produtor / consumidor)
//         (Contas) 
pthread_mutex_t trinco_tr, trinco_cr, trinco_sims, trinco_contador;
pthread_mutex_t trinco_contas[NUM_CONTAS];

// Trincos condicionais (simulacoes)
pthread_cond_t pode_simular;


// Contador de operacoes a exeutar
int op_counter;


// Cabeca da lista ligada de processos
Node sims = NULL;  

int FLAG_TERM;// =  0 //Se esta a terminar processos

/****************************************
* Inicializa todas as variaveis globais *
****************************************/
int initComandos() {
	int i;
	// Inicilizacao das threads
	for (i=0; i < MAX_THREADS; i++) {
		if(pthread_create(&th_list[i], 0, buscaComando,(void*)com_buffer))
			return -1;
	}

	for(i=0; i < NUM_CONTAS; i++) {
		// Inicializacao dos trincos
		if(pthread_mutex_init(&trinco_contas[i], 0))
			return -1;
	}

	if(pthread_cond_init(&pode_simular, NULL))
		return -1;

	if(pthread_mutex_init(&trinco_cr, 0))
		return -1;
	if(pthread_mutex_init(&trinco_tr, 0))
		return -1;
	if(pthread_mutex_init(&trinco_sims, 0))
		return -1;
	if(pthread_mutex_init(&trinco_contador, 0))
		return -1;

	// Inicializacao dos semaforos
	if(sem_init(&pode_criar, 0, MAX_COMANDOS))
		return -1;

	if(sem_init(&pode_tratar, 0, 0))
		return -1;

	// Set dos indices a zer0
	ptr_tratacoms = 0; 
	ptr_criacoms =  0;

	// Set da flag a zer0
	FLAG_TERM = 0;

	//Set do contador de opreacoes a zer0
	op_counter = 0;

	return 0;
}

void termComandos(int force) {
	int i;

	for (i = 0; i < MAX_THREADS; i++)
        criaComando(OP_SAIR, 0, 0, 0);
    activaSinal();
    closeThreads(force);

	for(i = 0; i < NUM_CONTAS; i++) {
		pthread_mutex_destroy(&trinco_contas[i]);
	}

	pthread_cond_destroy(&pode_simular);
	pthread_mutex_destroy(&trinco_cr);
	pthread_mutex_destroy(&trinco_tr);
	pthread_mutex_destroy(&trinco_sims);
	pthread_mutex_destroy(&trinco_contador);

	sem_destroy(&pode_criar);
	sem_destroy(&pode_tratar);

}


/****************************************
* Termina os fios de execucao           *
****************************************/
void closeThreads(int force) {
	int i;
	// Espera que cada tarefa encerre
	for (i=0; i < MAX_THREADS; i++) {
		pthread_join(th_list[i], NULL);
	}
	// Termina os processos filho
	stopNodes(sims, force);
}

/****************************************
* Espera que exista um comando para     *
* tratar e trata-o                      *
****************************************/
void * buscaComando() {
	int curProcess = 0;

	while(1337) {
		comando_t com;
		
		esperar(&pode_tratar);          // Espera que exista um comando
		fechar(&trinco_tr);   // Fecha o acesso de outras tarefas

		com = com_buffer[ptr_tratacoms];
		ptr_tratacoms = (ptr_tratacoms+1) % MAX_COMANDOS;

		abrir(&trinco_tr); // Reabre 
		assinalar(&pode_criar);			 // Assinala que pode produzir

		trataComando(com, &curProcess);  // Trata o comando
	}
}


/****************************************
* Executa o comando                     *
****************************************/
void trataComando(comando_t com, int * curProcess) {
	
	switch(com.command) {
		
		case OP_DEBITAR:
		{
			fechar(&trinco_contas[com.idConta-1]);
			if (debitar (com.idConta, com.valor) < 0)
	           printf("%s(%d, %d): Erro\n\n", COMANDO_DEBITAR, com.idConta, com.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, com.idConta, com.valor);
			abrir(&trinco_contas[com.idConta-1]);
			break;
		}

		case OP_CREDITAR:
		{
			fechar(&trinco_contas[com.idConta-1]);
			if (creditar (com.idConta, com.valor) < 0)
                printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, com.idConta, com.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, com.idConta, com.valor);
			abrir(&trinco_contas[com.idConta-1]);
			break;
		}

		case OP_LER_SALDO: 
		{
			fechar(&trinco_contas[com.idConta-1]);
			com.valor = lerSaldo (com.idConta);
            if (com.valor < 0)
                printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, com.idConta);
            else
                printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, com.idConta, com.valor);
			abrir(&trinco_contas[com.idConta-1]);
			break;
		}

		case OP_SIMULAR: 
		{
			// parar de produzir
			// esperar pelo fim das tarefas, ctr > 0
			// lancar o filho
			// re-inicialisar producao

			pid_t cpid;
			// fecha a producao
			fechar(&trinco_cr);
			// fecha o contador
			fechar(&trinco_contador);
			// caso haja mais do que uma(simulacao) a correr espera o seu termino
			
			while(op_counter > 1) pthread_cond_wait(&pode_simular, &trinco_contador);
            
            cpid = fork();
            if (cpid == 0) {
                //codigo do filho
                simular(com.valor);
                exit(EXIT_SUCCESS);
            } else if (cpid < 0) {
            	printf("Deu erro a criar filho\n");
            } else {
                //codigo do pai

                //abre o contador
                abrir(&trinco_contador);

                // abre a producao
                abrir(&trinco_cr);
                
                //atualiza a lista de processos
                fechar(&trinco_sims);
                sims = createNode(sims, cpid);
                abrir(&trinco_sims);
            }
            break;
		}

		case OP_TRANSFERIR:
		{
			if (com.idConta == com.idConta2) break;

			if (com.idConta < com.idConta2) {
				fechar(&trinco_contas[com.idConta-1]);
				fechar(&trinco_contas[com.idConta2-1]);
			}
			else {
				fechar(&trinco_contas[com.idConta2-1]);
				fechar(&trinco_contas[com.idConta-1]);
			}
			if (debitar (com.idConta, com.valor) < 0)
	           printf("%s(%d, %d): Erro, Saldo insuficiente\n\n", COMANDO_TRANSFERENCIA, com.idConta, com.valor);
	       	else{
				creditar(com.idConta2, com.valor);
				printf("%s(%d)->(%d): Transferencia de %d efectuada.\n\n", COMANDO_TRANSFERENCIA, com.idConta, com.idConta2, com.valor);;
			}

			if (com.idConta < com.idConta2) {
				abrir(&trinco_contas[com.idConta2-1]);
				abrir(&trinco_contas[com.idConta-1]);
			} else {
				abrir(&trinco_contas[com.idConta-1]);
				abrir(&trinco_contas[com.idConta2-1]);
			}
			break;
		}

		case OP_SAIR: 
		{
            pthread_exit(NULL);
		}
	}
	decrementaContador();
}

/****************************************
* Cria um comando a partir dos          *
* argumentos e coloca o no buffer       *
* Recebe:                               *
*   - Numero do comando                 *
*   - Numero da conta                   *
*   - Valor de alteracao                *
****************************************/
void criaComando(int command, int conta, int conta2, int valor) {
	esperar(&pode_criar);
	fechar(&trinco_cr);

	com_buffer[ptr_criacoms].command = command;
	com_buffer[ptr_criacoms].idConta = conta;
	com_buffer[ptr_criacoms].idConta2 = conta2;
	com_buffer[ptr_criacoms].valor = valor;
	ptr_criacoms = (ptr_criacoms+1) % MAX_COMANDOS;
	
	if(command != OP_SAIR) {
		aumentaContador();
	}
	abrir(&trinco_cr);
	assinalar(&pode_tratar);
}

void aumentaContador() {
	fechar(&trinco_contador);
	op_counter++;
	abrir(&trinco_contador);
}

void decrementaContador() {
	fechar(&trinco_contador);
	op_counter--;
	pthread_cond_signal(&pode_simular);
	abrir(&trinco_contador);
}