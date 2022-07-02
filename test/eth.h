#ifndef ETH_H
#define ETH_H

#include "packet.h"

typedef struct eth_header {
    uint8_t dst[6];
    uint8_t src[6];
    uint16_t type;
} eth_header;

#define ETH_TYPE_IP 0x08
#define ETH_TYPE_ARP 0x0806

#define ETH_HEADER_SIZE sizeof(eth_header)
#define ETH_PACKET  (struct packet) {.data = malloc(ETH_HEADER_SIZE), .size = ETH_HEADER_SIZE, .child = NULL}
#define ETH_DEFAULT_HEADER (eth_header) {.dst = 0, .src = 0, .type = ETH_TYPE_IP}
#define ETH_INIT (void*) &ETH_DEFAULT_HEADER, ETH_HEADER_SIZE
#define ETH_HEADER(p) ((eth_header*) p->data)

#define ETH_SET_DST(p, addr) for(uint8_t i = 0; i < 6; i++) (ETH_HEADER(p)->dst[i] = addr[i])
#define ETH_SET_SRC(p, addr) for(uint8_t i = 0; i < 6; i++) (ETH_HEADER(p)->src[i] = addr[i])


#endif // ETH_H