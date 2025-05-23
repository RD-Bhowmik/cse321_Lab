#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;
    int global_a = 90, b = 11;
    char message[] = "Hello, from the ";
    printf("Parent process started\n");
    
    child_pid = fork();
    if (child_pid == -1) {
        printf("Fork Failed\n");
    } else if (child_pid > 0) {
        wait(NULL);
        global_a += 97; 
        printf("%sAddition: %d + %d = %d\n", message, global_a, b, global_a + b);
        printf("%sSubtraction: %d - %d = %d\n", message, b, global_a, b - global_a);
    } else {
        b *= 7;
        printf("Multiplication: %d * %d = %d\n", global_a, b, global_a * b);
        printf("Division: %d / %d = %d\n", b, global_a, b / global_a);
    }
    return 0;
}
