#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define BUFFER_SIZE 100
#define IP_HEADER_SIZE 20
#define UDP_HEADER_SIZE 8

int main() {
    int sockfd;
    unsigned char buffer[BUFFER_SIZE];
    struct sockaddr_in source_addr;
    socklen_t addr_len = sizeof(source_addr);

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Raw socket created and listening for UDP packets...\n");

    while (1) {
        ssize_t data_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &source_addr, &addr_len);
        if (data_len < 0) {
            perror("Failed to receive packets");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        struct iphdr *ip_header = (struct iphdr *) buffer;
        struct udphdr *udp_header = (struct udphdr *) (buffer + (ip_header->ihl * 4));

        printf("Received UDP packet:\n");
        printf("Source IP: %s\n", inet_ntoa(*(struct in_addr *) &ip_header->saddr));
        printf("Destination IP: %s\n", inet_ntoa(*(struct in_addr *) &ip_header->daddr));

        size_t packet_size = ntohs(udp_header->len) - UDP_HEADER_SIZE;

        char *data = (char *) buffer + IP_HEADER_SIZE + UDP_HEADER_SIZE;
        printf("Packet data: %zu\n" , packet_size);
        for (int i = 0; i < packet_size; i++) {
            if (data[i] >= 32 && data[i] <= 126) {
                printf("%c", data[i]);
            } else {
                printf(".");
            }
        }
        printf("\n--------------------------------------------\n");
    }

    close(sockfd);
    return 0;
}
