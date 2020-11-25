#ifndef UHCI_H
#define UHCI_H

#include <stdint.h>

void uhci_init(uint8_t bus, uint8_t slot, uint8_t function);

#endif