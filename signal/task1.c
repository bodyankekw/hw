#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigusr1(int sig) {
    (void)sig;
    printf("Got SIGUSR1\n");
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handle_sigusr1;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }


    while (1) {
        sleep(1);
    }

    return 0;
}
