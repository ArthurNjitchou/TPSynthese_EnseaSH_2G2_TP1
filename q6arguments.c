//NJITCHOU Arthur & HAZAN Justine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>

const void *bye = "Bye bye...\n";

int Command(char *command, int *status, long *exec_time){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);   //start timer

    pid_t pid = fork();                                  //fork a new process
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(pid == 0){                                 //child process
        char *args[100];
        int i = 0;
        char *token = strtok(command, " ");
        while (token != NULL){
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        execvp(args[0], args);                 //execute command with arguments
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {                                            //parent process
        char pid_message[50];
        snprintf(pid_message, sizeof(pid_message), "Child PID : %d\n", pid);
        write(STDOUT_FILENO, pid_message, strlen(pid_message));
        if (waitpid(pid, status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        clock_gettime(CLOCK_MONOTONIC, &end); //end timer
        *exec_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000; // Milliseconds
    }
    return 0;
}

void Prompt(int status, long exec_time){
    char buf[100];
    if (WIFEXITED(status)) {
        snprintf(buf, sizeof (buf), "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), exec_time);
    } else if (WIFSIGNALED(status)) {
        snprintf(buf, sizeof (buf), "enseash [sign:%d|%ldms] %% ", WTERMSIG(status), exec_time);
    }
    write(STDOUT_FILENO, buf, strlen(buf));
}

int main(){
    long exec_time = 0;
    int status = 0;

    while(1){
        Prompt(status, exec_time);
        char userInput[1024];
        int bytesRead = read(STDIN_FILENO, userInput, sizeof(userInput));
        if (bytesRead == 0){
            write(STDOUT_FILENO, bye, strlen(bye));
            exit(EXIT_SUCCESS);
        }
        userInput[strcspn(userInput, "\n")] = '\0';

        if (strcmp(userInput, "exit") == 0) {
            write(STDOUT_FILENO, bye, strlen(bye));
            exit(EXIT_SUCCESS);
        }

        Command(userInput, &status, &exec_time);
    }
    return 0;
}

