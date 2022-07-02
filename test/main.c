#include <errno.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include "packet.h"
#include "ntp.h"
#include "udp.h"
#include "ip.h"
#include "eth.h"
#include "rawnet.h"

void print_packet(packet* p) {
    uint16_t size = packet_size(p);
    uint8_t* buf = malloc(size);
    packet_serialize(p, buf);
    for (int i = 0; i < size; i++) {
        if (i % 16 == 0) printf("\n%04x: ", i);
        if (i % 16 == 8) printf(" ");
        printf("%02x ", buf[i]);
    }
    printf("\n\n");
}

void send_packet(packet* p) {
    uint16_t size = packet_size(p);
    uint8_t* buf = malloc(size);
    printf("%d\n", open_raw());
    packet_serialize(p, buf);
    printf("%d\n", write_raw(buf, size));
    close_raw();
}

int main() {
    /*
    packet* eth = packet_init(&(eth_header) {
            .dst = {0x00, 0x50, 0xe8, 0x02, 0xc6, 0xbd},
            .src = {0xf4, 0xd4, 0x88, 0x6d, 0x93, 0xb9},
            .type = ETH_TYPE_IP},
        ETH_HEADER_SIZE);
    */

   packet* eth = packet_init(NULL, 0);

    packet* ip = packet_init(&(ip_header) {
            .version = 0x45,
            .services = 0,
            .len = 0x4c,
            .id = 0xb685,
            .flags = 0x00,
            .ttl = 64,
            .protocol = IP_PROTO_UDP,
            .checksum = 0xd159,
            .src = 0x410114ac,
            .dst = 0x08080808},
        IP_HEADER_SIZE);
    
    packet* udp = packet_init(&(udp_header) {
            .src_port = 0xd2c8,
            .dst_port = 0x7b,
            .len = 0x38,
            .checksum = 0xc59f},
        UDP_HEADER_SIZE);


    packet* ntp = packet_init(NTP_INIT);

    packet* data = packet_init("Hello, world!", 20);

    eth->child = ip;
    ip->child = udp;
    udp->child = ntp;
    ntp->child = data;
    print_packet(eth);

    uint8_t* buf = malloc(packet_size(eth));
    packet_serialize(eth, buf);

    //send_packet(eth);

    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    int yes = 1;
    printf("%d\n", fd);
    int res = setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &yes, sizeof(yes));

    printf("%d\n", res);

    //res = write(fd, buf, packet_size(eth));
    struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(123), .sin_addr = {.s_addr = inet_addr("8.8.8.8")}};
    sendto(fd, buf, packet_size(eth), 0, (struct sockaddr*)&addr, sizeof(addr));

    printf("%d\n", res);
    printf("%s\n", strerror(errno));


    /*
    packet* eth2 = packet_init(buf, ETH_HEADER_SIZE);
    buf += ETH_HEADER_SIZE;
    packet* ip2 = packet_init(buf, IP_HEADER_SIZE);
    buf += IP_HEADER_SIZE;
    packet* udp2 = packet_init(buf, UDP_HEADER_SIZE);
    buf += UDP_HEADER_SIZE;
    packet* ntp2 = packet_init(buf, NTP_HEADER_SIZE);
    buf += NTP_HEADER_SIZE;
    packet* data2 = packet_init(buf, 20);
    buf += 20;

    printf("\n\n");
    print_packet(data2);
    */
}