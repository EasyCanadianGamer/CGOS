#ifndef SERIAL_H
#define SERIAL_H
#include <stdint.h>

void serial_init(void);
void serial_putc(char c);
void serial_print(const char *s);
void serial_print_hex64(uint64_t value);
void serial_print_hex(uint64_t value);
#endif