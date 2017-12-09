#ifndef _TERMINAL_H
#define _TERMINAL_H
#include "vga.h"

class Terminal{
public:
    void Initialize(void);
    void Setcolor(uint8_t color);
    void Putentryat(unsigned char c, uint8_t color, size_t x, size_t y);
    void Putchar(unsigned char c);
    void Write(const char* data, size_t size);
    void Writestring(const char* data);
    void Backspace(void);

    size_t GetTerminalRow(void);
    size_t GetTerminalColumn(void);

private:
    size_t terminal_row;
    size_t terminal_column;
    uint8_t terminal_color;
    uint16_t* terminal_buffer;
};


#endif