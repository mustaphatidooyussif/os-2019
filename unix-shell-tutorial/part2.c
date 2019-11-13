#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);

static sigjmp_buf env;

void sigint_handler(int signo) {
    siglongjmp(env, 42);
}


char **get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;

    if(command == NULL){
        printf("Malloc failed");
        exit(1);
    }

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}

int cd(char *path) {
    return chdir(path);
}

int main() {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;

    signal(SIGINT, sigint_handler);

    if (sigsetjmp(env, 1) == 42) {
            printf("Restart.\n");
    
    }
    //signal(SIGTSTP, SIG_IGN);

    while (1) {
        input = readline("wish> ");
        if(input == NULL){ //exit by CTRL-D
            exit(1);
        }
        
        command = get_input(input);

        if (strcmp(command[0], "cd") == 0) {
            if (cd(command[1]) < 0) {
                perror(command[1]);
            }

            /* Skip the fork */
            continue;
        }

        child_pid = fork();
        if (child_pid == 0) {
            signal(SIGINT, SIG_DFL);
            //signal(SIGTSTP, SIG_DFL);

            /* Never returns if the call is successful */
            if(execvp(command[0], command) < 0){
                perror(command[0]); //Write good error message
                exit(1);
            }
            //printf("This won't be printed if execvp is successul\n");

        else if(child_pid < 0){
            printf("Fork failed");
            exit(1);
        }
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        free(input);
        free(command);
    }

    return 0;
}