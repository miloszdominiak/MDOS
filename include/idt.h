#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_TYPE_32_ISR 0xE
#define IDT_TYPE_PRESENT (1 << 7)

#define ISR_TYPE (IDT_TYPE_PRESENT | IDT_TYPE_32_ISR)

struct __attribute__ ((packed)) IDTEntry
{
    uint16_t OffsetLower;
    uint16_t Selector;
    uint8_t Reserved;
    uint8_t Type;
    uint16_t OffsetHigher;
};

struct IDTPointer
{
    uint16_t Size;
    uint32_t Base;
} __attribute__((packed));

typedef void (*ISRPointer)();

void idt_init();
void idt_register_handler(uint8_t number, ISRPointer isr);

#endif