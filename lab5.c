/*
Andres Lemus 21634
04/08/2023
*/
#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main (void)

{
    int random_delay; //definir numero
    wiringPiSetup () ; //setup
    pinMode(25, OUTPUT);
    pinMode(24, OUTPUT);
    digitalWrite(25, HIGH);
    digitalWrite(24, LOW);
    srand(time(NULL));
    for (;;)//Loop
    {    
        random_delay = rand() % 1000 + 1 + 500;

        printf("%d\n", random_delay);
        fflush(stdout);

        delay(random_delay);

        digitalWrite(25, !digitalRead(25));
        digitalWrite(24, !digitalRead(24));

    }
    return 0 ;
}