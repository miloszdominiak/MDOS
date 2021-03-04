#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <ports.h>
#include <stdio.h>
#include <ps2.h>

struct __attribute__ ((packed)) IDTEntry
{
    uint16_t OffsetLower;
    uint16_t Selector;
    uint8_t Reserved;
    uint8_t Type;
    uint16_t OffsetHigher;
};

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void interrupts_init();
void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();
void irq1_handler();
void load_idt();

void irq0_handler()
{
    //printf("0 ");
    outb(0x20, 0x20);
}

void irq1_handler()
{
    printf("%1 ", ps2_controller_read());
    outb(0x20, 0x20);
}

void irq2_handler()
{
    printf("2 ");
    outb(0x20, 0x20);
}

void irq3_handler()
{
    printf("3 ");
    outb(0x20, 0x20);
}

void irq4_handler()
{
    printf("4 ");
    outb(0x20, 0x20);
}

void irq5_handler()
{
    printf("5 ");
    outb(0x20, 0x20);
}

void irq6_handler()
{
    printf("6 ");
    outb(0x20, 0x20);
}

void irq7_handler()
{
    //printf("7 ");
    outb(0x20, 0x20);
}

void irq8_handler()
{
    printf("8 ");
    outb(0x20, 0x20);
}

void irq9_handler()
{
    printf("9 ");
    outb(0x20, 0x20);
}

void irq10_handler()
{
    printf("10 ");
    outb(0x20, 0x20);
}

void irq11_handler()
{
    printf("11 ");
    outb(0x20, 0x20);
}

void irq12_handler()
{
    printf("12 ");
    outb(0x20, 0x20);
}

void irq13_handler()
{
    printf("13 ");
    outb(0x20, 0x20);
}

void irq14_handler()
{
    printf("14 ");
    outb(0x20, 0x20);
}

void irq15_handler()
{
    printf("15 ");
    outb(0x20, 0x20);
}

#endif