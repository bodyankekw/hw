#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        perror("sigprocmask failed");
        exit(EXIT_FAILURE);
    }

    printf("SIGINT blocked.\n");

    while (1) {
        sleep(1);
    }

    return 0;
}

