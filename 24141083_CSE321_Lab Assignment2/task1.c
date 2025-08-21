#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int n;                 
int* fibo_arr;  
int s_count;       
int* s_idx;    
int* store;    

void* fibo(void* arg) {
    int arr_size = (n + 1) * sizeof(int);
    fibo_arr = (int*)malloc(arr_size);

    if (fibo_arr == NULL) {
        printf("allocation error\n");
        pthread_exit(NULL);
    }
    
    fibo_arr[0] = 0;
    if (n >= 1) {
        fibo_arr[1] = 1;
    }
    
    int i = 2;
    while (i <= n) {
        fibo_arr[i] = fibo_arr[i-1] + fibo_arr[i-2];
        i++;
    }
    pthread_exit(NULL);
}

void* search_fibo(void* arg) {
    int i = 0;
    while (i < s_count) {
        int index = s_idx[i];
        if (index >= 0 && index <= n) {
            store[i] = fibo_arr[index];
        } else {
            store[i] = -1;
        }
        i++;
    }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t f_thread;
    pthread_t s_th;
    
    printf("Enter the term of fibonacci sequence:\n");
    scanf("%d", &n);

    if (!(n >= 0 && n <= 40)) {
        printf("0 ≤ n ≤ 40 \n");
        return 1;
    }

    printf("How many numbers you are willing to search?:\n");
    scanf("%d", &s_count);
    
    if (s_count <= 0) {
        printf("enter positive int.\n");
        return 1;
    }
    
    int search_arr = s_count * sizeof(int);
    
    s_idx = (int*)malloc(search_arr);
    
    store = (int*)malloc(search_arr);
    
    if (!(s_idx != NULL && store != NULL)) {
        printf("allocation fail\n");
        return 1;
    }
    
    int i = 0;
    while (i < s_count) {
        printf("Enter search %d:\n", i + 1);
        scanf("%d", &s_idx[i]);
        i++;
    }
    
    if (pthread_create(&f_thread, NULL, fibo, NULL) == 0) {
        if (pthread_join(f_thread, NULL) == 0) {
        } 
        else {
            printf("fibo thread join fail\n");
            return 1;
        }
    }
    else {
        printf("fibo thread fail\n");
        return 1;
    }
    
    i = 0;
    while (i <= n) {
        printf("a[%d] = %d\n", i, fibo_arr[i]);
        i++;
    }
    
    if (pthread_create(&s_th, NULL, search_fibo, NULL) == 0) {
        if (pthread_join(s_th, NULL) == 0) {
        } 
        else {
            printf("search thread join fail\n");
            return 1;
        }
    } 
    else {
        printf("search thread fail\n");
        return 1;
    }
    
    i = 0;
    while (i < s_count) {
        printf("result of search #%d = %d\n", i + 1, store[i]);
        i++;
    }
    
    free(fibo_arr);
    free(s_idx);
    free(store);
    
    return 0;
}
