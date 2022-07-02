#include "rawnet.h"

int open_raw() {
    fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    int yes = 1;
    if (fd < 0) {
        return -1;
    } else {
        return setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &yes, sizeof(yes));
    }
}

void close_raw() {
    close(fd);
}

int write_raw(uint8_t* buffer, uint16_t size) {
    return write(fd, buffer, size);
}

void read_raw(uint8_t* buffer, uint16_t size) {
    read(fd, buffer, size);
}