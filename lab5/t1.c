#include <stdio.h>
#include <stdlib.h>


int main(){
    int sum =0; 
    int a;
    int i;
    int num; 
    printf("enter the N: ");
    scanf("%d", &num);

    a = fork();
    if (a == 0){
        printf("child process");
        for (i=0; i<num; i++){
            if (i%2==0){
                sum = sum+i;
                printf("even numbers %d\n",i);
                printf("sum of even numbers: %d\n",sum);

            }
        }
    } 
    else{ 
        printf("parent process");
        for (i=0; i<num; i++){
            if (i%2!=0){
                sum = sum+i;
                printf("odd numbers %d\n",i);
                printf("sum of odd numbers: %d\n",sum);
            }
        }
    }
}

