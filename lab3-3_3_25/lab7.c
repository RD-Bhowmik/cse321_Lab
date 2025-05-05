#include <stdio.h>

int main(void){
    FILE *fptr;
    fptr = fopen("io.txt","r");

    fputs("Hello World",fptr);
    fclose(fptr);
}