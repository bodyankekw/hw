#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = "Hello from client";

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    write(client_fd, buffer, strlen(buffer) + 1);

    read(client_fd, buffer, sizeof(buffer));
    printf("Received from server: %s\n", buffer);
    printf("Server address: %s, port: %d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    close(client_fd);
    return 0;
}