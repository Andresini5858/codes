#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <wiringPi.h>
#include "IE3059lab7.h"

int status_button = 0;

sem_t my_semaphore;

int priority1, priority2, priority3;

void My_thread1(void *ptr);
void My_thread2(void *ptr);
void My_thread3(void *ptr);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Sin argumentos, proridades en 1 \n");
        fflush(stdout);
        priority1 = 1;
        priority2 = 1;
        priority3 = 1;
    }

    else if (argc == 4)
    {
        printf("Prioridades seteadas respectivamente \n");
        fflush(stdout);
        priority1 = atoi(argv[1]);
        priority2 = atoi(argv[2]);
        priority3 = atoi(argv[3]);

        if ((priority1 == 0) || (priority2 == 0) || (priority3 == 0))
        {
            printf("Se introdujo un número no entero o 0 \n");
            fflush(stdout);
            exit(0);
        }
        
    }

    else
    {
        printf("El argumento debe ser de vacío o tres enteros. Los enteros representan la prioridad de cada hilo respectivamente \n");
        fflush(stdout);
        exit(0);
    }

    sem_init(&my_semaphore, 0, 1);

    wiringPiSetup();
    pinMode(23, OUTPUT); //PEATONAL
    pinMode(24, OUTPUT); //DIRECCION1
    pinMode(25, OUTPUT); //DIRECCION2
    pinMode(22, INPUT);
    pullUpDnControl(22, PUD_DOWN);

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    pthread_create(&thread1, NULL, (void*)&My_thread1, NULL);
    pthread_create(&thread2, NULL, (void*)&My_thread2, NULL);
    pthread_create(&thread3, NULL, (void*)&My_thread3, NULL);

    while(1)
    {
        //status_button = check_button();
        //usleep(1000);
    }

    return 0;
}

void My_thread1(void *ptr)
{

    struct sched_param param; //Llamar a la estructura sched_param para prioridades
    param.sched_priority = priority1; //Asignar prioridad 1
    printf("%d \n", priority1);
    fflush(stdout);
    sched_setscheduler(0, SCHED_RR, &param); //Asignar a este hilo una política de First-In-First-Out

    while(1)
    {
        sem_wait(&my_semaphore);
        //printf("luego del semwait hilo 1 roja \n");
        //fflush(stdout);
        digitalWrite(25, HIGH); //Dirección 1
        sleep(1);
        digitalWrite(25, LOW); //Dirección 1
        sem_post(&my_semaphore);
        usleep(1000);   
    }

    pthread_exit(0);
}

void My_thread2(void *ptr)
{
    struct sched_param param; //Llamar a la estructura sched_param para prioridades
    param.sched_priority = priority2; //Asignar prioridad 2
    printf("%d \n", priority2);
    fflush(stdout);
    sched_setscheduler(0, SCHED_RR, &param); //Asignar a este hilo una política de First-In-First-Out

    while(1)
    {
        sem_wait(&my_semaphore);
        //printf("luego del semwait hilo 2 amarilla \n");
        //fflush(stdout);
        digitalWrite(24, HIGH);//Direccion 2
        sleep(1);
        digitalWrite(24, LOW);//Direccion 2
        sem_post(&my_semaphore);
        usleep(1000);
    }

    pthread_exit(0);
}

void My_thread3(void *ptr)
{
    struct sched_param param; //Llamar a la estructura sched_param para prioridades
    param.sched_priority = priority3; //Asignar prioridad 3
    printf("%d \n", priority3);
    fflush(stdout);
    int sched = sched_setscheduler(0, SCHED_RR, &param); //Asignar a este hilo una política de First-In-First-Out
    if (sched == -1){
        printf("error \n");
        fflush(stdout);
    }

    while(1)
    {
        sem_wait(&my_semaphore);
        //printf("luego del semwait hilo 3 verde \n");
        //fflush(stdout);
        status_button = check_button();
        if (status_button == 1)
        {
            digitalWrite(23, HIGH);//Peatonal
            sleep(1); 
            digitalWrite(23, LOW);//Peatonal
            clear_button();
        }  
        //else {
            sem_post(&my_semaphore);
            usleep(1000);
        //}
    }

    pthread_exit(0);
}