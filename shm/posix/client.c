#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHM_NAME "/my_shm"
#define SHM_SIZE 1024

int main() {
    int shm_fd;
    void *ptr;

    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);

    ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    printf("Received from server: %s\n", (char *)ptr);
    sprintf(ptr, "Hello!");

    return 0;
}
