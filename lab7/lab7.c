/*
Andrés Lemus 21634
Electrónica Digital 3
Laboratorio 7 
09/08/2023
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <wiringPi.h>
#include "IE3059lab7.h"

int status_button = 0; //Variable para estado del botón

void My_thread1(void *ptr); //Definir hilo a utilizar

int main(int argc, char *argv[])
{
    wiringPiSetup(); //Utilizar numeración de wiringPi
    pinMode(23, OUTPUT); //Led Peatonal
    pinMode(24, OUTPUT); //Led de dirección 2
    pinMode(25, OUTPUT); //Led de dirección 1
    pinMode(22, INPUT); //Botón para el peatón
    pullUpDnControl(22, PUD_DOWN); //Definir el botón con pull-down interno

    pthread_t thread1; //Variable tipo pthread_t
    pthread_create(&thread1, NULL, (void*)&My_thread1, NULL); //Crear el hilo

    while(1)
    {
        status_button = check_button(); //Leer el botón
    }

    return 0;
}

void My_thread1(void *ptr)
{
    while(1)
    {
        digitalWrite(25, HIGH); //Encender dirección 1
        digitalWrite(24, LOW); //Apagar dirección 2
        digitalWrite(23, LOW); //Apagar peatonal
        sleep(1); //delay de 1s
        digitalWrite(25,LOW); //Apagar dirección 1
        digitalWrite(24, HIGH); //Encender dirección 2
        digitalWrite(23, LOW); //Apagar peatonal
        sleep(1); //delay de 1s
        if (status_button == 1) //Si se presiono botón...
        {
            digitalWrite(25, LOW); //Apagar dirección 1
            digitalWrite(24, LOW); //Apagar dirección 2
            digitalWrite(23, HIGH); //Encender peatonal
            sleep(1); //delay de 1s
            clear_button(); //Limpiar botón
        }
    }

    pthread_exit(0);
}
