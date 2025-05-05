#include <stdio.h>
#include <unistd.h>

main(){ 
    int c;
    fork(); 
    fork(); 
    c=fork(); 
    if(c>0){ 
    printf("hi\n"); 
    fork(); 
    } 
    fork(); 
    printf("bye\n"); 
} 
