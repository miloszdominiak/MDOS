#include <acpi.h>
#include <stddef.h>
#include <string.h>

#include <stdio.h>

static uint8_t checksum(uint8_t* start, size_t length)
{
    uint8_t sum = 0;
    for(size_t i = 0; i < length; i++)
        sum += start[i];
    
    return sum;
}

static bool validate_rsdp(struct RSDP* rsdp)
{
    if(strncmp(rsdp->Signature, "RSD PTR ", 8))
        return false;

    if(rsdp->Revision == 0)
        return !checksum((uint8_t*)rsdp, 20);
    else
        return !checksum((uint8_t*)rsdp, rsdp->Length);
}

static bool validate_sdt(void* table, const char* signature)
{
    struct SDTHeader* sdt = table;
    return !strncmp(sdt->Signature, signature, 4) && !checksum((uint8_t*)sdt, sdt->Length);
}

struct RSDP* find_rsdp()
{
    uint32_t ebda_pointer = *(uint16_t*)0x40E * 16;

    struct RSDP* rsdp;
    for(size_t i = 0; i < 1024; i++)
    {
        rsdp = (struct RSDP*)ebda_pointer + i;
        if(validate_rsdp(rsdp))
            return rsdp;
    }

    for(uint32_t i = 0x000E0000; i < 0x000FFFFF; i++)
    {
        rsdp = (struct RSDP*)i;
        if(validate_rsdp(rsdp))
            return rsdp;
    }

    return NULL;
}

struct RSDT* find_rsdt()
{
    struct RSDP* rsdp = find_rsdp();
    if(!rsdp)
        return NULL;
    
    if(validate_sdt((void*)rsdp->RsdtAddress, "RSDT"))
        return (struct RSDT*)rsdp->RsdtAddress;

    return NULL;
}

struct XSDT* find_xsdt()
{
    struct RSDP* rsdp = find_rsdp();
    if(!rsdp)
        return NULL;
    
    if(validate_sdt((void*)(uint32_t)rsdp->XsdtAddress, "XSDT"))
        return (struct XSDT*)(uint32_t)rsdp->XsdtAddress;
        
    return NULL;
}

struct FADT* find_fadt()
{
    struct RSDP* rsdp = find_rsdp();
    if(!rsdp)
        return false;

    if(rsdp->Revision == 0)
    {
        struct RSDT* rsdt = find_rsdt();
        if(!rsdt)
            return NULL;

        struct FADT* fadt;
        for(size_t i = 0; i < (rsdt->Header.Length - sizeof(rsdt->Header)) / 4; i++)
        {
            fadt = (struct FADT*)rsdt->Entry[i];
            if(validate_sdt(fadt, "FACP"))
                return fadt;
        }
    }

    if(rsdp->Revision == 2)
    {
        struct XSDT* xsdt = find_xsdt();
        if(!xsdt)
            return NULL;

        struct FADT* fadt;
        for(size_t i = 0; i < (xsdt->Header.Length - sizeof(xsdt->Header)) / 8; i++)
        {
            fadt = (struct FADT*)(uint32_t)xsdt->Entry[i];
            if(validate_sdt(fadt, "FACP"))
                return fadt;
        }
    }

    return NULL;
}

struct DSDT* find_dsdt()
{
    struct FADT* fadt = find_fadt();
    if(!fadt)
        return false;

    struct DSDT* dsdt = (struct DSDT*)(uint32_t)fadt->XDsdtAddress;
    if(fadt->Header.Revision > 3 && validate_sdt(dsdt, "DSDT"))
        return dsdt;

    dsdt = (struct DSDT*)fadt->DsdtAddress;
    if(validate_sdt(dsdt, "DSDT"))
        return dsdt;

    return NULL;
}

bool is_ps2_present()
{
    struct FADT* fadt = find_fadt();
    if(!fadt)
        return false;
        
    if(fadt->Header.Revision < 3 || fadt->IAPCBootArch & 0x2)
        return true;

    printf("DELL\n");
    
    struct DSDT* dsdt = find_dsdt();
    if(!dsdt)
        return false;

    uint8_t* tablica = (uint8_t*)dsdt;
    for(size_t i = 0; i < dsdt->Header.Length; i++)
        if(!memcmp(tablica + i, "\x41\xD0\x03\x03", 4))
            return true;

    return false;
}