#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


int *func_thread(int *v);
int *t_ret;

int main(){
    pthread_t t1;
    int n = 5;
    int *t_ret;
                                        // func_thread likhe coma er porer ta hocche oitar modhe pass kora value 
    pthread_create(&t1, NULL , func_thread, &n);
    pthread_join(t1, &t_ret);               // means 5 pass korsi 
    printf("thread returned: %d\n", *t_ret);
    return 0;
}

int *func_thread(int *v){
    *v = *v * 5;
    //    5 * 5 = 25 ashche . 
    return v;
}