#include "terminal.h"
#include "keyboard.h"

#define keyboard get_Keyboard()

static inline bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile (
        "pushf\n\t"
        "pop %0"
        : "=g"(flags)
    );
    return flags & (1 << 9);
}

Terminal terminal = Terminal();

void Terminal_Keyboard(HANDLE handle)
{
    
}

extern "C"
void kernel_main(void)
{
    terminal.Initialize();
    keyboard->term = &terminal;

    Listener listener = {
        0,
        &Terminal_Keyboard
    };

    keyboard->AddHandleListener(&listener);

    idt_init();
    kb_init();

    if(are_interrupts_enabled())
    {
        terminal.Writestring("Interrupts are enabled!\n");
    }else{
        terminal.Writestring("Interrupts are not enabled!\n");
    }
    terminal.Writestring("Hello, kernel world!\n");

    while(1);
}