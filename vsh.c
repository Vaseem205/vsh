#include <stdio.h>
#include "main.c"


int main(void){

    int status;

    do{
        status = shell();
    }while(status);

    return 0;
}