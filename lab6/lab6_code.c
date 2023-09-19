/*
Andrés Lemus 21634
Electronica Digital 3
Fecha final: 25/08/2023
Laboratorio 6
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <sys/timerfd.h>

#define MAX_LETRAS 100 //Definir máximo de letras del buffer
#define MAX_CADENAS 60 //Definir máximo de cadenas del arreglo
#define PAR "Lab6_primero.txt" //Definir nombre del archivo de líneas pares
#define IMPAR "Lab6_segundo.txt" //Definir nombre del archivo de líneas impares
#define FIN "Lab6_reconstruido.txt" //Definir nombre del archivo final

void hilo_par(void *ptr); //Definir función del hilo de líneas pares
void hilo_impar(void *prt); //Definir función del hilo de líneas impares
void hilo_final(void *ptr); //Definir función para hilo final

char stringArray[MAX_CADENAS][MAX_LETRAS+1]; //Definir arreglo de cadenas

FILE *pares, *impares, *final; //Definir objetos tipo archivo

int main(void)
{ 
        pthread_t thread1; //Crear variable tipo thread
    	pthread_t thread2; //Crear variable tipo thread
    	pthread_t thread3; //Crear variable tipo thread
    
    	int i; //Definir variable para ciclo for

    	char *stringBuffer[MAX_LETRAS+1]; //Definir buffer para los hilos
    
    	pthread_create(&thread1, NULL, (void*)&hilo_par, (void*)stringBuffer); //Crear hilo de líneas pares
    	pthread_create(&thread2, NULL, (void*)&hilo_impar, (void*)stringBuffer); //Crear hilo de líneas impares
    	pthread_create(&thread3, NULL, (void*)&hilo_final, (void*)stringBuffer); //Crear hilo donde se juntan las líneas
    
    	pthread_join(thread1, (void*)stringBuffer); //Esperar a que termine el primer thread
    	pthread_join(thread2, (void*)stringBuffer); //Esperar a que termine el segundo thread
    	pthread_join(thread3, (void*)stringBuffer); //Esperar a que termine el tercer thread
    
    	final = fopen(FIN, "w"); //Abrir para escritura
    
        for (i = 0; i < 59; i++) //Chequear las líneas totales
    	{
    		printf("%s", stringArray[i]); //Mostrar en la terminal
    		fflush(stdout); //Forzar escritura
    		fputs(stringArray[i], final); //Enviar al archivo
    	}	
    
    	return 0;
}

void hilo_par(void *ptr)
{
    
    	char *buffer; //Definir buffer como tipo caracter
    	buffer = (char *)ptr; //Pasar el buffer a la variable del hilo
    
    	uint64_t num_periods; //Variable de 64 bits especificada por read()
    	num_periods = 0; //Iniciarla en 0

    	struct sched_param param; //Llamar a la estructura sched_param para prioridades
    	param.sched_priority = 1; //Asignar prioridad 1
    	sched_setscheduler(0, SCHED_FIFO, &param); //Asignar a este hilo una política de First-In-First-Out

    	pares = fopen(PAR, "r"); //Abrir el archivo de pares para lectura
	if(pares == NULL) //Chequear error
    	{
		printf("Error al abrir el archivo.");
		fflush(stdout);
		exit(0);
	}

    	int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0); //Crear timer

    	if (timer_fd == -1) //Chequear error al crear timer
    	{	
        	printf("Error al crear timer");
		fflush(stdout);
        	exit(0);
    	}

    	struct itimerspec itval; //Llamar a la estructura itimerspec para definir los periodos y tiempos de inicialización
    	itval.it_interval.tv_sec = 0; //Periodo de segundos
    	itval.it_interval.tv_nsec = 3e6; //Periodo en nanosegundos (3ms)

    	itval.it_value.tv_sec = 0; //Tiempo de inicialización después de setear al timer en segundos
    	itval.it_value.tv_nsec = 1e6; //Tiempo de inicialización después de setear al timer en nanosegundos (1ms)

    	int set = timerfd_settime(timer_fd, 0, &itval, NULL); //Setear timer con las características definidas anteriormente

	if (set == -1) //Chequear errores
    	{
        	printf("ERROR INICIANDO TIMER");
        	fflush(stdout);
        	exit(0);
    	}
    
    	read(timer_fd, &num_periods, sizeof(num_periods)); //Esperar a que termine un periodo
    
    	while(fgets(buffer, MAX_LETRAS, pares) != NULL) //Leer una línea del archivo y guardarla en buffer y hacerlo hasta que se termine el archivo
    	{
    		read(timer_fd, &num_periods, sizeof(num_periods)); //Esperar a que termine el tiempo
    	}
    
    	fclose(pares); //Cerrar el archivo

    	pthread_exit(0); //Terminar hilo
}

void hilo_impar(void *ptr)
{

        char *buffer; //Definir buffer como tipo caracter
    	buffer = (char *)ptr; //Pasar el buffer a la variable del hilo
    
    	uint64_t num_periods; //Variable de 64 bits especificada por read()
    	num_periods = 0; //Iniciarla en 0

    	struct sched_param param; //Llamar a la estructura sched_param para prioridades
    	param.sched_priority = 1; //Asignar prioridad 1
    	sched_setscheduler(0, SCHED_FIFO, &param); //Asignar a este hilo una política de First-In-First-Out

    	impares = fopen(IMPAR, "r"); //Abrir el archivo de impares para lectura
        if(impares == NULL) //Chequear error
        {
            printf("Error al abrir el archivo.");
            fflush(stdout);
            exit(0);
        }

    	int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0); //Crear timer

    	if (timer_fd == -1) //Chequear error
    	{
       		printf("Error al crear timer");
            fflush(stdout);
        	exit(0);
    	}

    	struct itimerspec itval; //Llamar a la estructura itimerspec para definir los periodos y tiempos de inicialización
    	itval.it_interval.tv_sec = 0; //Periodo de segundos
    	itval.it_interval.tv_nsec = 3e6; //Periodo en nanosegundos (3ms)

    	itval.it_value.tv_sec = 0; //Tiempo de inicialización después de setear al timer en segundos
        itval.it_value.tv_nsec = 25e5; //Tiempo de inicialización después de setear al timer en nanosegundos (2.5ms)

    	int set = timerfd_settime(timer_fd, 0, &itval, NULL); //Setear timer con las características definidas anteriormente
    
    	if (set == -1) //Chequear error
    	{
       		printf("ERROR INICIANDO TIMER");
       		fflush(stdout);
        	exit(0);
    	}
    
    	read(timer_fd, &num_periods, sizeof(num_periods)); //Esperar a que termine un periodo
    
    	while(fgets(buffer, MAX_LETRAS, impares) != NULL) //Leer una línea del archivo y guardarla en buffer y hacerlo hasta que se termine el archivo
    	{	
    		read(timer_fd, &num_periods, sizeof(num_periods)); //Esperar a que termine el tiempo
    	}
    
    	fclose(impares); //Cerrar el archivo

    	pthread_exit(0); //Terminar hilo
}

void hilo_final(void *ptr)
{
    	char *buffer; //Definir buffer como tipo caracter
    	buffer = (char *)ptr; //Pasar el buffer a la variable del hilo
    
    	uint64_t num_periods; //Variable de 64 bits especificada por read()
    	num_periods = 0; //Iniciarla en 0
    	
    	int cont = 0; //Variable para que el ciclo lea el número de líneas indicadas

    	struct sched_param param; //Llamar a la estructura sched_param para prioridades
    	param.sched_priority = 1; //Asignar prioridad 1
    	sched_setscheduler(0, SCHED_FIFO, &param); //Asignar a este hilo una política de First-In-First-Out

    	int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);//Crear timer

    	if (timer_fd == -1) //Chequear timer
    	{
       		printf("Error al crear timer");
		fflush(stdout);
        	exit(0);
    	}

    	struct itimerspec itval; //Llamar a la estructura itimerspec para definir los periodos y tiempos de inicialización
    	itval.it_interval.tv_sec = 0; //Periodo de segundos
    	itval.it_interval.tv_nsec = 15e5; //Periodo en nanosegundos (1.5ms)

    	itval.it_value.tv_sec = 0; //Tiempo de inicialización después de setear al timer en segundos
    	itval.it_value.tv_nsec = 175e4; //Tiempo de inicialización después de setear al timer en nanosegundos (1.75ms)

    	int set = timerfd_settime(timer_fd, 0, &itval, NULL); //Setear timer con las características definidas anteriormente
    
    	if (set == -1) //Chequear errores
    	{
        	printf("ERROR INICIANDO TIMER");
        	fflush(stdout);
        	exit(0);
    	}
    
    	read(timer_fd, &num_periods, sizeof(num_periods)); //Esperar a que termine un periodo

    	while(cont < 59) //Ciclo hasta termina número de líneas
    	{
    		strcpy(stringArray[cont], buffer); //Copiar del buffer y pegarlo en el arreglo de cadenas
    		cont++; //Aumentar la posición del arreglo
    		read(timer_fd, &num_periods, sizeof(num_periods)); //Esperar a que termine el tiempo
    	}
	
    	pthread_exit(0); //Salir del hilo
}
