#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char user[100];
    
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }
    
    file = fopen(argv[1], "w");
    
    printf("file opened, to stop enter -1\n");

    while (true) {
        printf("enter your input: ");
        fgets(user, sizeof(user), stdin);
        user[strcspn(user, "\n")] = 0;
        
        if (strcmp(user, "-1") == 0) {
            break;
        }
        
        fprintf(file, "%s\n", user);
    }
    
    fclose(file);
    
    return 0;
}



