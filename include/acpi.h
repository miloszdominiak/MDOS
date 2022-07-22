#ifndef ACPI_H
#define ACPI_H


bool is_ps2_present();
struct FADT* find_fadt();
struct RSDP* find_rsdp();
struct RSDT* find_rsdt();
struct XSDT* find_xsdt();

#endif