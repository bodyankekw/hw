#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child Process: PID = %d, PPID = %d\n", getpid(), getppid());
        exit(0);
    } else {
        int status;
        printf("Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child exited with status = %d\n", WEXITSTATUS(status));
        } else {
            printf("Child did not exit successfully\n");
        }
    }

    return 0;
}