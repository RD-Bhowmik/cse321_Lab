#include <stdio.h>
#include <stdlib.h>


int main(void){
    FILE *fh_output;
    int data = 500;
    fh_output = fopen("io.txt","w");
    fprintf(fh_output,"data: %d\n", data);   // ei format ta for print inside a file or writing inside a file 


    // printf(fh_output,"data:%d\n",data);  eta kaaj korbe na 
    fclose(fh_output);
}