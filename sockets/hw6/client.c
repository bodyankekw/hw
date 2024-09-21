#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>

#define SERVER_IP "192.168.1.41"
#define SERVER_PORT 8080
#define SOURCE_IP "192.168.1.37"
#define SOURCE_PORT 12345
#define BUFFER_SIZE 1024
#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    struct iphdr *ip_header;
    struct udphdr *udp_header;
    char *data;
    int opt = 1;

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));

    char packet[BUFFER_SIZE];

    ip_header = (struct iphdr *) packet;
    udp_header = (struct udphdr *) (packet + sizeof(struct iphdr));
    data = (char *) (packet + sizeof(struct iphdr) + sizeof(struct udphdr));

    strcpy(data, "Hello from raw socket!");

    ip_header->version = 4;
    ip_header->ihl = 5;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->tot_len = htons(IP_HDR_LEN + UDP_HDR_LEN + BUFFER_SIZE);
    ip_header->check = 0;
    ip_header->frag_off = 0;
    ip_header->id = 0;
    ip_header->ttl = 255;
    ip_header->tos = 0;
    inet_pton(AF_INET, SOURCE_IP, &ip_header->saddr);
    inet_pton(AF_INET, SERVER_IP, &ip_header->daddr);

    udp_header->source = htons(SOURCE_PORT);
    udp_header->dest = htons(SERVER_PORT);
    udp_header->len = htons(sizeof(struct udphdr) + strlen(data));
    udp_header->check = 0;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (sendto(sockfd, packet, IP_HDR_LEN + UDP_HDR_LEN + BUFFER_SIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
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
