#include <gdt.h>
#include <stdio.h>

struct GDTSelector GDT[5];

void gdt_store(uint8_t selector, uint32_t limit, uint32_t base, uint8_t access, uint8_t flags)
{
    GDT[selector / 8].LimitLower = limit & 0xFFFF;
    GDT[selector / 8].LimitHigher = (limit >> 16) & 0xF;

    GDT[selector / 8].BaseLower = base & 0xFFFF;
    GDT[selector / 8].BaseMedium = (base >> 16) & 0xFF;
    GDT[selector / 8].BaseHigher = (base >> 24) & 0xFF;

    GDT[selector / 8].AccessByte = access;
    GDT[selector / 8].Flags = flags;
}
 
void gdt_init()
{
    gdt_store(0x0, 0x0, 0x0, 0x0, 0x0);
    gdt_store(0x08, 0xFFFFF, 0x0, GDT_KERNEL_ACCESS, GDT_FLAGS_32 | GDT_FLAGS_4K_BLOCK);
    gdt_store(0x10, 0xFFFFF, 0x0, GDT_DATA_ACCESS, GDT_FLAGS_32 | GDT_FLAGS_4K_BLOCK);
    gdt_store(0x18, 0xFFFFF, 0x0, GDT_KERNEL_ACCESS, 0);
    gdt_store(0x20, 0xFFFFF, 0x0, GDT_DATA_ACCESS, 0);

    struct GDTPointer gdtp;
    gdtp.Size = sizeof(struct GDTSelector) * 5 - 1;
    gdtp.Offset = (uint32_t)GDT;

    load_gdt(gdtp);
}