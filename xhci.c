#include <xhci.h>

void xhci_init(uint8_t bus, uint8_t slot, uint8_t function)
{
    uint32_t base = pci_read(bus, slot, function, 0x10, 4) & ~0xF;
    uint32_t offset = *(uint32_t*)(base + 0x10) >> 16 << 2;

    uint32_t xecp = base + offset;

    uint8_t id;
    while(offset > 0)
    {
        uint32_t capability = *(uint32_t*)xecp;

        if(capability & 0xFF == 1)
        {
            printf("xHCI legacy before: %4", capability);
            capability = capability | 1 << 24;
            *(uint32_t*)xecp = capability;

            uint32_t timeout = 100000;
            while(timeout--)
            {
                capability = *(uint32_t*)xecp;
                if((capability >> 24) & 0x1 && !((capability >> 16) & 0x1)) 
                    break;
            }

            *(uint32_t*)(xecp + 4) = 0;

            printf(", after: %4\n", capability);
            break;
        }

        offset = (*(uint32_t*)(xecp) & 0xFFFF) >> 8 << 2;
        xecp += offset;
    }
}