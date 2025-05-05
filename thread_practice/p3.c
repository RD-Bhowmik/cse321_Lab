#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *func_thread(void *n);

int main(){
    pthread_t t1;
    int num = 5;  // Using local variable instead of global
    int *result;
    
    pthread_create(&t1, NULL, func_thread, &num);
    pthread_join(t1, (void**)&result);
    printf("thread returned: %d\n", *result);
    free(result);  // Free allocated memory
    return 0;
}

void *func_thread(void *n){
    printf("entered in thread:\n");
    int *num = (int*)n;
    int *result = malloc(sizeof(int));  // Allocate memory for result
    
    if (*num % 2 == 0){
        *result = (*num) * (*num);  // Square for even numbers
    }
    else{
        *result = (*num) * (*num) * (*num);  // Cube for odd numbers
    }
    
    return result;
}