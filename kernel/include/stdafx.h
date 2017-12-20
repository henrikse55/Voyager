#ifndef __STDAFX_H
#define __STDAFX_H

//Includes goes here
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if !defined(__cplusplus)
#include <stdbool.h>
#endif

extern "C"
{
    //Prototypes
    extern char read_port(unsigned short port);
    extern void write_port(unsigned short port, unsigned char value);
    extern void load_idt(unsigned long *idt_ptr);
    
    //Keyboard External(s)
    extern void keyboard_handler(void);
}


#endif