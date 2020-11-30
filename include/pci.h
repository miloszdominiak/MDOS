#ifndef PCI_H
#define PCI_H

#include <stdint.h>

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint8_t bytes);
void pci_write(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t data);
void pci_enum();

#endif