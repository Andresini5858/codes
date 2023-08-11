/*
Andres Lemus 21634 
Laboratorio 5 Parte 2
04/08/2023
*/
#include <wiringPi.h>
#include <softTone.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

uint8_t bandera = 0;
pthread_t thread2;
char input;

void boton(void);
void lectura(void *ptr);

int main (void)
{
    wiringPiSetup () ; //setup
    pinMode(23, INPUT);
    pullUpDnControl(23, PUD_UP);
    wiringPiISR(23, INT_EDGE_FALLING, boton);

    for (;;)//Loop
    {    
        while (bandera == 1)
        {

            digitalWrite(25, HIGH);
            usleep(500);
            digitalWrite(25, LOW);
            usleep(500); 
            if (bandera == 0){
                digitalWrite(25, LOW);
            }
        }
    }

    return 0 ;
}

void boton(void){
    if (bandera == 0){
    bandera = 1;
    delay(100);
    fflush(stdout);
    pthread_create(&thread2, NULL, (void*)&lectura, NULL);
    }
}

void lectura(void *ptr)
{
    while (1)
    {
        printf("Ingrese el caracter 'p' para salir, 'r' para resumir, o 's' para salir \n");
        fflush(stdout);
        scanf(" %c", &input);

        if (input == 'p' || input == 'P')
        {
            bandera = 0;
        }

        else if ((input == 'r' || input == 'R') && bandera == 0)
        {
            bandera = 1;
        }

        else if (input == 's' || input == 'S'){
            exit(0);
        }
        
    }

    pthread_exit(0);
}