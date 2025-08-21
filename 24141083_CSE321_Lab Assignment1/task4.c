#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sort_array(int arr[], int array) {
    for(int x = 0; x < array; x++) {
        for(int z = 0; z < array-x-1; z++) {
            if(arr[z] < arr[z+1]) {
                int store_array = arr[z];
                arr[z] = arr[z+1];
                arr[z+1] = store_array;
            }
        }
    }
}

void check_odd_even(int arr[], int array) {
    for(int x = 0; x < array; x++) {
        if(arr[x] % 2 == 0) {
            printf("%d is even\n", arr[x]);
        } else {
            printf("%d is odd\n", arr[x]);
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("incorrect input");
        return 1;
    }

    int array = argc - 1;  
    int numbers[array];
    
    for(int i = 0; i < array; i++) {
        numbers[i] = strtol(argv[i + 1], NULL, 20);
    }

    pid_t child = fork();

    if(child == 0) {
        sort_array(numbers, array);
        printf("sorted array:");
        for (int i = 0; i < array; i++) {
            printf("%d ", numbers[i]);
        }
        exit(0);
        
    }
    else if(child < 0) {
        printf("Fork failed\n");
    }
    else {
        wait(NULL);  
        printf("\nodd & even check:\n");
        sort_array(numbers, array); 
        check_odd_even(numbers, array);
    }

    return 0;
}
