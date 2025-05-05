#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
int t_id[]={1,2,3}; 
int var=50; 
void *t_func(int *v); 
int main(){ 
    pthread_t t[3]; 
    for(int i=0;i<3;i++){ 
        pthread_create(&t[i],NULL,(void *)t_func,&t_id[i]); 
    } 
    for(int i=0;i<3;i++){ 
        pthread_join(t[i],NULL); 
    } 
    printf("Final value of var: %d\n",var); 
    return 0; 
    } 
    
void *t_func(int *v){ 
    if(*v==0){ 
        printf("Entered in Thread %d...\n",*v); 
        for(int i=0;i<3;i++){ 
            var+=5; 
            printf("Thread %d modified value %d\n",*v,var); 
        } 
        printf("Modification done by Thread %d, value %d\n",*v,var); 
    } 
    else if(*v==1){ 
        printf("Entered in Thread %d...\n",*v); 
        for(int i=0;i<3;i++){ 
            var-=4; 
            printf("Thread %d modified value %d\n",*v,var); 
        }    
        printf("Modification done by Thread %d, value %d\n",*v,var); 
    } 
    else{ 
        printf("Entered in Thread %d...\n",*v); 
        for(int i=0;i<3;i++){ 
            var*=2; 
            printf("Thread %d modified value %d\n",*v,var); 
        } 
        printf("Modification done by Thread %d, value %d\n",*v,var); 
    } 
} 