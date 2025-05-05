#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#define INPUT_SIZE 200
#define HISTORY_SIZE 50
void show_history();

char cmd_history[HISTORY_SIZE][INPUT_SIZE];
int h_count = 0;
pid_t c_pid = -1;

#define PROMPT "SH> "

void ctrl_c(int sig){
    if (c_pid != -1){   
        kill(c_pid, SIGINT);
    }
    else{
        printf("\n%s", PROMPT);
        fflush(stdout);
    }
}

void shell(){
    int i = 0;
    while (i < HISTORY_SIZE){
        cmd_history[i][0] = '\0';
        i++;
    }
    signal(SIGINT, ctrl_c);
    printf("Shell started. Type 'exit' to quit.\n");
}

void commands_r(char *input){
    printf("%s", PROMPT);
    int c = getc(stdin);
    
    if (c == EOF) {
        printf("\nShell exited\n"); 
        exit(0);
    }
    ungetc(c, stdin);
    
    if (fgets(input, INPUT_SIZE, stdin) == NULL){
        printf("wrong input\n");
        exit(1);
    }
    
}

#define TOKENS 100
void token_commands(char *input, char **tokens){
    char *token;
    int i = 0; 
    token = strtok(input, " \t\n");     // \t spliting  , \n new line  
    for (i =0; token != NULL && i < TOKENS-1; i++){
        tokens[i] = token;
        token = strtok(NULL, " \t\n");
    }
    tokens[i] = NULL;

}


int direction(char **tokens){ 
    int input_d = -1;
    int output_d = -1;
    int c_input = 0;
    int c_output = 0; 
    int c_append = 0;
    char *input = NULL;
    char *output = NULL;
    int i = 0;
    while (tokens[i] != NULL){
        if (strcmp(tokens[i], "<") == 0){
            c_input = 1;
            input = tokens[i+1];
            tokens[i] = NULL;
            i++;
        }
        else if (strcmp(tokens[i], ">") == 0){
            c_output = 1;
            output = tokens[i+1];
            tokens[i] = NULL;
            i++;
        }
        else if (strcmp(tokens[i], ">>") == 0){
            c_append = 1;
            output = tokens[i+1];
            tokens[i] = NULL;
            i++;
        }
        i++;
    }
    if (c_input){
        input_d = open(input, O_RDONLY);
        if (input_d == -1){
            printf("input file error\n");
            return -1;
        }
        dup2(input_d, STDIN_FILENO);
        close(input_d);

    }
    else if (c_append){
        output_d = open(output, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (output_d == -1){
            printf("output file error\n");
            return -1;
        }
        dup2(output_d, STDOUT_FILENO);
        close(output_d); 
    }
    else if (c_output){
        output_d = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_d == -1){
            printf("output file error\n");
            return -1;
        }
        dup2(output_d, STDOUT_FILENO);
        close(output_d);
        
    }
    return 0;
    
}

int commands_exec(char **tokens){
    pid_t pid;
    int sts; 
    if (tokens[0] == NULL){
        return 0;
    }
    
    if (strcmp(tokens[0], "history") == 0){
        show_history();
        return 0;
    }
    else if (strcmp(tokens[0], "exit") == 0){
        printf("exited\n");
        exit(0); 
    }
    else if (strcmp(tokens[0], "cd") == 0){
        if (tokens[1] == NULL){
            chdir(getenv("HOME"));
        }
        else {
            if (chdir(tokens[1]) != 0){
                printf("file not found\n");
            }
        }
        return 0;
    }
    
    // child process
    pid = fork();
    if (pid < 0){
        printf("fork failed\n");
        return -1;
    }
    else if (pid == 0){
        if (direction(tokens) == -1){
            exit(1);
        }
        if (execvp(tokens[0], tokens) == -1){  // execvp check korbe if command is correct 
            printf("command not found\n");
            exit(1);
        }
    }
    else{
        c_pid = pid;
        int sts; 

        if (wait(&sts) != -1){
            c_pid = -1; 
            if ((sts & 0x7F) == 0){
                return (sts >> 8) & 0xFF;
            }
            else {
                return -1;
            }
        }
        else {
            perror("error in wait");
            return -1;
        }
        
    }
    return 0;
}

int pip_commands(char **tokens){
    int j;
    int i = 0;
    int p_count = 0;
    int p_positions[TOKENS];
    int c_start[TOKENS];

    c_start[0] = 0;
    while (tokens[i] != NULL){
        if (strcmp(tokens[i], "|") == 0){
            p_positions[p_count] = i;
            c_start[p_count + 1] = i + 1;
            p_count++;
            tokens[i] = NULL;
        }
        i++;
    }
    if (p_count == 0){
        return commands_exec(tokens);
    }

    int pips[p_count][2];
    pid_t pids[p_count + 1];

    i =0; 
    while (i < p_count){
        if (pipe(pips[i]) == -1){
            printf("pipe error\n");
            return -1;
        }
        i++;
    }
    i = 0; 
    while (i <= p_count){
        pids[i] = fork();

        if (pids[i] < 0){
            printf("fork error\n");
            return -1;
        }
        else if (pids[i] == 0){
            if (i > 0){
                dup2(pips[i-1][0], STDIN_FILENO);
            }
            if (i < p_count){
                dup2(pips[i][1], STDOUT_FILENO);
            }
            j = 0;
            while (j < p_count){
                close(pips[j][0]);
                close(pips[j][1]);
                j++;
            }
            if (direction(&tokens[c_start[i]]) == -1){
                exit(1);
            }
            if (execvp(tokens[c_start[i]], &tokens[c_start[i]]) == -1){
                printf("command not found\n");
                exit(1);
            }
            
        }
        i++;
    }

    for (i=0; i <= p_count; i++){
        if (i< p_count){
            close(pips[i][0]);
            close(pips[i][1]);
        }
        waitpid(pids[i], NULL, 0);
    }
    return 0;
}



void histroy_append(char *command){
    if (strlen(command) == 0){
        return;
    }
    if (h_count >= HISTORY_SIZE){
        for (int i = 0; i < HISTORY_SIZE - 1; i++){
            strcpy(cmd_history[i], cmd_history[i+1]);
        }
        h_count = HISTORY_SIZE - 1;
    }

    strcpy(cmd_history[h_count], command);
    h_count++;
}


void show_history(){
    for (int i = 0; i < h_count; i++){
        printf("%d: %s\n", i+1, cmd_history[i]);
    }
}


int multi_commands(char *input){
    char *commands[TOKENS];
    int cmd_count = 0;
    int store = 0;
    char *cmd;
    int i;

    for (cmd = strtok(input, ";"); cmd != NULL && cmd_count < TOKENS - 1; cmd = strtok(NULL, ";")){
        commands[cmd_count] = cmd;
        cmd_count++;
    }
    i = 0;
    while (i < cmd_count){
        char *short_cmd = commands[i];
        for (; *short_cmd == ' ' || *short_cmd == '\t'; short_cmd++);

        if (strlen(short_cmd) > 0){
            histroy_append(short_cmd);
            char temp_cmd[INPUT_SIZE];
            strcpy(temp_cmd, short_cmd);
            char *tokens[TOKENS];
            token_commands(temp_cmd, tokens);
            store = pip_commands(tokens);
            if (store != 0){
                break;
            }
        }
        i++;
    }
    return store;
    
}

int commands_with_and(char *input){
    char *commands[TOKENS];
    int cmd_count = 0;
    int store = 0;
    char *cmd;
    int i;
    
    for (cmd = strtok(input, "&&"); cmd != NULL && cmd_count < TOKENS - 1; cmd = strtok(NULL, "&&")){
        commands[cmd_count] = cmd;
        cmd_count++;
    }
    i = 0;
    while (i < cmd_count){
        char *short_cmd = commands[i];
        for (; *short_cmd == ' ' || *short_cmd == '\t'; short_cmd++);
        
        if (strlen(short_cmd) > 0){
            char temp_cmd[INPUT_SIZE];
            strcpy(temp_cmd, short_cmd);
            store = multi_commands(temp_cmd);
            if (store != 0){
                return store;
            }
        }
        i++;
    }
    return store;
}

int main(){
    char input[INPUT_SIZE];
    char *tokens[TOKENS];
    
    shell();
    while (1){
        commands_r(input);
        if (strlen(input) > 0){
            char input_copy[INPUT_SIZE];
            strcpy(input_copy, input);
            commands_with_and(input_copy);
            
        }
    }
    return 0;
}
