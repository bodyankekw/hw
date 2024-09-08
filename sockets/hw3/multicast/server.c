#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MULTICAST_IP "239.255.255.250"
#define MESSAGE "Hello, Multicast!"

int main() {
    int sock;
    struct sockaddr_in multicast_addr;
    int multicast_ttl = 1;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &multicast_ttl, sizeof(multicast_ttl)) < 0) {
        perror("setsockopt failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(PORT);
    multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_IP);

    while (1) {
        if (sendto(sock, MESSAGE, strlen(MESSAGE), 0, (struct sockaddr *) &multicast_addr, sizeof(multicast_addr)) <
            0) {
            perror("Multicast message failed");
        } else {
            printf("Multicast message sent: %s\n", MESSAGE);
        }
        sleep(2);
    }

    close(sock);
    return 0;
}
