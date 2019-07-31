#include "../../include/isr.h"

void register_interrupt_handler(uint8_t n,isr_t handler) {
	interrupt_handlers[n] = handler;
}

void isr_handler(struct registers regs) {
    WriteScreen("received interrupt:");
    monitor_write_dec(regs.int_no);
    WriteScreen("\n");
}

void irq_handler(struct registers regs) {
	if (regs.int_no >= 40) {
		outb(0xA0,0x20);
	}
	outb(0x20,0x20);
	if (interrupt_handlers[regs.int_no] != 0 ) {
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	} 
}
