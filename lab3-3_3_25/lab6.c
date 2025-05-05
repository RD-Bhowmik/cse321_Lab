#include <stdio.h>

int main(void){
    FILE *fptr;
    fptr = fopen("io.txt","r");

    char output[50];

    // fscanf(fptr,"%s",output); // scan e, jdi txt te kono space thake . oikhan theke r read korbe na . 
    // for example data: 500 thakle, only data: print korbe . space er karone 500 print hobe na. 

    fgets(output, 50, fptr); 

    // printf("%s",output);
    printf("%s",output);

    fclose(fptr);
}