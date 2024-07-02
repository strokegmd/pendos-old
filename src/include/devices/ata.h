#ifndef __ATA_H
#define __ATA_H

#include <stdint.h>
#include "../system.h"

extern void insw(uint16_t port, void *addr, unsigned int count);
extern void outsw(uint16_t port, void *addr, unsigned int count);

#define ATA_PRIMARY_DATA         0x1F0
#define ATA_PRIMARY_ERR          0x1F1
#define ATA_PRIMARY_SECCOUNT     0x1F2
#define ATA_PRIMARY_LBA_LO       0x1F3
#define ATA_PRIMARY_LBA_MID      0x1F4
#define ATA_PRIMARY_LBA_HI       0x1F5
#define ATA_PRIMARY_DRIVE_HEAD   0x1F6
#define ATA_PRIMARY_COMM_REGSTAT 0x1F7
#define ATA_PRIMARY_ALTSTAT_DCR  0x3F6

#define STAT_ERR  (1 << 0)
#define STAT_DRQ  (1 << 3)
#define STAT_SRV  (1 << 4)
#define STAT_DF   (1 << 5)
#define STAT_RDY  (1 << 6)
#define STAT_BSY  (1 << 7)

void ata_pio_read28(uint32_t LBA, uint8_t sectorcount, uint8_t *target) {
    outb(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((LBA >> 24) & 0x0F));
    outb(ATA_PRIMARY_ERR, 0x00);
    outb(ATA_PRIMARY_SECCOUNT, sectorcount);
    outb(ATA_PRIMARY_LBA_LO, LBA & 0xFF);
    outb(ATA_PRIMARY_LBA_MID, (LBA >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HI, (LBA >> 16) & 0xFF);
    outb(ATA_PRIMARY_COMM_REGSTAT, 0x20);

    // ata pivo mode ny ponayli tipo pio pivo ahahaha shutkanul))))

    uint8_t i;
    for(i = 0; i < sectorcount; i++) {
        while(1) {
            uint8_t status = inb(ATA_PRIMARY_COMM_REGSTAT);
            if(status & STAT_DRQ) {
                break;
            }
        }

        insw(ATA_PRIMARY_DATA, (void *)target, 256);
        target += 256;
    }
}

void ata_pio_write48(uint64_t LBA, uint16_t sectorcount, uint8_t *target) {
    outb(ATA_PRIMARY_DRIVE_HEAD, 0x40);
    outb(ATA_PRIMARY_SECCOUNT, (sectorcount >> 8) & 0xFF );
    outb(ATA_PRIMARY_LBA_LO, (LBA >> 24) & 0xFF);
    outb(ATA_PRIMARY_LBA_MID, (LBA >> 32) & 0xFF);
    outb(ATA_PRIMARY_LBA_HI, (LBA >> 40) & 0xFF);
    outb(ATA_PRIMARY_SECCOUNT, sectorcount & 0xFF);
    outb(ATA_PRIMARY_LBA_LO, LBA & 0xFF);
    outb(ATA_PRIMARY_LBA_MID, (LBA >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HI, (LBA >> 16) & 0xFF);
    outb(ATA_PRIMARY_COMM_REGSTAT, 0x34);

    uint8_t i;
    for(i = 0; i < sectorcount; i++) {
        while(1) {
            uint8_t status = inb(ATA_PRIMARY_COMM_REGSTAT);
            if(status & STAT_DRQ) {
                break;
            }
        }
        outsw(ATA_PRIMARY_DATA, (void *)target, 256);
        target += 256;
    }

    outb(ATA_PRIMARY_COMM_REGSTAT, 0xE7);
    while(inb(ATA_PRIMARY_COMM_REGSTAT) & STAT_BSY) {}
}

#endif
