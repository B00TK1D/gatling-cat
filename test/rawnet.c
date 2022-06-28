#include <stdio.h>
#include <string.h>
#include "packet.h"
#include "ntp.h"
#include "udp.h"
#include "ip.h"
#include "eth.h"

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

int main() {
    packet* eth = packet_init(&(eth_header) {
            .dst = {0x00, 0x50, 0xe8, 0x02, 0xc6, 0xbd},
            .src = {0xf4, 0xd4, 0x88, 0x6d, 0x93, 0xb9},
            .type = ETH_TYPE_IP}
        , ETH_HEADER_SIZE);

    packet* ip = packet_init(&(ip_header) {
            .version = 0x45,
            .services = 0,
            .len = {0x00, 0x4c},
            .id = {0x85, 0xb6},
            .flags = {0x00, 0x00},
            .ttl = 64,
            .protocol = IP_PROTO_UDP,
            .checksum = {0x59, 0xd1},
            .src_ip = {0xac, 0x14, 0x01, 0x41},
            .dst_ip = {0x22, 0x79, 0xcb, 0x4b}},
        IP_HEADER_SIZE);
    
    packet* udp = packet_init(&(udp_header) {
            .src_port = {0xc8, 0xd2},
            .dst_port = {0x00, 0x7b},
            .len = {0x00, 0x38},
            .checksum = {0x9f, 0xc5}},
        UDP_HEADER_SIZE);


    packet* ntp = packet_init(NTP_INIT);

    packet* data = packet_init("Hello, world!", 20);
    //strcpy(data->data, "Hello, world!");


    eth->child = ip;
    ip->child = udp;
    udp->child = ntp;
    ntp->child = data;
    print_packet(eth);
}