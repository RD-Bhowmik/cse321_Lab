#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

void *t_func(void *arg); 
int var=0; 
int t_id[]={1,2}; 
int main(){ 
    pthread_t t1; 
    pthread_t t2; 
    int a1[]={t_id[0],5}; 
    int a2[]={t_id[1],3}; 
    pthread_create(&t1,NULL,t_func,(void *)a1); 
    pthread_join(t1,NULL); 
    pthread_create(&t2,NULL,t_func,(void *)a2); 
    pthread_join(t2,NULL); 
    printf("Value of var after operations of threads: %d\n",var); 

    return 0; 
} 
void *t_func(void *arg){ 
    int *x=arg; 
    if(x[0]==1){ 
        printf("Entered in Thread :%d\n",x[0]); 
        var+=x[1]; 
        printf("Value of var after the operation of Thread %d: %d\n",x[0],var); 
        printf("Operation Done by Thread %d...\n",x[0]); 
    } 
    else{ 
        printf("Entered in Thread :%d\n",x[0]); 
        var-=x[1]; 
        printf("Value of var after the operation of Thread %d: %d\n",x[0],var); 
        printf("Operation Done by Thread %d...\n",x[0]); 
    } 

} 