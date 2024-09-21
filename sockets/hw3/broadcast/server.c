#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BROADCAST_IP "255.255.255.255"
#define MESSAGE "Hello, Broadcast!"

int main() {
    int sock;
    struct sockaddr_in broadcast_addr;
    int broadcast_permission = 1;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_permission, sizeof(broadcast_permission)) < 0) {
        perror("setsockopt failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    while (1) {
        if (sendto(sock, MESSAGE, strlen(MESSAGE), 0, (struct sockaddr *) &broadcast_addr, sizeof(broadcast_addr)) <
            0) {
            perror("Broadcast message failed");
        } else {
            printf("Broadcast message sent: %s\n", MESSAGE);
        }
        sleep(2);
    }

    close(sock);
    return 0;
}
