#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

#define ARRAY_SIZE 5

int arr[ARRAY_SIZE]; 
int search;
int found = -1;

void *create_array(void *arg){
    for (int i=0; i<ARRAY_SIZE; i++){
        printf("Enter the element %d: ",i+1);
        scanf("%d",&arr[i]);
    }
    return NULL;
}
void *search_array(void *arg){
    for (int i=0; i<ARRAY_SIZE; i++){
        if (arr[i] == search){
            found = i;
            break;
        }
    }
    return NULL;

    
}
    

int main(){
    pthread_t t1,t2;

    printf("Enter an element to search for: ");
    scanf("%d", &search);

    pthread_create(&t1,NULL,create_array,NULL);
    pthread_join(t1,NULL);
    pthread_create(&t2,NULL,search_array,NULL);
    pthread_join(t2,NULL);
    if (found != -1){
        printf("Element %d found at index %d\n",search,found);
    }
    else{
        printf("Element %d not found in the array\n",search);
    }
    return 0;
    
}

