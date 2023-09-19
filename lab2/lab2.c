#include <stdio.h>
#include <stdlib.h>

int var = 0;

int main(void){
    int x;

    for (x = 1; x <= 20; x++){
        var += 1;
        printf("%d \n", var);
        sleep(1);
    }

    puts("adios");
    return 0;
}