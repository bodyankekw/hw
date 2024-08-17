#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_LINE 1024
#define COMMAND_EXIT "exit"

int main() {
    char *args1[MAX_ARGS];
    char *args2[MAX_ARGS];
    char line[MAX_LINE];
    char *cmd1, *cmd2;

    while (1) {
        printf("mybash> ");
        fflush(stdout);

        if (!fgets(line, MAX_LINE, stdin)) {
            break;
        }
        line[strlen(line) - 1] = '\0';

        cmd1 = strtok(line, "|");
        cmd2 = strtok(NULL, "|");

        if (cmd2 != NULL) {
            int i = 0;
            args1[i] = strtok(cmd1, " ");
            while (args1[i] != NULL && i < MAX_ARGS - 1) {
                i++;
                args1[i] = strtok(NULL, " ");
            }
            args1[i] = NULL;

            i = 0;
            args2[i] = strtok(cmd2, " ");
            while (args2[i] != NULL && i < MAX_ARGS - 1) {
                i++;
                args2[i] = strtok(NULL, " ");
            }
            args2[i] = NULL;

            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            if (fork() == 0) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                execvp(args1[0], args1);
                perror("execvp");
                exit(EXIT_FAILURE);
            }

            if (fork() == 0) {
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                execvp(args2[0], args2);
                perror("execvp");
                exit(EXIT_FAILURE);
            }

            close(pipefd[0]);
            close(pipefd[1]);
            wait(NULL);
            wait(NULL);
        } else {
            int i = 0;
            char *cmd = strtok(line, " ");
            args1[i] = cmd;
            while (args1[i] != NULL) {
                i++;
                args1[i] = strtok(NULL, " ");
            }

            if (strcmp(cmd, COMMAND_EXIT) == 0) {
                exit(0);
            }

            if (fork() == 0) {
                execvp(cmd, args1);
                perror("execvp");
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
            }
        }
    }

    return 0;
}
