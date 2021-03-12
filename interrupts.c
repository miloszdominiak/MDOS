#include <interrupts.h>
#include <stdio.h>
#include <ps2.h>
#include <pic.h>
#include <gdt.h>
 
struct IDTEntry IDT[256];

void interrupts_init()
{  
    pic_remap();
    
    struct IDTPointer idtp;
    idtp.Size = (sizeof (struct IDTEntry) * 256) - 1;
    idtp.Base = (uint32_t)IDT;

    load_idt(idtp);

    IDT[PIC_MASTER_OFFSET + 7].OffsetLower = (uint32_t)unmaskable & 0xFFFF;
        IDT[PIC_MASTER_OFFSET + 7].Selector = GDT_KERNEL_SELECTOR;
        IDT[PIC_MASTER_OFFSET + 7].Reserved = 0;
        IDT[PIC_MASTER_OFFSET + 7].Type = ISR_TYPE;
        IDT[PIC_MASTER_OFFSET + 7].OffsetHigher = (uint32_t)unmaskable >> 16;
}

void interrupt_install_handler(uint8_t irq, void (*isr)())
{
    if(irq < 8)
    {
        IDT[PIC_MASTER_OFFSET + irq].OffsetLower = (uint32_t)isr & 0xFFFF;
        IDT[PIC_MASTER_OFFSET + irq].Selector = GDT_KERNEL_SELECTOR;
        IDT[PIC_MASTER_OFFSET + irq].Reserved = 0;
        IDT[PIC_MASTER_OFFSET + irq].Type = ISR_TYPE;
        IDT[PIC_MASTER_OFFSET + irq].OffsetHigher = (uint32_t)isr >> 16;

        uint8_t mask = pic_master_mask_read();
        mask &= ~PIC_MASK(irq);
        pic_master_mask_set(mask);
    }
    else
    {
        irq -= 8;
        IDT[PIC_SLAVE_OFFSET + irq].OffsetLower = (uint32_t)isr & 0xFFFF;
        IDT[PIC_SLAVE_OFFSET + irq].Selector = GDT_KERNEL_SELECTOR;
        IDT[PIC_SLAVE_OFFSET + irq].Reserved = 0;
        IDT[PIC_SLAVE_OFFSET + irq].Type = ISR_TYPE;
        IDT[PIC_SLAVE_OFFSET + irq].OffsetHigher = (uint32_t)isr >> 16;

        uint8_t mask = pic_slave_mask_read();
        mask &= ~PIC_MASK(irq);
        pic_slave_mask_set(mask);
    }   
}
