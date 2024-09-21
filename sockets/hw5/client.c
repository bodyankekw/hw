#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <sys/socket.h>

#define SERVER_IP "192.168.1.41"
#define SERVER_PORT 8080
#define SOURCE_PORT 12345
#define BUFFER_SIZE 1024
#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    struct udphdr *udp_header;
    char *data;

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    char packet[BUFFER_SIZE];

    udp_header = (struct udphdr *) packet;
    data = (char *) (packet + sizeof(struct udphdr));

    strcpy(data, "Hello from raw socket!");

    udp_header->source = htons(SOURCE_PORT);
    udp_header->dest = htons(SERVER_PORT);
    udp_header->len = htons(sizeof(struct udphdr) + strlen(data));
    udp_header->check = 0;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (sendto(sockfd, packet, sizeof(struct udphdr) + strlen(data), 0, (struct sockaddr *) &server_addr,
               sizeof(server_addr)) < 0) {
        perror("Send failed");
    } else {
        printf("Packet sent\n");
    }

    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);
    if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, &addr_len) < 0) {
        perror("Receive failed");
    } else {
        printf("Received: %s\n", buffer + (IP_HDR_LEN + UDP_HDR_LEN));
    }

    close(sockfd);
    return 0;
}
