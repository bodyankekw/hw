#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("my_shm", 'A');
    int shmid = shmget(key, SHM_SIZE, 0666);
    char *str = (char *) shmat(shmid, (void *)0, 0);

    printf("Received from server: %s\n", str);

    strcpy(str, "Hello!");

    shmdt(str);

    return 0;
}
