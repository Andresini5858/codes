/*
Andres Lemus 21634
Laboratorio 5 Parte 1
04/08/2023
*/
#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int random_delay; //definir variable para delay random


int main (void)

{
    wiringPiSetup () ; //setup
    pinMode(25, OUTPUT); //definir pin 25 como salida
    pinMode(24, OUTPUT); //definir pin 24 como salida
    digitalWrite(25, HIGH); //Iniciar con pin 25 en alto
    digitalWrite(24, LOW); //Iniciar con pin 24 en bajo
    srand(time(NULL)); //Que la semilla para la secuencia de numeros random siempre cambie

    //LOOP//
    for (;;)
    {    
        random_delay = (rand() % 1000 + 1) + 500; //que el delay random lo tome con un maximo de 1000 y sumarle quinientos

        printf("%d\n", random_delay); //mostrar el numero
        fflush(stdout); 

        delay(random_delay); //realizar el delay

        digitalWrite(25, !digitalRead(25)); //Si estaba apagado el pin, encenderlo y viceversa
        digitalWrite(24, !digitalRead(24)); //Si estaba apagado el pin, encenderlo y viceversa

    }
    return 0 ;
}