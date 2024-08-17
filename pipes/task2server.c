#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "/tmp/myfifo"

int main() {
    int fd;
    char *message = "Hi!";

    mkfifo(FIFO_NAME, 0666);

    fd = open(FIFO_NAME, O_WRONLY);
    write(fd, message, strlen(message) + 1);
    close(fd);

    return 0;
}
