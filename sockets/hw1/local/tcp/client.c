#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/local_socket"

int main() {
    int client_fd;
    struct sockaddr_un server_addr;
    char buffer[1024] = "Hello from client";

    client_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    write(client_fd, buffer, strlen(buffer) + 1);

    read(client_fd, buffer, sizeof(buffer));
    printf("Received from server: %s\n", buffer);
    printf("Server address: %s\n", server_addr.sun_path);

    close(client_fd);
    return 0;
}