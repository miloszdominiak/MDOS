#include <pic.h>
#include <stdint.h>
#include <ports.h>

void pic_master_send(uint8_t command)
{
    outb(PIC_MASTER_COMMAND, command);
}

void pic_master_data(uint8_t data)
{
    outb(PIC_MASTER_DATA, data);
}

void pic_slave_send(uint8_t command)
{
    outb(PIC_SLAVE_COMMAND, command);
}

void pic_slave_data(uint8_t data)
{
    outb(PIC_SLAVE_DATA, data);
}

void pic_init()
{
    pic_master_send(PIC_COMMAND_INIT);
    pic_slave_send(PIC_COMMAND_INIT);
}

void pic_offset(uint8_t master_offset, uint8_t slave_offset)
{
    pic_master_data(master_offset);
    pic_slave_data(slave_offset);
}

void pic_cascade(uint8_t master_cascade, uint8_t slave_cascade)
{
    pic_master_data(master_cascade);
    pic_slave_data(slave_cascade);
}

void pic_mode(uint8_t master_mode, uint8_t slave_mode)
{
    pic_master_data(master_mode);
    pic_slave_data(slave_mode);
}

void pic_master_mask_set(uint8_t mask)
{
    pic_master_data(mask);
}

void pic_slave_mask_set(uint8_t mask)
{
    pic_slave_data(mask);
}

uint8_t pic_master_mask_read()
{
    return inb(PIC_MASTER_DATA);
}

uint8_t pic_slave_mask_read()
{
    return inb(PIC_SLAVE_DATA);
}

void pic_remap()
{
    pic_init();
    pic_offset(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
    pic_cascade(PIC_MASTER_CASCADE, PIC_SLAVE_CASCADE);
    pic_mode(PIC_MODE_X86, PIC_MODE_X86);
    pic_master_mask_set(~PIC_MASK(2));
    pic_slave_mask_set(PIC_MASK_ALL);
}

void pic_master_eoi()
{
    pic_master_send(PIC_COMMAND_EOI);
}

void pic_slave_eoi()
{
    pic_slave_send(PIC_COMMAND_EOI);
}