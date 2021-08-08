#include <idt.h>
#include <ps2.h>
#include <pic.h>
#include <gdt.h>
 
struct IDTEntry IDT[256];
ISRPointer stub_table[256];
ISRPointer isr_table[256];

extern void load_idt(struct IDTPointer idtp);

void spurious_irq7()
{
    if(pic_master_isr() & 0x80)
        pic_master_eoi();
}

void idt_init()
{  
    pic_init();
    
    struct IDTPointer idtp;
    idtp.Size = (sizeof (struct IDTEntry) * 256) - 1;
    idtp.Base = (uint32_t)IDT;

    load_idt(idtp);

    idt_register_handler(0x27, spurious_irq7);
}

void idt_entry(uint8_t number, ISRPointer isr)
{
    IDT[number].OffsetLower = (uint32_t)isr & 0xFFFF;
    IDT[number].Selector = GDT_KERNEL_SELECTOR;
    IDT[number].Reserved = 0;
    IDT[number].Type = ISR_TYPE;
    IDT[number].OffsetHigher = (uint32_t)isr >> 16;
}

void idt_register_handler(uint8_t number, ISRPointer isr)
{
    idt_entry(number, stub_table[number]);
    isr_table[number] = isr;
}