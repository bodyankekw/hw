#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void create_child_process() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child Process: PID = %d, PPID = %d\n", getpid(), getppid());
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child PID = %d exited with status = %d\n", pid, WEXITSTATUS(status));
        } else {
            printf("Child PID = %d did not exit successfully\n", pid);
        }
    }
}

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid1 == 0) {
        printf("Process1: PID = %d, PPID = %d\n", getpid(), getppid());
        create_child_process(); // p3
        create_child_process(); // p4
        exit(0);
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid2 == 0) {
        printf("Process2: PID = %d, PPID = %d\n", getpid(), getppid());
        create_child_process(); // p5
        exit(0);
    }

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    printf("Parent Process: PID = %d\n", getpid());

    return 0;
}