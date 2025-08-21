#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child, grandchild;
    int status;

    child = fork();
    
    if (child < 0) {
        printf("Fork failed\n");
    }
    else if (child == 0) {
       
        grandchild = fork();
        
        if (grandchild < 0) {
            printf("Fork failed\n");
        }
        else if (grandchild == 0) {

            printf("I am grandchild\n");
        }
        else {
            wait(NULL);  
            printf("I am child\n");
        }
    }
    else {
        wait(NULL); 
        printf("I am parent\n");
    }
    
    return 0;
}
