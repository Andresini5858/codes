/*
* test_thrd_mutex.c
* Por: Luis Alberto Rivera
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void My_thread1(void *ptr);  // prototipo

int counter = 0;	// se pudo definir local en el main, y pasar la dirección a los hilos
pthread_mutex_t lock;

int main(void)
{
	pthread_t thread1;
	srand(time(NULL));	// inicializa semilla del generador de números aleatorios
	
	pthread_create(&thread1, NULL, (void*)&My_thread1, NULL);
	
	while(counter < 100)
	{
		usleep(500000+rand()%1000001);	// tiempo aleatorio entre 0.5 y 1.5 segundos
		pthread_mutex_lock(&lock);
		counter++;
		pthread_mutex_unlock(&lock);
		printf("ParentThread: %d \n", counter);
	}
	
	return(0);
}

void My_thread1(void *ptr)
{
	while(counter < 100)
	{
		usleep(500000+rand()%1000001);	// tiempo aleatorio entre 0.5 y 1.5 segundos
		pthread_mutex_lock(&lock);
		counter++;
		pthread_mutex_unlock(&lock);
		printf("My_thread1: %d \n", counter);
	}
	
	pthread_exit(0);
}
