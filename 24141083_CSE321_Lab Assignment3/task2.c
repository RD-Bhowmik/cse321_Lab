#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct msg {
    long int type;
    char txt[6]; 
};

#define OTP 1
#define LOG_OTP 2
#define MAIL_OTP 3
#define CHECK_MAIL_LOG_OTP 4
#define START 0
#define MAIL 1
#define WORKSPACE "cse321"

void msg_send(int q_id, struct msg* msg, int type) {
    msg->type = type;
    msgsnd(q_id, msg, sizeof(msg->txt), 0);
}


void msg_rec(int q_id, struct msg* msg, int type) {
    msgrcv(q_id, msg, sizeof(msg->txt), type, 0);
}

void resource(int q_id) {
    if (q_id != -1) {
        msgctl(q_id, IPC_RMID, NULL);
    }
}

int opt_check(const char* otp) {
    int len = strlen(otp);
    if (len < 4 || len > 5) return 0;
    
    for (int i = 0; i < len; i++) {
        if (otp[i] < '0' || otp[i] > '9') return 0;
    }
    
    return 1;
}

void login(int q_id) {
    wait(NULL);
    
    struct msg otp_msg;
    msg_rec(q_id, &otp_msg, LOG_OTP);
    printf("Log in received OTP from OTP generator: %s\n", otp_msg.txt);
    

    struct msg mail_msg;
    msg_rec(q_id, &mail_msg, CHECK_MAIL_LOG_OTP);
    printf("Log in received OTP from mail: %s\n", mail_msg.txt);
    
    if (strcmp(otp_msg.txt, mail_msg.txt) == 0) {
        printf("OTP Verified\n");
    } 
    else {
        printf("OTP Incorrect\n");
    }
    
    resource(q_id);
}

void check_mail(int q_id) {
    
    struct msg mail_msg;
    msg_rec(q_id, &mail_msg, MAIL_OTP);
    printf("Mail received OTP from OTP generator: %s\n", mail_msg.txt);
    
    mail_msg.type = CHECK_MAIL_LOG_OTP;
    msg_send(q_id, &mail_msg, CHECK_MAIL_LOG_OTP);
    printf("OTP sent to log in from mail: %s\n", mail_msg.txt);
}



void opt_gene(int q_id, int process_stage) {
    struct msg recv_msg;
    msg_rec(q_id, &recv_msg, OTP);
    
    printf("OTP generator received workspace name from log in: %s\n", WORKSPACE);
    
    int otp = getpid() % 10000;
    
    struct msg msg;
    
    char temp[6];
    snprintf(temp, sizeof(temp), "%d", otp);
    
    strncpy(msg.txt, temp, 5);
    msg.txt[5] = '\0';
    
    if (!opt_check(msg.txt)) {
        fprintf(stderr, "opt invalid: %s\n", msg.txt);
        exit(EXIT_FAILURE);
    }
    
    msg_send(q_id, &msg, LOG_OTP);
    printf("OTP sent to log in from OTP generator: %s\n", msg.txt);
    
    msg_send(q_id, &msg, MAIL_OTP);
    printf("OTP sent to mail from OTP generator: %s\n", msg.txt);
    
    if (process_stage == MAIL) {
        pid_t mail_pid = fork();
        
        if (mail_pid < 0) {
            fprintf(stderr, "Failed to create mail process: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        if (mail_pid == 0) {
            check_mail(q_id);
            exit(EXIT_SUCCESS);
        } else {
            wait(NULL);
        }
    }
}

int main() {
    key_t key = ftok(".", 'M');
    int q_id = msgget(key, IPC_CREAT | 0666);
    
    if (q_id == -1) {
        fprintf(stderr, "Failed to create msg queue: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    char workspace[10];
    printf("Please enter the workspace name:\n");
    scanf("%s", workspace);
    
    if (strcmp(workspace, WORKSPACE) != 0) {
        printf("Invalid workspace name\n");
        resource(q_id);
        exit(EXIT_SUCCESS);
    }
    
    struct msg msg;
    msg.type = OTP;
    
    strncpy(msg.txt, WORKSPACE, 5);
    msg.txt[5] = '\0'; 
    
    msg_send(q_id, &msg, OTP);
    
    printf("Workspace name sent to otp generator from log in: %s\n", WORKSPACE);
    
    pid_t otp_pid = 0, mail_pid = 0;
    int is_parent = 1;
    int process_stage = MAIL;

    otp_pid = fork();
    
    if (otp_pid < 0) {
        fprintf(stderr, "Failed to create OTP generator process: %s\n", strerror(errno));
        resource(q_id);
        exit(EXIT_FAILURE);
    }
    
    if (otp_pid == 0) {
        is_parent = 0;
        opt_gene(q_id, process_stage);
        exit(EXIT_SUCCESS);
    } else if (is_parent) {
        login(q_id);
    }
    
    return EXIT_SUCCESS;
}

