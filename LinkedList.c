#include "LinkedList.h"

/****************************************
* Estrutura onde se guardam os id's dos *
* processos                             *
****************************************/
struct node {
	struct node * _next;
	pid_t _pid;
};

/****************************************
* Cria um novo nodo e insere-o no       *
* inicio                                *
* Recebe: Cabeca da lista, Novo id      *
* Retorna: Nova Cabeca da lista         *
****************************************/
Node createNode(Node head, pid_t pid) {
	Node temp = (Node)malloc(sizeof(struct node));
	temp->_next = head;
	temp->_pid = pid;
	return temp;
}


/****************************************
* Apaga um nodo da lista                *
* Recebe: Cabeca da lista, id a retirar *
* Retorna: Cabeca da lista pos mudanca  *
* NOTA: AINDA NAO IMPLEMENTADO          *
****************************************/
Node deleteNode(Node head, pid_t pid) {
	Node iter = head, temp;
	if (head == NULL) return NULL;
	if (iter->_pid == pid) {
		if (iter->_next == NULL) {
			free(iter);
			return NULL;
		} else {
			temp = iter->_next;
			free(iter);
			printf("Apagou 1 e returnou %d\n", temp->_pid);
			return temp;
		}
	}

	while (iter->_next != NULL && iter->_next->_pid != pid) {
		iter = iter->_next;
	}
	if (iter->_next == NULL) {
		printf("Delete : Process not found\n");
		return head;
	} else {
		temp = iter->_next;
		iter->_next = iter->_next->_next;
		free(temp);
		printf("Apagou 2\n");
		return head;
	}
}

/****************************************
* Limpa a estrutura da memoria          *
* Recebe: Cabeca da Lista               *
****************************************/
void closeNodes(Node head) {
	if(head == NULL) return;
	closeNodes(head->_next);
	free(head);
}


/****************************************
* Espera activamente pelos processos    *
* consumindo os terminados e gravando o *
* seu estado de saida                   *
* NOTA: AINDA NAO IMPLEMENTADO          *
****************************************/
void runNodes(Node head) {
	Node iter;
	pid_t res;
	int status;
	for (iter = head; iter != NULL; iter = iter->_next) {
		res = waitpid(iter->_pid, &status, WNOHANG);
	    if (res == -1) {
	        //erro
	    } else if (res == 0) {
	        //ainda corre
	    } else if (res == iter->_pid) {
	    	//terminou
	    	//TODO ver se o state e tratar erros
	        head = deleteNode(head, iter->_pid);
	    }
	}
}

/****************************************
* Para os processos em execucao         *
* Recebe: Cabeca da Lista, forca com    *
* que terminar                          *
* NOTA:                                 *
* 1:                                    *
*   - FORCE    = termina o processo     *
*                pelo sistema           *
*   - NOFORCE  = espera que o processo  *
*                termine                *
*   - SIGFORCE = manda sinal para       *
*                terminar e espera      *
* 2:                                    *
*   Funciona de forma iterativa, ou     *
*   seja, manda o proximo sinal apos o  *
*   fim do processo anterior            *
****************************************/
void stopNodes(Node head, int force) {
	Node iter = head;
	int status;
	pid_t res;
	int finish[20][2];
	int i = 0;
	int sig;

	if (force == FORCE) sig = SIGTERM;
	else if (force == NOFORCE) sig = SIGUSR1;
	while (iter != NULL) {
		res = waitpid(iter->_pid, &status, WNOHANG);
		if (res == 0){
			
			if (force == NOFORCE || force == FORCE){
				kill(iter->_pid, sig);
			}
			
			if(waitpid(iter->_pid, &status, 0)) {
				finish[i][0] = iter->_pid;
				finish[i++][1] = status;
			} else {
				printf("Processo %d crashou\n", iter->_pid);
			}
			

		} else {
			finish[i][0] = iter->_pid;
			finish[i++][1] = status;
		}
		iter = iter->_next;
	}

	printf("i-banco vai terminar.\n--\n");
	for (i--; i >= 0; i--) {
		printf("FILHO TERMINADO (PID=%d; terminou %s)\n", finish[i][0], trataEstado(finish[i][1]));
	}
	printf("--\ni-banco terminou.\n");

	closeNodes(head);
}


/****************************************
* Lista todos os id's na estrutura      *
* Recebe: Cabeca da Lista               *
****************************************/
void listNodes(Node head) {
	if (head == NULL) return;
	printf("%d\n", head->_pid);
	listNodes(head->_next);
}

/****************************************
* Trata o estado de saida do processo   *
* Recebe: Estado de saida do processo   *
* Retorna:                              *
*	- "normalmente" caso nao tenham     *
*	  ocurrido erros                    *
*   - "abruptamente" caso contrario     *
****************************************/
const char* trataEstado(int status){
	if(WIFEXITED(status))
		if (WEXITSTATUS(status)  == EXIT_SUCCESS)
			return "normalmente";
	return "abruptamente";
}