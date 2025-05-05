#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *funcThread(void *arg);

int main(){
    pthread_t t1;
    pthread_create(&t1, NULL, funcThread, NULL);

    printf("checking if it was waiting...\n"); // join er age deyar karone wait kore nai 

    //pthread_join(t1, NULL);  // join er karone wait korse for th functhread function to finish its working 

    printf("checking if it was waiting...\n"); //join er pore deyar karone wait korse 
    return 0;
}

void *funcThread(void *arg){
    printf("enter thread:\n");
    for (int i = 0; i < 3; i++) {
        printf("thread: %d\n", i);
    }
    printf("done with threads...\n");
    return NULL;  // eta slide e bhul disilo 

}
