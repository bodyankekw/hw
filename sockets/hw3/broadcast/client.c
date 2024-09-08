#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in recv_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(recv_addr);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(PORT);
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *) &recv_addr, sizeof(recv_addr)) < 0) {
        perror("Bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    while (1) {
        ssize_t recv_len = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &recv_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
        } else {
            buffer[recv_len] = '\0'; // Завершаем строку
            printf("Received broadcast message: %s\n", buffer);
        }
    }

    close(sock);
    return 0;
}
