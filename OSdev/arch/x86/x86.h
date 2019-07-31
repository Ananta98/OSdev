#include <stdint.h>

inline void outb(uint16_t port,uint8_t value) {
    asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(port) );
}

inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1,%0;" 
    : "=a" (ret) 
    : "Nd"(port));
    return ret;
}

inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1,%0;" 
    : "=a"(ret) 
    : "Nd"(port));
    return ret;
}
