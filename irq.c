#include <irq.h>
#include <idt.h>
#include <pic.h>

void irq_install_handler(uint8_t irq, ISRPointer isr)
{
    if(irq < 8)
    {
        idt_register_handler(PIC_MASTER_OFFSET + irq, isr);

        uint8_t mask = pic_master_mask_read();
        mask &= ~PIC_MASK(irq);
        pic_master_mask_set(mask);
    }
    else
    {
        irq -= 8;
        
        idt_register_handler(PIC_SLAVE_OFFSET + irq, isr);

        uint8_t mask = pic_slave_mask_read();
        mask &= ~PIC_MASK(irq);
        pic_slave_mask_set(mask);
    }
}