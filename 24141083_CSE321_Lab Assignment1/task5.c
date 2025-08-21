#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child, grandchild;
    int i;

    child = fork();

    if (child < 0) {
        printf("Fork failed\n");
    }
    
    if (child == 0) {
        printf("2. Child process ID: %d\n", getpid());
        
        for(i = 0; i < 3; i++) {
            grandchild = fork();
            
            if (grandchild < 0) {
                printf("Fork failed\n");
            }
            
            if (grandchild == 0) {
                printf("%d. Grand Child process ID: %d\n", i+3, getpid());
                exit(0);
            }
            else {
                wait(NULL);
            }
        }
        exit(0);
    }
    else {
        printf("1. Parent process ID: %d\n", getpid());
        wait(NULL);
    }

    return 0;
}
