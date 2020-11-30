# MDOS
Simple x86 OS for educational purposes

### What's done:
+ GRUB booting
+ Printing to screen
+ PCI enumeration
+ Disabling USB Legacy Support (UHCI, EHCI)
+ ACPI PS/2 detection
+ PS/2 controller initialisation
+ PS/2 mouse and keyboard scancodes

### Todo: PS/2
+ PS/2 keyboard driver
+ Detect MMIO or Port IO
+ Add pci_write_byte...
+ EHCI check of each register like in XHCI
+ EHCI, XHCI check if 64 bit
+ Defines instead of numbers
+ EHCI, XHCI proper timeout

![Screenshot](http://parafiazelechlinek.pl/milosz/mdos.png#2 "MDOS")
