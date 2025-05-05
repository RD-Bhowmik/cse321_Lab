#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>



int main() {
    pid_t a, b, c;
    static int p_count = 0; 
    pid_t child_pid = getpid();   
    
    a = fork();
    if (a >= 0) { 
        p_count++;
        if (a > 0 && a % 2 == 1) {  
            fork();
            p_count++;
        }
    }
    b = fork();
    if (b >= 0) { 
        p_count++;
        if (b > 0 && b % 2 == 1) {  
            p_count++;
        }
    }
    
    c = fork();
    if (c >= 0) {  
        p_count++;
        if (c > 0 && c % 2 == 1) {  
            fork();
            p_count++;
        }
    }
    
    while (wait(NULL) > 0);
        if (getpid() == child_pid) {
            printf("Total number of processes created: %d\n", p_count);
        }
    
    return 0;
}
