
#include "../include/paging.h"
void main() {
    init_gdt(); init_idt();
    ClearScreen();
    // initialize_paging();
    uint32_t *ptr= (uint32_t*)0xA0000000;
    uint32_t do_page_fault = *ptr;
}   