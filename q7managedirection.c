#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>

const void *bye = "Bye bye...\n";

int Command(char *command, int *status, long *exec_time){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);   //start timer

    char *input_file = NULL, *output_file = NULL;
    char *args[100];
    int i = 0;
    char *token = strtok(command, " ");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            input_file = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            output_file = token;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    pid_t pid = fork();                                  //fork a new process
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(pid == 0){                                 //child process
        if (input_file) {
            int in_fd = open(input_file, O_RDONLY);
            if (in_fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (output_file) {
            int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {                                            //parent process
        wait(&status);            //wait for the child to finish
        clock_gettime(CLOCK_MONOTONIC, &end); //end timer
        *exec_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000; // Milliseconds
    }
}

void Prompt(int status, long exec_time){
    char buf[50];
    if (WIFEXITED(status)) {
        snprintf(buf, sizeof (buf), "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), exec_time);
    } else if (WIFSIGNALED(status)) {
        snprintf(buf, sizeof (buf), "enseash [exit:%d|%ldms] %% ", WTERMSIG(status), exec_time);
    }
    write(STDOUT_FILENO, buf, strlen(buf));
}

int main(){
    long exec_time = 0;
    int status = 0;

    while(1){
        Prompt(status, exec_time);
        char userInput[1024];
        if (read(STDIN_FILENO, userInput, 1024) == 0){
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
}
