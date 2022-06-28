#ifndef UDP_H
#define UDP_H

#include "packet.h"


typedef struct udp_header {
    uint8_t src_port[2];
    uint8_t dst_port[2];
    uint8_t len[2];
    uint8_t checksum[2];
} udp_header;

#define UDP_HEADER_SIZE sizeof(udp_header)
#define UDP_PACKET  (struct packet) {.data = malloc(UDP_HEADER_SIZE), .size = UDP_HEADER_SIZE, .child = NULL}
#define UDP_DEFAULT_HEADER (udp_header) {.src_port = 0, .dst_port = 0, .len = 0, .checksum = 0}
#define UDP_INIT (void*) &UDP_DEFAULT_HEADER, UDP_HEADER_SIZE
#define UDP_HEADER(p) ((udp_header*) p->data)


#endif // UDP_H