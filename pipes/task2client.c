#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/myfifo"

int main() {
    int fd;
    char buf[1024];

    fd = open(FIFO_NAME, O_RDONLY);
    read(fd, buf, sizeof(buf));
    printf("Клиент получил: %s\n", buf);
    close(fd);

    unlink(FIFO_NAME);

    return 0;
}
