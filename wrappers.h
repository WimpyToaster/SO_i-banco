#ifndef _WRPR_
#define _WRPR_

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void fechar(pthread_mutex_t*);
void abrir(pthread_mutex_t*);
void esperar(sem_t*);
void assinalar(sem_t*);

#endif