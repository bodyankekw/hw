#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    sendto(sock, "Hello from UDP client", strlen("Hello from UDP client"), 0,
           (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Message sent\n");

    socklen_t addr_len = sizeof(serv_addr);
    ssize_t bytes_read = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, &addr_len);
    printf("Message received: %s\n", buffer);

    close(sock);
    return 0;
}
