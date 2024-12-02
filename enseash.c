#include <stdio.h>
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

    write_message("Bienvenue dans le Shell ENSEA.\n");
    write_message("Pour quitter, tapez 'exit'.\n");

    // Boucle principale du shell
    while (1) {
        // Affichage du prompt
        write_message("enseash %% ");

        // Lire la commande entrée par l'utilisateur
        ssize_t bytes_read = read_input(input, MAX_INPUT);

        // Si l'utilisateur tape Ctrl+D (EOF), quitter
        if (bytes_read <= 0) {
            write_message("\n");
            break;
        }

        // Suppression de caractère de nouvelle ligne (si présent)
        input[strnlen(input, bytes_read) - 1] = '\0';

        // Si l'utilisateur entre "exit", quitter le shell
        if (strncmp(input, "exit", 4) == 0 && strnlen(input, bytes_read) == 4) {
            break;
        }

        // Exécution de la commande
        pid_t pid = fork();

        if (pid == -1) {
            // Erreur de fork
            write_message("Erreur de fork\n");
            continue;
        }

        if (pid == 0) {
            
            char *args[] = {input, NULL};
            if (execvp(input, args) == -1) {
                write_message("Commande introuvable\n");
                exit(1);
            }
        } else {
            
            wait(NULL);
        }
    }

    // Message de sortie
    write_message("Au revoir !\n");

    return 0;
}

