#ifndef NTP_H
#define NTP_H

#include "packet.h"


typedef struct ntp_header {
    uint8_t flags;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t reference_identifier;
} ntp_header;

#define NTP_HEADER_SIZE sizeof(ntp_header)
#define NTP_PACKET  (struct packet) {.data = malloc(NTP_HEADER_SIZE), .size = NTP_HEADER_SIZE, .child = NULL}
#define NTP_DEFAULT_HEADER (ntp_header) {.flags = 0x23, .stratum = 0, .poll = 0, .precision = 0, .root_delay = 0, .root_dispersion = 0, .reference_identifier = 0}
#define NTP_INIT (void*) &NTP_DEFAULT_HEADER, NTP_HEADER_SIZE
#define NTP_HEADER(p) ((ntp_header*) p->data)

#endif // NTP_H