#include <acpi.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static uint32_t find_rsdp()
{
    uint32_t ebda = *(uint16_t*)0x040E << 4;

    for(const char* ptr = ebda; ptr <= ebda + 1024; ptr += 16)
        if(!strncmp(ptr, "RSD PTR ", 8))
            return ptr;

    for(const char* ptr = 0x000E0000; ptr <= 0x000FFFFF; ptr += 16)
        if(!strncmp(ptr, "RSD PTR ", 8))
            return ptr;

    return 0;
}

uint8_t is_ps2()
{
    int8_t* rsdp = find_rsdp();

    uint8_t acpi_v = rsdp[15];
    
    // if(acpi_v == 0)
    //     printf("ACPI 1.0\n");
    // else
    //     printf("ACPI 2.0\n");

    int32_t checksum = 0;
    for(int i = 0; i < 20; i++)
        checksum += rsdp[i];

    checksum &= 0xFF;

    // if(!checksum)
    //     printf("RSDP checksum OK\n");

    int8_t* rsdt = *(uint32_t*)(rsdp + 16);

    checksum = 0;
    for(int i = 0; i < *(uint32_t*)(rsdt + 4); i++)
        checksum += rsdt[i];

    checksum &= 0xFF;

    // if(!checksum)
    //     printf("RSDT checksum OK\n");
    
    uint8_t* fadt;
    for(int i = 0; ; i++)
    {
        fadt = *(uint32_t*)(rsdt + 36 + i);
        if(!strncmp(fadt, "FACP", 4))
        {
            // printf("Znalazlem FACP\n");
            break;
        }
    }

    // printf("IAPC: %2\n", *(uint16_t*)(fadt + 109));
    if(fadt[109] & 0x2 || fadt[8] < 3)
        return 1;
    else
        return 0;
    
}