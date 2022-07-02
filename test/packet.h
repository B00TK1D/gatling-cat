#ifndef PACKET_H
#define PACKET_H

#include <stdlib.h>

typedef struct packet {
    void* data;
    uint16_t size;
    struct packet* child;
} packet;


packet* packet_init(void* data, uint16_t size);

void packet_serialize(packet* p, uint8_t* buf);

int packet_size(packet* p);

#endif // PACKET_H