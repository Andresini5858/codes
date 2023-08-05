/*
Andres Lemus 21634
04/08/2023
*/
#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
int main (void)

{
    int random_delay; //definir numero
    wiringPiSetup () ; //setup
    pinMode(23, INPUT);
    pinMode(3, OUTPUT);
    pullUpDnControl(3, PUD_UP);

    for (;;)//Loop
    {    
        if *


    }
    return 0 ;