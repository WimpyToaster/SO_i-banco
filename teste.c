#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "LinkedList.h"



int criaProcessos() {
	pid_t cpid = fork();
	if (cpid == 0) {
		while(1) { 
			sleep(1);
		}
	} else {
		return cpid;
	}
}

int criaProcessoQA() {
	pid_t cpid = fork();
	if (cpid == 0) {
			sleep(2);
			printf("suis finni\n");
			exit(0);
	} else {
		return cpid;
	}
}

int main() {

	Node proc = NULL;

	proc = createNode(proc, criaProcessos());
	proc = createNode(proc, criaProcessos());
	proc = createNode(proc, criaProcessoQA());

	listNodes(proc);
	printf("#######\n");
	int i = 0;
	while(i < 3) {
		runNodes(&proc);
		listNodes(proc);
		printf("#######\n");
		sleep(3);
		i++;
	}
	printf("saiu do loop\n");
	listNodes(proc);
	stopNodes(proc, 0);
	printf("parou tudo\n");

	return 0;
}

