#include <stdint.h>
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
}__attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

void init_gdt();

extern void gdt_flush(uint32_t);
static void gdt_set_gate(int32_t,uint32_t,uint32_t,uint8_t,uint8_t);

struct gdt_entry gdt_entries[5];
struct gdt_ptr gdt_ptr;
