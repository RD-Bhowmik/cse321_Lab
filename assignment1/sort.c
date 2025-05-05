#include <stdio.h>
#include <stdlib.h>

void sort_array(int arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] < arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Usage: %s number1 number2 ...\n", argv[0]);
        return 1;
    }

    int n = argc - 1; 
    int numbers[n];
    
    for(int i = 0; i < n; i++) {
        numbers[i] = atoi(argv[i + 1]);
    }

    sort_array(numbers, n);
    
    printf("Sorted array in descending order: ");
    for(int i = 0; i < n; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    return 0;
} 