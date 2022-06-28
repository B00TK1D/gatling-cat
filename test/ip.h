#ifndef IP_H
#define IP_H

#include "packet.h"


typedef struct ip_header {
    uint8_t version;
    uint8_t services;
    uint8_t len[2];
    uint8_t id[2];
    uint8_t flags[2];
    uint8_t ttl;
    uint8_t protocol;
    uint8_t checksum[2];
    uint8_t src_ip[4];
    uint8_t dst_ip[4];
} ip_header;

#define IP_HEADER_SIZE sizeof(ip_header)
#define IP_PACKET  (struct packet) {.data = malloc(IP_HEADER_SIZE), .size = IP_HEADER_SIZE, .child = NULL}
#define IP_DEFAULT_HEADER (ip_header) {.version = 0x45, .services = 0x00, .len = {0x00, 0x00}, .id = {0x00, 0x00}, .flags = {0x00, 0x00}, .ttl = 0x40, .protocol = 0, .checksum = {0x00, 0x00}, .src_ip = {0x00, 0x00, 0x00, 0x00}, .dst_ip = {0x00, 0x00, 0x00, 0x00}}
#define IP_INIT (void*) &IP_DEFAULT_HEADER, IP_HEADER_SIZE
#define IP_HEADER(p) ((ip_header*) p->data)

#define IP_PROTO_UDP 17
#define IP_PROTO_TCP 6
#define IP_PROTO_ICMP 1

#endif // IP_H