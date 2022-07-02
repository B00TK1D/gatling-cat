#ifndef IP_H
#define IP_H

#include "packet.h"


typedef struct ip_header {
    uint8_t version;
    uint8_t services;
    uint16_t len;
    uint16_t id;
    uint16_t flags;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src;
    uint32_t dst;
} ip_header;

#define IP_HEADER_SIZE sizeof(ip_header)
#define IP_PACKET  (struct packet) {.data = malloc(IP_HEADER_SIZE), .size = IP_HEADER_SIZE, .child = NULL}
#define IP_DEFAULT_HEADER (ip_header) {.version = 0x45, .services = 0, .len = 0, .id = 0, .flags = 0, .ttl = 64, .protocol = 0, .checksum = 0, .src = 0, .dst = 0}
#define IP_INIT (void*) &IP_DEFAULT_HEADER, IP_HEADER_SIZE
#define IP_HEADER(p) ((ip_header*) p->data)

#define IP_PROTO_UDP 17
#define IP_PROTO_TCP 6
#define IP_PROTO_ICMP 1

#endif // IP_H