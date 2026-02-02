#include <limine.h>
#include <stdint.h>

#define COM1 0x3F8

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init(void) {
    outb(COM1 + 1, 0x00);  // Disable interrupts
    outb(COM1 + 3, 0x80);  // Enable DLAB
    outb(COM1 + 0, 3);     // Baud 38400 low
    outb(COM1 + 1, 0);     // High
    outb(COM1 + 3, 0x03);  // 8N1, DLAB off
    outb(COM1 + 2, 0xC7);  // FIFO
    outb(COM1 + 4, 0x0B);  // IRQs on, RTS/DSR
}

void serial_putc(char c) {
    while (!(inb(COM1 + 5) & 0x20));
    outb(COM1, c);
}

void serial_print(const char *s) {
    while (*s) serial_putc(*s++);

}

void serial_print_hex(uint64_t value) {
    serial_print("0x");

    // Special case for zero
    if (value == 0) {
        serial_putc('0');
        return;
    }

    // Print digits from most significant to least (no leading zeros)
    char buf[16];           // max 16 hex digits for 64-bit
    int i = 0;

    while (value > 0) {
        uint8_t digit = value & 0xF;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        value >>= 4;
    }

    // Reverse to print MSB first
    while (i > 0) {
        serial_putc(buf[--i]);
    }
}


void serial_print_hex64(uint64_t value) {
    serial_print("0x");

    for (int i = 15; i >= 0; i--) {
        uint8_t digit = (value >> (i * 4)) & 0xF;
        serial_putc(digit < 10 ? '0' + digit : 'A' + digit - 10);
    }
}