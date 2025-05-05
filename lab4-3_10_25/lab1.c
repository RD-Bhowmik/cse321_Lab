#include <stdio.h>
#include <fcntl.h>    /* For file control options like O_RDWR */
#include <unistd.h>   /* For read(), write(), close(), lseek() */
#include <string.h>   /* For string operations */

int main() {

    char buffer[100];
    char message[] = "Hello, World! ami kichu ekta kortesi ig";
    int fd = open("file.txt", O_WRONLY | O_CREAT, 0644);
    
    write(fd, message, sizeof(message)-1);
    close(fd);

    char message2[] = "  Appended";
    int fd2 = open("file.txt", O_WRONLY | O_APPEND);
    
    write(fd2, message2, sizeof(message2)-1);
    close(fd2);
    
    int fd3 = open("file.txt", O_RDWR);
    
    lseek(fd3, 7, SEEK_SET);
    int bytes_read = read(fd3, buffer, 10);
    close(fd3); 
    
    /* Null-terminate the buffer and print its contents */
    buffer[bytes_read] = '\0';
    printf("Read from file: '%s'\n", buffer);

    return 0;
}
