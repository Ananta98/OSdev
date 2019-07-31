#include <stdint.h>
#include "../arch/x86/x86.h"

static int cursor_x,cursor_y;
static uint16_t* vga_memory = (uint16_t*)0xB8000;

void WriteScreen(const char *str);
void ClearScreen();
void WriteCharacter(char c);
void monitor_write_dec(uint32_t n);
void monitor_write_hex(uint32_t n);