#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <semaphore.h> 

int t_id[]={1,2}; 
void *t_func1(int *id); 
void *t_func2(int *id); 
int sum=0; 
sem_t s1,s2; 

int main(){ 
    pthread_t t[2]; 
    sem_init(&s1,0,1); 
    sem_init(&s2,0,0); 
    pthread_create(&t[0],NULL,(void *)t_func1,&t_id[0]); 
    pthread_create(&t[1],NULL,(void *)t_func2,&t_id[1]); 
    for(int i=0;i<2;i++){ 
        pthread_join(t[i],NULL); 
    } 
    sem_destroy(&s1); 
    sem_destroy(&s2); 
    printf("Total sum: %d\n",sum); 
    return 0; 
} 
void *t_func1(int *id){ 
    sem_wait(&s1); 
    for(int i=0;i<10;i++){ 
        printf("Sum: %d\n",sum); 
        sum+=10; 
    } 
    sem_post(&s1); 
    sem_post(&s2); 
} 
void *t_func2(int *id){ 
    sem_wait(&s2); 
    for(int i=0;i<10;i++){ 
        printf("Sum: %d\n",sum); 
        sum-=5; 
    } 
    sem_post(&s2); 
} 