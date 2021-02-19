#ifndef ACPITABLES_H
#define ACPITABLES_H

#include <stdint.h>

struct RSDP
{
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];
};

struct __attribute__ ((packed)) SDTHeader {
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
};

struct RSDT {
    struct SDTHeader Header;
    uint32_t Entry[];
};

struct XSDT {
    struct SDTHeader Header;
    uint64_t Entry[];
};

struct __attribute__ ((packed)) FADT {
    struct SDTHeader Header;
    uint32_t FirmwareCtrl;
    uint32_t DsdtAddress;
    uint8_t Reserved;
    uint8_t PreferredPMProfile;
    uint16_t SCIInt;
    uint32_t SMICmd;
    uint8_t ACPIEnable;
    uint8_t ACPIDisable;
    uint8_t S4BIOSCnt;
    uint8_t PStateCnt;
    uint32_t PM1aEvtBlk;
    uint32_t PM1bEvtBlk;
    uint32_t PM1aCntBlk;
    uint32_t PM1bCntBlk;
    uint32_t PM2CntBlk;
    uint32_t PMTmrBlk;
    uint32_t GPE0Blk;
    uint32_t GPE1Blk;
    uint8_t PM1EvtLen;
    uint8_t PM1CntLen;
    uint8_t PM2CntLen;
    uint8_t PMTmrLen;
    uint8_t GPE0BlkLen;
    uint8_t GPE1BlkLen;
    uint8_t GPE1Base;
    uint8_t CSTCnt;
    uint16_t PLvl2Lat;
    uint16_t PLvl3Lat;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t DutyOffset;
    uint8_t DutyWidth;
    uint8_t DayAlrm;
    uint8_t MonAlrm;
    uint8_t Century;
    uint16_t IAPCBootArch;
    uint8_t Reserved2;
    uint32_t Flags;
    uint8_t ResetReg[12];
    uint8_t ResetValue;
    uint8_t Reserved3[3];
    uint64_t XFirmwareCtrl;
    uint64_t XDsdtAddress;
    uint8_t XPM1aEvtBlk[12];
    uint8_t XPM1bEvtBlk[12];
    uint8_t XPM1aCntBlk[12];
    uint8_t XPM1bCntBlk[12];
    uint8_t XPM2CntBlk[12];
    uint8_t XPMTmrBlk[12];
    uint8_t XGPE0Blk[12];
    uint8_t XGPE1Blk[12];
};

struct DSDT
{
    struct SDTHeader Header;
    uint8_t DefinitionBlock[];
};

#endif