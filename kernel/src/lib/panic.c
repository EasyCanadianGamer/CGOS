#include <lib/serial.h>
#include <stdarg.h>

__attribute__((noreturn, format(printf,1,2)))
void panicf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    serial_print("\r\n*** CGOS KERNEL PANIC ***\r\n");
    serial_print("Message: ");

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    const char *s = va_arg(ap, const char *);
                    serial_print(s ? s : "(null)");
                    break;
                }
                case 'x':
                case 'p': {
                    uint64_t val = va_arg(ap, uint64_t);
                    serial_print_hex(val);
                    break;
                }
                default:
                    serial_putc('%');
                    serial_putc(*fmt);
            }
        } else {
            serial_putc(*fmt);
        }
        fmt++;
    }

    serial_print("\r\nHalting...\r\n");

    asm volatile("cli");
    for (;;) asm volatile("hlt");
}

__attribute__((noreturn))
void panic(const char *msg) {
    panicf("%s", msg ? msg : "(null)");
}