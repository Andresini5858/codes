/*
Andres Lemus 21634
04/08/2023
*/
#include <wiringPi.h>
#include <softTone.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

uint8_t bandera = 0;
pthread_t thread2;
char input;

int main (void)
{
    wiringPiSetup () ; //setup
    pinMode(23, INPUT);
    //pinMode(3, OUTPUT);
    softToneCreate(3);
    pullUpDnControl(23, PUD_UP);
    wiringPiISR(23, INT_EDGE_FALLING, boton);

    for (;;)//Loop
    {    
        while (bandera == 1)
        {
            softToneWrite(3, 2000);
        }
    }

    return 0 ;
}

void boton(void){
    delay(25);
    bandera = 1;
    pthread_create(&thread2, NULL, (void*)&lectura, NULL);
    wiringPiISR(23, INT_EDGE_FALLING, NULL);
}

void lectura(void *ptr)
{
    while (1)
    {
        printf("Ingrese el caracter 'p' para salir, 'r' para resumir, o 's' para salir ");
        fflush(stdout);
        scanf(" %c", &input);

        if (input == 'p')
        {
            bandera = 0;
        }

        else if (input == 'r' && bandera == 0)
        {
            bandera == 1;
        }

        else if (input == 's'){
            exit(0);
        }
        
    }

    pthread_exit(0);
}