#include <stdint.h>
#include "packet.h"

void fieldcpy(uint8_t* dst, uint8_t* src, uint16_t len) {
    while(len--) *dst++ = *src++;
}

packet* packet_init(void* data, uint16_t size) {
    packet* p = malloc(sizeof(packet));
    p->data = calloc(size, sizeof(uint8_t));
    p->size = size;
    while (size--) *((char*) p->data + size) = *((char*) data + size);
    p->child = NULL;
    return p;
}

void packet_serialize(packet* p, uint8_t* buf) {
    if (p->child) packet_serialize(p->child, buf + p->size);
    while (p->size--) *((uint8_t*) buf + p->size) = *((uint8_t*) p->data + p->size);
}

int packet_size(packet* p) {
    if (p->child) return p->size + packet_size(p->child);
    return p->size;
}