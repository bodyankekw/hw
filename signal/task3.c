#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t set;
    int sig;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        perror("sigprocmask failed");
        exit(EXIT_FAILURE);
    }

    printf("SIGUSR1 blocked\n");

    while (1) {
        if (sigwait(&set, &sig) == 0) {
            printf("Got SIGUSR1\n");
        } else {
            perror("error while waiting for signal");
        }
    }

    return 0;
}
