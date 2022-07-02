#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>

int fd;

int open_raw();
void close_raw();
int write_raw(uint8_t* buffer, uint16_t size);
void read_raw(uint8_t* buffer, uint16_t size);