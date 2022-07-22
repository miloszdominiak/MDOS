#include <idt.h>
#include <irq.h>
#include <ps2.h>
#include <pic.h>
#include <gdt.h>
#include <stdio.h>
 
struct IDTEntry IDT[256];
ISRPointer stub_table[256];
ISRPointer isr_table[256];
struct IDTPointer idtp;

extern void load_idt(struct IDTPointer idtp);
extern void load_stubs();

void spurious_irq7()
{
    //printf("s");
    if(pic_master_isr() & 0x80)
        pic_master_eoi();
}

void handler()
{
    printf("unha");
    pic_master_eoi();
}

void handler2()
{
    printf("ha2");
    pic_slave_eoi();
    pic_master_eoi();
}

void idt_entry(uint8_t number, ISRPointer isr)
{
    IDT[number].OffsetLower = (uint32_t)isr & 0xFFFF;
    IDT[number].Selector = GDT_KERNEL_SELECTOR;
    IDT[number].Reserved = 0;
    IDT[number].Type = ISR_TYPE;
    IDT[number].OffsetHigher = (uint32_t)isr >> 16;
}

void idt_init()
{  
    pic_init();
    printf("Po picu\n");

    load_stubs();
    printf("Po stubsach\n");

    for(int i = 0; i < 256; i++)
    {
        idt_entry(i, stub_table[i]);
        printf("J");
        isr_table[i] = handler;
    }

    printf("6");
    
    
    idtp.Size = (sizeof (struct IDTEntry) * 256) - 1;
    idtp.Base = (uint32_t)IDT;


    printf("8");

    // for(int i = 0; i < 8; i++)
    //     idt_register_handler(0x20 + i, handler);

    // for(int i = 8; i < 16; i++)
    //     idt_register_handler(0x28 + i, handler2);

    

    idt_register_handler(0x27, spurious_irq7);

    printf("7");

    printf("Size: %4\n", idtp.Size);
    printf("Base: %4\n", idtp.Base);

    printf("&IDTP: %4\n", &idtp);
    printf("IDTP: %4\n", idtp);
    load_idt(idtp);
    //idt_register_handler(0x0, handler2);

    printf("9");
}

void idt_register_handler(uint8_t number, ISRPointer isr)
{
    idt_entry(number, stub_table[number]);
    isr_table[number] = isr;
}