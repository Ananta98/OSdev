#include <stdint.h>
#include "../../include/pit.h"

static void timer_callback(struct registers regs) {
        tick++;
        WriteScreen("Tick: ");
        monitor_write_dec(tick);
        WriteScreen("\n");
}

void init_timer(uint32_t frequency) {
    register_interrupt_handler(IRQ0,&timer_callback);
    uint32_t divisor = 1193180/frequency;
    outb(0x43,0x36);

    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40,l);
    outb(0x40,h);
}