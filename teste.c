#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4


pthread_t tid [N];

int buffer[5][3];
int ptr_con = 0, ptr_bui = 0;
pthread_mutex_t trinco_con, trinco_bui;

void espera() {

}

void init() {
	pthread_mutex_init(&trinco_con, NULL);
	pthread_mutex_init(&trinco_bui, NULL);
}

void * trataPedido(void * ptr) {
	printf("Sou a thread %ld\n", pthread_self());

}

void main () {	
	int i;
	for (i=0; i<N; i++) {
		if (pthread_create(&tid[i], NULL,trataPedido, (void *) &buffer) == 0)
			printf("Criou com sucesso a tarefa %ld\n", tid[i]);
		else {
			printf("shit happened");
			exit(1);
		}
	}
}

