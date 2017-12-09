#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#define HANDLE Handle

#include "stdafx.h"
#include "terminal.h"

enum SpecialKeys
{
    Shift_Pressed = 0x36,
    Shift_Released = 0xB6,
    Backspace_Pressed = 0x0E,
    Backspace_Released = 0x8E,
    Escape_Pressed = 0x01,
    Escape_Released = 0x81,
};

//TODO rewrite this hgandle to send "special" keys easier
//Possibly add an enum list or something
struct Handle // The handle which contains data about the keyboard interrupt
{
    unsigned char status;
    unsigned char keycode;
    unsigned char key;
    SpecialKeys skey;
};

//TODO Implement proxy levels deppending on cpu access levels
struct Listener //Some data about the listener to locate the functions
{
    size_t proxy; // Priority, lower is better
    void (*fire)(HANDLE);
};

class Keyboard
{   
public:
    void AddHandleListener(Listener* listener);
    void FireHandle(HANDLE handle);

    void EnableCurser(uint8_t cursor_start, uint8_t cursor_end);
    void UpdateCursor(int x, int y);

    Terminal* term;
protected:

private:
    Listener *listen;
};

extern "C" void keyboard_handler_main(void);
void idt_init(void);
void kb_init(void);
Keyboard* get_Keyboard();

#endif