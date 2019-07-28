#include <stdint.h>
#include "port.h"

static int cursor_x,cursor_y;
static uint16_t* vga_memory = (uint16_t*)0xC03FF000;

void WriteScreen(const char *str);
void ClearScreen();
void WriteCharacter(char c);