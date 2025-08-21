#include <stdio.h>
#include <stdlib.h>


void check_odd_even(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        if(arr[i] % 2 == 0) {
            printf("%d is even\n", arr[i]);
        } else {
            printf("%d is odd\n", arr[i]);
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


    printf("Checking odd/even status:\n");
    check_odd_even(numbers, n);

    return 0;
} 