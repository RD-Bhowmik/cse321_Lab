#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ARRAY_SIZE 10
#define HALF_SIZE 5

int num[ARRAY_SIZE];
int sum1 = 0;
int sum2 = 0;

void *first_half(void *arg){
    int i;
    for ( i=0; i<HALF_SIZE; i++){
        sum1 += num[i];
        printf("sum of 1st half: %d\n", sum1);
    }
}

void *second_half(void *arg){
    int i;
    for (i=HALF_SIZE; i < ARRAY_SIZE; i++){
        sum2 += num[i];
        printf("sum of 2nd half: %d\n", sum2);
    }
}


int main(){
    pthread_t t1, t2;
    int i; 
    printf("enter int nums: \n");
    for (i =0; i<ARRAY_SIZE; i++){
        scanf("%d", &num[i]);
    }
    pthread_create(&t1, NULL, first_half, NULL);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL, second_half, NULL);
    pthread_join(t2, NULL);
    printf("total sum: %d\n", sum1+sum2);
    return 0;
}