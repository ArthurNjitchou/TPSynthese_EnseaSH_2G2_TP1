//NJITCHOU Arthur & HAZAN Justine
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

void write_message(const char *message) {
    write(STDOUT_FILENO, message, strnlen(message, MAX_INPUT));
}

ssize_t read_input(char *buffer, size_t size) {
    return read(STDIN_FILENO, buffer, size);
}

int main() {
    char input[MAX_INPUT];

    write_message("Welcome to ENSEA Tiny Shell.\n");

    while (1) {
        write_message("enseash %% ");

        ssize_t bytes_read = read_input(input, MAX_INPUT);

        if (bytes_read <= 0) {
            write_message("\nBye bye...\n");
            break;
        }

        input[strnlen(input, bytes_read) - 1] = '\0';

        if (strncmp(input, "exit", 4) == 0 && strnlen(input, bytes_read) == 4) {
            write_message("Bye bye...\n");
            break;
        }

        pid_t pid = fork();

        if (pid == -1) {
            write_message("Fork error\n");
            continue;
        }

        if (pid == 0) {
            char *args[] = {input, NULL};

            if (execvp(input, args) == -1) {
                write_message("Commande not found\n");
                exit(1);
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}

