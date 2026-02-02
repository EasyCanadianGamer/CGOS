#ifndef PANIC_H
#define PANIC_H

__attribute__((noreturn))
void panic(const char *msg);

__attribute__((noreturn, format(printf, 1, 2)))
void panicf(const char *fmt, ...);

#endif