#include "keyboard.h"
#include "keymap.h"

#define IDT_SIZE 256
#define KERNEL_CORE_SEGMNET_OFFSET 0x08
#define INTERRUPT_GATE 0x8e
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

struct IDT_Entry{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_Entry IDT[IDT_SIZE];

void idt_init(void){
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = KERNEL_CORE_SEGMNET_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    write_port(0x20, 0x11);
    write_port(0xA0, 0x11);

    write_port(0x21, 0x20);
    write_port(0xA1, 0x28);

    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);

    write_port(0x21, 0x01);
    write_port(0xA1, 0x01);

    write_port(0x21, 0xff);
    write_port(0xA1, 0xff);

    idt_address = (unsigned long) IDT;
    idt_ptr[0] = (sizeof (struct IDT_Entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);
}

Keyboard keyboard = Keyboard();

//I seriesly need a fix for this, I find this increadobly ugly
Keyboard* get_Keyboard(){
    return &keyboard;
}

void kb_init(void){
    write_port(0x21, 0xFD);

    keyboard.EnableCurser(0, 15);
    keyboard.UpdateCursor(keyboard.term->GetTerminalColumn(), keyboard.term->GetTerminalRow());
}

bool Shift = false;
void keyboard_handler_main(void){
    unsigned char status;
    unsigned char ukeycode;

    status = read_port(KEYBOARD_STATUS_PORT);
    if(status & 0x01){
        ukeycode = read_port(KEYBOARD_DATA_PORT);

        switch(ukeycode)
        {
            case 0x36: //Shift Pressed
                Shift = true;
                return;

            case 0xB6: //Shift Released
                Shift = false;
                return;

            case 0x0E: //Backspace pressed
                keyboard.term->Backspace();
                keyboard.UpdateCursor(keyboard.term->GetTerminalColumn(), keyboard.term->GetTerminalRow());   
                return;
            case 0x8E: //Backspace Released
                return;
        }

        if(ukeycode >= 128)
            return;

        unsigned char key;

        if(Shift)
        {
            key = keymapUpper[ukeycode];
        }
        else
        {
            key = keymap[ukeycode]; 
        }

        HANDLE handle = {
            status,
            ukeycode,
            key,
        };

        keyboard.FireHandle(handle);

        keyboard.term->Putchar(handle.key);
        keyboard.UpdateCursor(keyboard.term->GetTerminalColumn(), keyboard.term->GetTerminalRow());
    }
}

void Keyboard::EnableCurser(uint8_t cursor_start, uint8_t cursor_end){
    write_port(0x3D4, 0x0A);
    write_port(0x3D5, (read_port(0x3D5) & 0xC0) | cursor_start);

    write_port(0x3D4, 0x0B);
    write_port(0x3D5, (read_port(0x3E0) & 0xE0) | cursor_end);
}

void Keyboard::UpdateCursor(int x, int y)
{
    uint16_t pos = y * VGA_WIDTH + x;
    write_port(0x3D4, 0x0F);
    write_port(0x3D5, (uint8_t) (pos & 0xFF));
    write_port(0x3D4, 0x0E);
    write_port(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void Keyboard::AddHandleListener(Listener* listener){
    listen = listener;
}

void Keyboard::FireHandle(HANDLE handle){
    listen->fire(handle);
}