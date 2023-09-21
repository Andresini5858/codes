#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <wiringPi.h>
#include "IE3059lab7.h"

int status_button; //Variable para leer botón

sem_t my_semaphore; //Variable tipo semaphore

int priority1, priority2, priority3; //Variables para manejar prioridades

void My_thread1(void *ptr); //Hilo para direccional 1
void My_thread2(void *ptr); //Hilo para direccional 2
void My_thread3(void *ptr); //Hilo para peatonal

int main(int argc, char *argv[])
{
    if (argc == 1) //Si no se indicaron argumentos extras al correr el programa...
    {
        printf("Sin argumentos, proridades en 1 \n"); //Mostrar mensaje
        fflush(stdout); //Forzar escritura
        priority1 = 1; //Asignar prioridad 1 a direccional 1
        priority2 = 1; //Asignar prioridad 1 a direccional 2
        priority3 = 1; //Asignar prioridad 1 a direccional 3
    }

    else if (argc == 4) //Si se indicaron tres argumentos al correr el programa...
    {
        printf("Prioridades seteadas respectivamente \n"); //Mostrar mensaje
        fflush(stdout); //Forzar escritura
        priority1 = atoi(argv[1]); //Asignar prioridad de direccional 1 con primer argumento
        priority2 = atoi(argv[2]); //Asignar prioridad de direccional 2 con segundo argumento
        priority3 = atoi(argv[3]); //Asignar prioridad de peatonal con tercer argumento

        if ((priority1 == 0) || (priority2 == 0) || (priority3 == 0)) //Si se introdujo un numero no entero, cadena, o 0...
        {
            printf("Se introdujo un número no entero o 0 \n"); //Mostrar mensaje
            fflush(stdout); //Forzar escritura
            exit(0); //Salir
        }
        
    }

    else //Si se puso un diferente número de argumentos a lo solicitado...
    {
        printf("El argumento debe ser de vacío o tres enteros. Los enteros representan la prioridad de cada hilo respectivamente \n"); //Mostrar mensaje
        fflush(stdout); //Forzar escritura
        exit(0); //Salir
    }

    sem_init(&my_semaphore, 0, 1); //Inicializar semaphore

    wiringPiSetup(); //Usar numeración de wiringPi
    pinMode(23, OUTPUT); //Led peatonal
    pinMode(24, OUTPUT); //Led de dirección 1
    pinMode(25, OUTPUT); //Led de dirección 2
    pinMode(22, INPUT); //Botón como entrada
    pullUpDnControl(22, PUD_DOWN); //Botón con pull-down interno

    pthread_t thread1; //Variable del hilo 1
    pthread_t thread2; //Variable del hilo 2
    pthread_t thread3; //Variable del hilo 3

    pthread_create(&thread1, NULL, (void*)&My_thread1, NULL); //Crear hilo de direccional 1
    pthread_create(&thread2, NULL, (void*)&My_thread2, NULL); //Crear hilo de direccional 2
    pthread_create(&thread3, NULL, (void*)&My_thread3, NULL); //Crear hilo peatonal

    while(1)
    {

    }

    return 0;
}

void My_thread1(void *ptr)
{

    struct sched_param param; //Llamar a la estructura sched_param para prioridades
    param.sched_priority = priority1; //Asignar prioridad dependiendo del argumento
    int sched = sched_setscheduler(0, SCHED_RR, &param); //setear política de escalonamiento
    if (sched == -1){ //Checkear error 
        printf("error al asignar prioridad\n");
        fflush(stdout);
        exit(0);
    }

    while(1)
    {
        sem_wait(&my_semaphore); //Esperar al semáforo
        digitalWrite(25, HIGH); //Encender dirección 1
        sleep(1); //delay de 1s
        digitalWrite(25, LOW); //Apagar dirección 1
        sem_post(&my_semaphore); //Devolver semaphore
        usleep(1000); //delay mínimo
    }

    pthread_exit(0);
}

void My_thread2(void *ptr)
{
    struct sched_param param; //Llamar a la estructura sched_param para prioridades
    param.sched_priority = priority2; //Asignar prioridad dependiendo del argumento
    int sched = sched_setscheduler(0, SCHED_RR, &param); //setear política de escalonamiento
    if (sched == -1){ //Checkear error 
        printf("error al asignar prioridad\n");
        fflush(stdout);
        exit(0);
    }

    while(1)
    {
        sem_wait(&my_semaphore); //Esperar al semáforo
        digitalWrite(24, HIGH); //Encender dirección 2
        sleep(1); //delay de 1s
        digitalWrite(24, LOW);//Apagar direccion 2
        sem_post(&my_semaphore); //Devolver semaphore
        usleep(1000); //delay mínimo
    }

    pthread_exit(0);
}

void My_thread3(void *ptr)
{
    struct sched_param param; //Llamar a la estructura sched_param para prioridades
    param.sched_priority = priority3; //Asignar prioridad dependiendo del argumento
    int sched = sched_setscheduler(0, SCHED_RR, &param); //setear política de escalonamiento
    if (sched == -1){ //Checkear error
        printf("error al asginar prioridad\n");
        fflush(stdout);
        exit(0);
    }

    while(1)
    {
        sem_wait(&my_semaphore); //Esperar al semaphore
        status_button = check_button(); //Ver si se presionó el botón
        if (status_button == 1) //Si se presionó...
        {
            digitalWrite(23, HIGH);//Encender peatonal
            sleep(1); //delay de 1s
            digitalWrite(23, LOW);//Apagar peatonal
            clear_button(); //Limpiar bandera de botón
        }  
        sem_post(&my_semaphore); //Devolver semaphore
        usleep(1000); //delay mínimo
    }

    pthread_exit(0);
}
