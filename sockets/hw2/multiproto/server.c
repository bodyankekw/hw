#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_tcp_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    bytes_read = read(client_socket, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        printf("TCP Client says: %s\n", buffer);
        send(client_socket, buffer, bytes_read, 0);
    } else if (bytes_read == 0) {
        printf("TCP client disconnected.\n");
        close(client_socket);
    } else {
        perror("TCP read error");
        close(client_socket);
    }
}

void handle_udp_client(int udp_socket) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int bytes_read;

    bytes_read = recvfrom(udp_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
    if (bytes_read > 0) {
        printf("UDP Client says: %s\n", buffer);
        sendto(udp_socket, buffer, bytes_read, 0, (struct sockaddr *)&client_addr, client_addr_len); // Отправка данных обратно
    } else {
        perror("UDP read error");
    }
}

int main() {
    int tcp_socket, udp_socket, new_tcp_socket, max_sd;
    struct sockaddr_in address;
    fd_set readfds;
    int addrlen = sizeof(address);

    if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("TCP socket failed");
        exit(EXIT_FAILURE);
    }

    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("UDP socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(tcp_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("TCP bind failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    if (bind(udp_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("UDP bind failed");
        close(udp_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(tcp_socket, 3) < 0) {
        perror("TCP listen failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d for both TCP and UDP...\n", PORT);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(tcp_socket, &readfds);
        FD_SET(udp_socket, &readfds);
        max_sd = tcp_socket > udp_socket ? tcp_socket : udp_socket;

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select error");
            continue;
        }

        if (FD_ISSET(tcp_socket, &readfds)) {
            if ((new_tcp_socket = accept(tcp_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("accept error");
                exit(EXIT_FAILURE);
            }
            printf("New TCP connection\n");
            handle_tcp_client(new_tcp_socket);
        }

        if (FD_ISSET(udp_socket, &readfds)) {
            handle_udp_client(udp_socket);
        }
    }

    close(tcp_socket);
    close(udp_socket);
    return 0;
}
