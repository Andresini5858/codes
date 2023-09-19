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
int debounce;

void My_thread1(void *ptr);

int main(int argc, char *argv[])
{
    wiringPiSetup();
    pinMode(23, OUTPUT); //PEATONAL
    pinMode(24, OUTPUT); //DIRECCION1
    pinMode(25, OUTPUT); //DIRECCION2
    pinMode(22, INPUT);
    pullUpDnControl(22, PUD_DOWN);

    pthread_t thread1;
    pthread_create(&thread1, NULL, (void*)&My_thread1, NULL);

    while(1)
    {
        status_button = check_button();
    }

    return 0;
}

void My_thread1(void *ptr)
{
    while(1)
    {
        digitalWrite(25, HIGH);
        digitalWrite(24, LOW);
        digitalWrite(23, LOW);
        sleep(1);
        digitalWrite(25,LOW);
        digitalWrite(24, HIGH);
        digitalWrite(23, LOW);
        sleep(1);
        if (status_button == 1)
        {
            digitalWrite(25, LOW);
            digitalWrite(24, LOW);
            digitalWrite(23, HIGH);
            sleep(1);
            clear_button();
        }
    }

    pthread_exit(0);
}