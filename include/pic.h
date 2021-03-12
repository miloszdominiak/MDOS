#ifndef PIC_H
#define PIC_H

#include <stdint.h>

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA 0xA1

#define PIC_COMMAND_INIT 0x11
#define PIC_COMMAND_ISR 0x0B
#define PIC_COMMAND_EOI 0x20

#define PIC_MASTER_OFFSET 0x20
#define PIC_SLAVE_OFFSET (PIC_MASTER_OFFSET + 8)

#define PIC_CASCADE_IRQ 2
#define PIC_MASTER_CASCADE (1 << PIC_CASCADE_IRQ)
#define PIC_SLAVE_CASCADE PIC_CASCADE_IRQ

#define PIC_MODE_X86 0x01

#define PIC_MASK_NONE 0x00
#define PIC_MASK_ALL 0xFF
#define PIC_MASK(a) (1 << a)

void pic_remap();

void pic_master_eoi();
void pic_slave_eoi();
void pic_master_mask_set(uint8_t mask);
void pic_slave_mask_set(uint8_t mask);
uint8_t pic_master_mask_read();
uint8_t pic_slave_mask_read();

#endif