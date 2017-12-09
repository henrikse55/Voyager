#include "terminal.h"

static inline size_t strlen(const char* str){
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void Terminal::Initialize(void){
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for(size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for(size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void Terminal::Setcolor(uint8_t color){
    terminal_color = color;
}

void Terminal::Putentryat(unsigned char c, uint8_t color, size_t x, size_t y){
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void Terminal::Putchar(unsigned char c){
    if(c == '\n')
    {
        terminal_row++;
        terminal_column = 0;
        return;
    }
    
    Putentryat(c, terminal_color, terminal_column, terminal_row);
    if(++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        if(++terminal_row == VGA_HEIGHT)
        {
            terminal_row = 0;
        }
    }
}

void Terminal::Write(const char* data, size_t size){
    for(size_t i = 0; i < size; i++)
        Putchar(data[i]);
}

void Terminal::Writestring(const char* data){
    Write(data, strlen(data));
}

void Terminal::Backspace(void){
    if(--terminal_column == 0)
    {
        --terminal_row;
        terminal_column = VGA_WIDTH;
    }
    Putentryat(' ', terminal_color, terminal_column, terminal_row);
}

size_t Terminal::GetTerminalColumn(void){
    return terminal_column;
}

size_t Terminal::GetTerminalRow(void){
    return terminal_row;
}
