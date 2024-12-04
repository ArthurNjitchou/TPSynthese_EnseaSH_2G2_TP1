//NJITCHOU Arthur & HAZAN Justine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>  // For clock_gettime

const void *bye = "Bye bye...\n";

// Function to execute a command and return its status
int Command(char *command, long *execution_time_ms) {
    pid_t pid = fork();  // Create a new process
    struct timespec start_time, end_time;

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Child process
        execlp(command, command, (char *)NULL);  // Execute the command
        perror("execlp");  // If execlp fails
        exit(EXIT_FAILURE);
    } else {  // Parent process
        clock_gettime(CLOCK_MONOTONIC, &start_time);  // Time before command execution
        int status;
        pid_t wpid = waitpid(pid, &status, 0);
        if (wpid == -1) {
            perror("waitpid");  // If waitpid fails
            exit(EXIT_FAILURE);
        }

        clock_gettime(CLOCK_MONOTONIC, &end_time);  // Time after command execution

        // Calculate execution time in milliseconds
        long long start_ns = start_time.tv_sec * 1000000000 + start_time.tv_nsec;
        long long end_ns = end_time.tv_sec * 1000000000 + end_time.tv_nsec;
        long long duration_ns = end_ns - start_ns;
        *execution_time_ms = duration_ns / 1000000;  // Convert to milliseconds

        return status;
    }
}

// Function to display the welcome message
void WelcomeMessage() {
    const void *message = "Welcome in ENSEA tiny Shell.\nType 'exit' to quit.\n";
    write(STDOUT_FILENO, message, strlen(message));  // Display the message
}

// Function to display the prompt
void Prompt(int status, long execution_time_ms) {
    char prompt[100];

    if (WIFEXITED(status)) {
        snprintf(prompt, sizeof(prompt), "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), execution_time_ms);
    } else if (WIFSIGNALED(status)) {
        snprintf(prompt, sizeof(prompt), "enseash [sign:%d|%ldms] %% ", WTERMSIG(status), execution_time_ms);
    } else {
        snprintf(prompt, sizeof(prompt), "enseash %% ");
    }

    write(STDOUT_FILENO, prompt, strlen(prompt));
}

int main() {
    WelcomeMessage();  // Display the welcome message
    int status = 0;
    long execution_time_ms = 0;  // Variable to store execution time in ms

    while (1) {
        Prompt(status, execution_time_ms);  // Display the prompt with status and execution time
        char userInput[1024];
        int byteread;

        // Read user input
        if ((byteread = read(STDIN_FILENO, userInput, sizeof(userInput))) == 0) {
            // Handle Ctrl+D or end of file
            write(STDOUT_FILENO, bye, strlen(bye));
            exit(EXIT_SUCCESS);
        }
        userInput[byteread - 1] = '\0';  // Remove newline character

        if (strcmp(userInput, "exit") == 0) {  // Check if user wants to exit
            write(STDOUT_FILENO, bye, strlen(bye));
            exit(EXIT_SUCCESS);
        }

        // Execute the command and get its status
        status = Command(userInput, &execution_time_ms);
    }

    return 0;
}

