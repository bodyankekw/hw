#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/my_shm"
#define SHM_SIZE 1024

int main() {
    int shm_fd;
    void *ptr;

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);

    ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sprintf(ptr, "Hi!");

    printf("Waiting for client...\n");
    while(strcmp(ptr, "Hi!") == 0);
    printf("Received from client: %s\n", (char *)ptr);
    shm_unlink(SHM_NAME);

    return 0;
}
