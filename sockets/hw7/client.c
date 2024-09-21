#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/ether.h>

#define SERVER_IP "192.168.1.41"
#define CLIENT_IP "192.168.1.37"
#define SRC_MAC "08:00:27:16:A3:3C"
#define DST_MAC "08:00:27:68:A4:38"
#define SOURCE_PORT 12345
#define SERVER_PORT 8080
#define BUFFER_SIZE 100
#define UDP_HDR_LEN 8
#define IP_HDR_LEN 20
#define ETH_HDR_LEN 14
#define INTERFACE_NAME "enp0s3"

short checksum(struct iphdr *iph) {
    long tmp, csum = 0;
    unsigned short *ptr = (short *)iph;
    for (int i = 0; i < 10; ++i) {
        csum += *ptr;
        ++ptr;
    }
    while ((tmp = csum >> 16)) {
        csum = (csum & 0xFFFF) + tmp;
    }
    return ~csum;
}

int main() {
    int sockfd;
    struct sockaddr_ll server_addr;
    struct ifreq ifr;
    char buffer[BUFFER_SIZE + ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN];
    char recv_buffer[BUFFER_SIZE + ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN];
    socklen_t sock_len = sizeof(server_addr);

    struct ethhdr *eth_header = (struct ethhdr *)buffer;
    struct iphdr *ip_header = (struct iphdr *)(buffer + ETH_HDR_LEN);
    struct udphdr *udp_header = (struct udphdr *)(buffer + ETH_HDR_LEN + IP_HDR_LEN);
    char *data = buffer + ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN;

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, INTERFACE_NAME, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("Failed to get interface index");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sll_ifindex = ifr.ifr_ifindex;
    server_addr.sll_halen = ETH_ALEN;
    ether_aton_r(DST_MAC, (struct ether_addr *)server_addr.sll_addr);

    ether_aton_r(DST_MAC, (struct ether_addr *)eth_header->h_dest);
    ether_aton_r(SRC_MAC, (struct ether_addr *)eth_header->h_source);
    eth_header->h_proto = htons(ETH_P_IP);

    ip_header->version = 4;
    ip_header->ihl = 5;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->tot_len = htons(IP_HDR_LEN + UDP_HDR_LEN + BUFFER_SIZE);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->tos = 0;
    inet_pton(AF_INET, CLIENT_IP, &ip_header->saddr);
    inet_pton(AF_INET, SERVER_IP, &ip_header->daddr);
    ip_header->check = checksum(ip_header);

    udp_header->source = htons(SOURCE_PORT);
    udp_header->dest = htons(SERVER_PORT);
    udp_header->len = htons(UDP_HDR_LEN + BUFFER_SIZE);
    udp_header->check = 0;

    strncpy(data, "Hello from raw socket!", BUFFER_SIZE);

    if (sendto(sockfd, buffer, ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, sock_len) < 0) {
        perror("Send failed");
    } else {
        printf("Packet sent successfully\n");
    }

    if (recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&server_addr, &sock_len) < 0) {
        perror("Receive failed");
    } else {
        printf("Received message: %s\n", recv_buffer + ETH_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN);
    }

    close(sockfd);
    return 0;
}
