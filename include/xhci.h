#ifndef XHCI_H
#define XHCI_H

#include <stdint.h>

void xhci_init(uint8_t bus, uint8_t slot, uint8_t function);

#endif