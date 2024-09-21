#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define CLIENT_SOCKET_PATH "/tmp/local_socket_udp_client"
#define SERVER_SOCKET_PATH "/tmp/local_socket_udp_server"

int main() {
    int client_fd;
    struct sockaddr_un client_addr, server_addr;
    char buffer[1024] = "Hello from client";
    socklen_t addr_len = sizeof(server_addr);

    client_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_LOCAL;
    strncpy(client_addr.sun_path, CLIENT_SOCKET_PATH, sizeof(client_addr.sun_path) - 1);

    unlink(CLIENT_SOCKET_PATH);
    if (bind(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strncpy(server_addr.sun_path, SERVER_SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    sendto(client_fd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&server_addr, addr_len);

    recvfrom(client_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &addr_len);
    printf("Received from server: %s\n", buffer);
    printf("Server address: %s\n", server_addr.sun_path);

    close(client_fd);
    unlink(CLIENT_SOCKET_PATH);
    return 0;
}