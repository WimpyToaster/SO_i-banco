#include "wrappers.h"

void fechar(pthread_mutex_t * m) {
	if (pthread_mutex_lock(m)) {
		printf("Deu erro a fechar um trinco\n");
		exit(EXIT_SUCCESS);
	}
}

void abrir(pthread_mutex_t * m) {
	if (pthread_mutex_unlock(m)) {
		printf("Deu erro a abrir um trinco\n");
		exit(EXIT_SUCCESS);
	}
}

void esperar(sem_t * s) {
	if(sem_wait(s)) {
		printf("Deu erro a esperar por semaforo\n");
		exit(EXIT_SUCCESS);
	}
}

void assinalar(sem_t * s) {
	if(sem_post(s)) {
		printf("Deu erro a assinalar um semaforo\n");
		exit(EXIT_SUCCESS);
	}
}

