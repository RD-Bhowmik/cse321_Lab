#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

struct shared {
    char sel[100];
    int b;
};

int balance(int amount, int balance, char op) {
    if (amount <= 0){
        return 0;
    }
    if (op == 'w' && amount > balance){
        return 0;
    }
    return 1;
}

void user_inputs() {
    printf("Provide Your Input From Given Options:\n");
    printf("1. Type a to Add Money\n");
    printf("2. Type w to Withdraw Money\n");
    printf("3. Type c to Check Balance\n");
}

void deposits(struct shared *data) {
    int add_amount;
    printf("Enter amount to be added:\n");
    scanf("%d", &add_amount);
    
    if (balance(add_amount, (*data).b, 'a')) {
        (*data).b += add_amount;
        printf("Balance added successfully\n");
        printf("Updated balance after addition:\n%d\n", (*data).b);
        printf("Thank you for using\n");
    } 
    else {
        printf("Adding failed, Invalid amount\n");
    }
}

void withdraw(struct shared *data) {
    int wd_amount;
    printf("Enter amount to be withdrawn:\n");
    scanf("%d", &wd_amount);
    
    if (balance(wd_amount, (*data).b, 'w')) {
        (*data).b -= wd_amount;
        printf("Balance withdrawn successfully\n");
        printf("Updated balance after withdrawal:\n%d\n", (*data).b);
        printf("Thank you for using\n");
    } 
    else {
        printf("Withdrawal failed, Invalid amount\n");
    }
}

int main() {

    key_t key = ftok(".", 'S');
    int shmid = shmget(key, sizeof(struct shared), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    struct shared *mem = (struct shared *)shmat(shmid, NULL, 0);
    if (mem == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    int pip[2];
    if (pipe(pip) == -1) {
        perror("pipe failed");
        exit(1);
    }

    user_inputs();

    scanf("%s", (*mem).sel);
    (*mem).b = 1000; 

    printf("Your selection: %s\n", (*mem).sel);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) { 
        close(pip[0]);

        wait(NULL);

        char message[100];
        read(pip[0], message, sizeof(message));
        printf("%s\n", message);

        shmdt(mem);
        shmctl(shmid, IPC_RMID, NULL);
        close(pip[1]);
    }
    else { 
        close(pip[1]);

        if (strcmp((*mem).sel, "a") == 0) {
            deposits(mem);
        }
        else if (strcmp((*mem).sel, "w") == 0) {
            withdraw(mem);
        }
        else if (strcmp((*mem).sel, "c") == 0) {
            printf("Your current balance is:\n%d\n", (*mem).b);
            printf("Thank you for using\n");
        }
        else {
            printf("Invalid selection\n");
        }

        const char *message = "Thank you for using";
        write(pip[1], message, strlen(message) + 1);

        shmdt(mem);
        close(pip[0]);
        exit(0);
    }

    return 0;
}
