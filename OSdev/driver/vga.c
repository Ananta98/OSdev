#include "vga.h"

static void MoveCursor() {
    uint16_t position = cursor_y * 80 + cursor_x;
    outb(0x3D4,0x0F);
    outb(0x3D5,position >> 8);
    outb(0x3D4,0x0E);
    outb(0x3D5,position);
}

static void ScrollScreen() {
    uint8_t attribute = (0 << 4) | (15 & 0x0F);
    char blank = 0x20 | (attribute << 8);
    if (cursor_y >= 25) {
        int j;
        for (j=0*80;j<24*80;j++)
            vga_memory[j] = vga_memory[j+80];
        for (j=24*80;j<25*80;j++)
            vga_memory[j] = blank;
    }
}

void WriteCharacter(char c) {
    uint8_t attributebyte = (0 << 4) | (15 & 0x0F);
    uint16_t attribute = attributebyte << 8;
    uint16_t *location;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == 0x08 && cursor_x ) {
        cursor_x--;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 8) & ~(8-1);
    } else if (c >= ' ') {
        location = vga_memory + (cursor_y * 80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

     if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
    
    ScrollScreen();
    MoveCursor();
}

void ClearScreen() {
    uint8_t attribute = (0 << 4) | (15 & 0x0F);
    char blank = 0x20 | (attribute << 8);
    for (int j=0*80;j<80*25;j++)
        vga_memory[j] = blank;
    cursor_x = 0;
    cursor_y = 0;
    MoveCursor();
}

void WriteScreen(const char *str) {
    for (int j=0;str[j] != '\0';j++)
        WriteCharacter(str[j]);
}
