#ifndef IRQ_H
#define IRQ_H

#include <idt.h>

void irq_install_handler(uint8_t irq, ISRPointer isr);

#endif