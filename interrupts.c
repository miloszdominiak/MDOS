#include <interrupts.h>
#include <ports.h>
#include <stdio.h>
#include <ps2.h>

struct IDTEntry IDT[256];
struct idt_ptr idtp;

void interrupts_init()
{
    outb(0x20, 0x11);
        outb(0xA0, 0x11);
        outb(0x21, 0x20);
        outb(0xA1, 40);
        outb(0x21, 0x04);
        outb(0xA1, 0x02);
        outb(0x21, 0x01);
        outb(0xA1, 0x01);
        outb(0x21, 0x0);
        outb(0xA1, 0x0);

    IDT[32].OffsetLower = (uint32_t)irq0 & 0xFFFF;
    IDT[32].Selector = 0x8;
    IDT[32].Reserved = 0;
    IDT[32].Type = 0x8E;
    IDT[32].OffsetHigher = ((uint32_t)irq0 & 0xFFFF0000) >> 16;

    IDT[33].OffsetLower = (uint32_t)irq1 & 0xFFFF;
    IDT[33].Selector = 0x8;
    IDT[33].Reserved = 0;
    IDT[33].Type = 0x8E;
    IDT[33].OffsetHigher = ((uint32_t)irq1 & 0xFFFF0000) >> 16;

    IDT[34].OffsetLower = (uint32_t)irq2 & 0xFFFF;
    IDT[34].Selector = 0x8;
    IDT[34].Reserved = 0;
    IDT[34].Type = 0x8E;
    IDT[34].OffsetHigher = ((uint32_t)irq2 & 0xFFFF0000) >> 16;

    IDT[35].OffsetLower = (uint32_t)irq3 & 0xFFFF;
    IDT[35].Selector = 0x8;
    IDT[35].Reserved = 0;
    IDT[35].Type = 0x8E;
    IDT[35].OffsetHigher = ((uint32_t)irq3 & 0xFFFF0000) >> 16;

    IDT[36].OffsetLower = (uint32_t)irq4 & 0xFFFF;
    IDT[36].Selector = 0x8;
    IDT[36].Reserved = 0;
    IDT[36].Type = 0x8E;
    IDT[36].OffsetHigher = ((uint32_t)irq4 & 0xFFFF0000) >> 16;

    IDT[37].OffsetLower = (uint32_t)irq5 & 0xFFFF;
    IDT[37].Selector = 0x8;
    IDT[37].Reserved = 0;
    IDT[37].Type = 0x8E;
    IDT[37].OffsetHigher = ((uint32_t)irq5 & 0xFFFF0000) >> 16;

    IDT[38].OffsetLower = (uint32_t)irq6 & 0xFFFF;
    IDT[38].Selector = 0x8;
    IDT[38].Reserved = 0;
    IDT[38].Type = 0x8E;
    IDT[38].OffsetHigher = ((uint32_t)irq6 & 0xFFFF0000) >> 16;

    IDT[39].OffsetLower = (uint32_t)irq7 & 0xFFFF;
    IDT[39].Selector = 0x8;
    IDT[39].Reserved = 0;
    IDT[39].Type = 0x8E;
    IDT[39].OffsetHigher = ((uint32_t)irq7 & 0xFFFF0000) >> 16;

    IDT[40].OffsetLower = (uint32_t)irq8 & 0xFFFF;
    IDT[40].Selector = 0x8;
    IDT[40].Reserved = 0;
    IDT[40].Type = 0x8E;
    IDT[40].OffsetHigher = ((uint32_t)irq8 & 0xFFFF0000) >> 16;

    IDT[41].OffsetLower = (uint32_t)irq9 & 0xFFFF;
    IDT[41].Selector = 0x8;
    IDT[41].Reserved = 0;
    IDT[41].Type = 0x8E;
    IDT[41].OffsetHigher = ((uint32_t)irq9 & 0xFFFF0000) >> 16;
    IDT[42].OffsetLower = (uint32_t)irq10 & 0xFFFF;
    IDT[42].Selector = 0x8;
    IDT[42].Reserved = 0;
    IDT[42].Type = 0x8E;
    IDT[42].OffsetHigher = ((uint32_t)irq10 & 0xFFFF0000) >> 16;
    IDT[43].OffsetLower = (uint32_t)irq11 & 0xFFFF;
    IDT[43].Selector = 0x8;
    IDT[43].Reserved = 0;
    IDT[43].Type = 0x8E;
    IDT[43].OffsetHigher = ((uint32_t)irq11 & 0xFFFF0000) >> 16;
    IDT[44].OffsetLower = (uint32_t)irq12 & 0xFFFF;
    IDT[44].Selector = 0x8;
    IDT[44].Reserved = 0;
    IDT[44].Type = 0x8E;
    IDT[44].OffsetHigher = ((uint32_t)irq12 & 0xFFFF0000) >> 16;
    IDT[45].OffsetLower = (uint32_t)irq13 & 0xFFFF;
    IDT[45].Selector = 0x8;
    IDT[45].Reserved = 0;
    IDT[45].Type = 0x8E;
    IDT[45].OffsetHigher = ((uint32_t)irq13 & 0xFFFF0000) >> 16;
    IDT[46].OffsetLower = (uint32_t)irq14 & 0xFFFF;
    IDT[46].Selector = 0x8;
    IDT[46].Reserved = 0;
    IDT[46].Type = 0x8E;
    IDT[46].OffsetHigher = ((uint32_t)irq14 & 0xFFFF0000) >> 16;
    IDT[47].OffsetLower = (uint32_t)irq15 & 0xFFFF;
    IDT[47].Selector = 0x8;
    IDT[47].Reserved = 0;
    IDT[47].Type = 0x8E;
    IDT[47].OffsetHigher = ((uint32_t)irq15 & 0xFFFF0000) >> 16;
   

    idtp.limit = (sizeof (struct IDTEntry) * 256) - 1;
    idtp.base = (uint32_t)IDT;

    load_idt();
}

