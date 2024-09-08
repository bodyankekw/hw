#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MULTICAST_IP "239.255.255.250"
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in multicast_addr;
    struct ip_mreq mreq;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(multicast_addr);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(PORT);
    multicast_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *) &multicast_addr, sizeof(multicast_addr)) < 0) {
        perror("Bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    while (1) {
        ssize_t recv_len = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &multicast_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
        } else {
            buffer[recv_len] = '\0';
            printf("Received multicast message: %s\n", buffer);
        }
    }

    close(sock);
    return 0;
}
