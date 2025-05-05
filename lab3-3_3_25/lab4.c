#include <stdio.h>

// void fun(int *x, int *y) {
// int fun(int *x, int *y) {
//     *x = 20; 
//     *y = 30;
// }
// void fun(int x, int y) {
//     x = 20; 
//     y = 30;
// }
int fun(int *x, int *y) {
    *x = 20; 
    *y = 30;
}

int main() {
    int x = 10, y = 20;
    fun(&x, &y);
    printf("x=%d, y=%d\n", x, y);
    return 0;
}