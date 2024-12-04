//NJITCHOU Arthur & HAZAN Justine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

const void *bye = "Bye bye...\n";

int Command(char *command){
    pid_t pid = fork();                                  //fork a new process
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(pid == 0){                                 //child process
        execlp(command, command, (char *)NULL);                           //if execlp fails
        perror("execlp");                             //if execlp fails
        exit(EXIT_FAILURE);
    } else {                                            //parent process
        int status;
        pid_t wpid = waitpid(pid, &status,0);
        if (wpid == -1) {
            perror("execlp");                             //if execlp fails
            exit(EXIT_FAILURE);
        }
        return(status);
    }
}

void WelcomeMessage(){
    const void *message= "Welcome in ENSEA tiny Shell.\nType 'exit' to quit.\n";

    //Display of a welcome message
    write(STDOUT_FILENO, message, strlen(message));
}

void Prompt(int status){
    char prompt[100];

    if(WIFEXITED(status)){
        snprintf(prompt, sizeof(prompt), "enseash [exit:%d] %% ", WEXITSTATUS(status));
    } else if(WIFSIGNALED(status)){
        snprintf(prompt, sizeof(prompt), "enseash [sign:%d] %% ", WTERMSIG(status));
    } else {
        snprintf(prompt, sizeof(prompt), "enseash %% ");
    }

    write(STDOUT_FILENO, prompt, strlen(prompt));
}

int main(){
    WelcomeMessage();
    int status = 0;

    while(1){
        Prompt(status);
        char userInput[1024];
        int byteread;

        //read user input
        if((byteread = read(STDIN_FILENO,userInput, sizeof(userInput))) == 0){
            //handle ctrl+d or end of file
            write(STDOUT_FILENO,bye, strlen(bye));
            exit(EXIT_SUCCESS);
        }
        userInput[byteread - 1] = '\0';
        //write(STDOUT_FILENO, userInput, strlen(userInput));

        if((strcmp(userInput, "exit") == 0)){              //check if the user wants to exit
            //handle ctrl+d or end of file
            write(STDOUT_FILENO,bye, strlen(bye));
            exit(EXIT_SUCCESS);
        }

        //execute the entered command
        status = Command(userInput);
    }
    return 0;
}
