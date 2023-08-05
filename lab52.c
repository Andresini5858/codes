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

void boton(void);
void lectura(void *ptr);

int main (void)
{
    wiringPiSetup () ; //setup
    pinMode(23, INPUT);
    softToneCreate(25);
    pullUpDnControl(23, PUD_UP);
    wiringPiISR(23, INT_EDGE_FALLING, boton);

    for (;;)//Loop
    {    
        while (bandera == 1)
        {

            softToneWrite(25, 1000);
            if (bandera == 0){
                softToneWrite(25, 0);
            }
        }
    }

    return 0 ;
}

void boton(void){
    if (bandera == 0){
    bandera = 1;
    delay(100);
    printf("ENTRE \n");
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

        if (input == 'p')
        {
            bandera = 0;
        }

        else if (input == 'r' && bandera == 0)
        {
            bandera = 1;
        }

        else if (input == 's'){
            exit(0);
        }
        
    }

    pthread_exit(0);
}