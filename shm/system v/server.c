#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("my_shm", 'A');
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    char *str = (char *) shmat(shmid, (void *)0, 0);

    strcpy(str, "Hi!");

    printf("Waiting for client...\n");
    while(strcmp(str, "Hi!") == 0);

    printf("Received from client: %s\n", str);

    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
