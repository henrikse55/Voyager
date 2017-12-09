#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "stdafx.h"
#include "terminal.h"

struct Handle // The handle which contains data about the keyboard interrupt
{
    unsigned char status;
    char keycode;
    unsigned char key;
};
#define HANDLE Handle

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