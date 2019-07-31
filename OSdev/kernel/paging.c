#include "../include/paging.h"
#include "../include/kheap.h"

uint32_t *frames;
uint32_t nframes;

extern uint32_t placement_address;
page_directory_t *current_directory=0;
page_directory_t *kernel_directory=0;

#define INDEX_FROM_BIT(a)  (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

static void test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= (0x1 << off);
}

static uint32_t first_frame() {
    uint32_t i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        if (frames[i] != 0xFFFFFFFF) {
            for (j = 0; j < 32; j++) {
                uint32_t test = 0x1 << j;
                if (!(frames[i] & test))
                    return i * 4 + 8 + j;
            }
        }
    }
}

void alloc_frame(page_t *page,int is_kernel,int is_writeable) {
    if (page->frame != 0)
        return;
    else {
        uint32_t idx = first_frame();
        if (idx == (uint32_t)-1) {
            WriteScreen("No free frames!");
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw =  (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 1 : 0;
        page->frame = idx;
    }
}

void free_frame(page_t *page) {
    uint32_t frame;
    if (!(frame = page->frame))
        return;
    else {
        clear_frame(frame);
        page->frame = 0;
    }
}

void initialize_paging() {
    uint32_t mem_end_page = 0x1000000;
    nframes = mem_end_page / 0x1000;
    frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames,0,INDEX_FROM_BIT(nframes));
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    current_directory = kernel_directory;
    int i = 0;
    while (i < placement_address) {
        alloc_frame(get_page(i,1,kernel_directory),0,0);
        i += 0x1000;
    }
    register_interrupt_handler(14,page_fault);
    switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
   // Turn the address into an index.
   address /= 0x1000;
   // Find the page table containing this address.
   uint32_t table_idx = address / 1024;
   if (dir->tables[table_idx]) // If this table is already assigned
   {
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else if(make)
   {
       uint32_t tmp;
       dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
       memset(dir->tables[table_idx], 0, 0x1000);
       dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else
   {
       return 0;
   }
}

void page_fault(struct registers regs)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    WriteScreen("Page fault! ( ");
    if (present) {WriteScreen("present ");}
    if (rw) {WriteScreen("read-only ");}
    if (us) {WriteScreen("user-mode ");}
    if (reserved) {WriteScreen("reserved ");}
    WriteScreen(") at 0x");
    monitor_write_hex(faulting_address);
    WriteScreen("\n");
    WriteScreen("Page fault");
}