#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "LinkedList.h"


struct node {
	struct node * _next;
	pid_t _pid;
};

Node createNode(Node head, pid_t pid) {
	Node temp = (Node)malloc(sizeof(struct node));
	temp->_next = head;
	temp->_pid = pid;
	return temp;
}

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

void closeNodes(Node head) {
	if(head == NULL) return;
	closeNodes(head->_next);
	free(head);
}

pid_t runNodes(Node head) {
	Node iter;
	pid_t res;
	int status;
	for (iter = head; iter != NULL; iter = iter->_next) {
		printf("sims nao e nulo no filho\n");
		res = waitpid(iter->_pid, &status, WNOHANG);
	    if (res == -1) {
	        //erro
	        printf("PUFF");
	    } else if (res == 0) {
	        //ainda corre
	        //printf("processo %d ainda corre\n", iter->_pid);
	    } else if (res == iter->_pid) {
	    	//terminou
	    	//TODO ver se o state e tratar erros
	        head = deleteNode(head, iter->_pid);
	    }
	}
}

//para os processos com a force necessaria 
void stopNodes(Node head, int force) {
	Node iter = head;
	int status;
	int command;
	pid_t res;
	int finish[20][2];
	int i = 0;
	int sig;

	if (force == FORCE) sig = SIGTERM;
	else if (force == NOFORCE) sig = SIGUSR1; 

	listNodes(head);

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
		printf("FILHO TERMINADO (PID=%d; terminou %s)\n", finish[i][0], TODO(finish[i][1]));
	}
	printf("--\ni-banco terminou.\n");

	closeNodes(head);
}

void listNodes(Node head) {
	if (head == NULL) return;
	printf("%d\n", head->_pid);
	listNodes(head->_next);
}

const char* TODO(int status){
	if (WIFEXITED(status)){
		return "normalmente";
	}
	else{
		return "abruptamente";
	}
}