#ifndef __FAT_H
#define __FAT_H

#include <stdint.h>
#include "../devices/ata.h"

typedef struct {
    uint8_t jmp[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t num_fats;
    uint16_t root_entries;
    uint16_t total_sectors_short;
    uint8_t media_descriptor;
    uint16_t fat_size_sectors;
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_long;
    uint32_t fat_size_32;
    uint16_t ext_flags;
    uint16_t fs_version;
    uint32_t root_cluster;
    uint16_t fs_info;
    uint16_t backup_boot_sector;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved1;
    uint8_t boot_signature;
    uint32_t volume_id;
    char volume_label[11];
    char fs_type[8];
} __attribute__((packed)) fat_bs_t;

typedef struct {
    uint8_t name[11];
    uint8_t attr;
    uint8_t reserved;
    uint8_t create_time_tenth;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access_date;
    uint16_t cluster_high;
    uint16_t write_time;
    uint16_t write_date;
    uint16_t cluster_low;
    uint32_t file_size;
} __attribute__((packed)) fat_entry_t;

void read_sectors(uint32_t lba, uint8_t* buffer, uint32_t sector_count) {
    ata_pio_read28(lba, sector_count, buffer);
}

void read_boot_sector(fat_bs_t* bs) {
    read_sectors(0, (uint8_t*)bs, 1);
}

uint32_t get_first_data_sector(fat_bs_t* bpb) {
    return bpb->reserved_sectors + (bpb->num_fats * bpb->fat_size_32);
}

uint32_t get_sector_for_cluster(fat_bs_t* bpb, uint32_t cluster) {
    return get_first_data_sector(bpb) + (cluster - 2) * bpb->sectors_per_cluster;
}

uint32_t get_next_cluster_from_fat(uint32_t current_cluster, fat_bs_t* bpb) {
    uint8_t sector_buffer[512];

    uint32_t fat_offset = current_cluster * 4;
    uint32_t fat_sector = bpb->reserved_sectors + (fat_offset / bpb->bytes_per_sector);
    uint32_t entry_offset = fat_offset % bpb->bytes_per_sector;

    read_sectors(fat_sector, sector_buffer, 1);

    uint32_t next_cluster = *((uint32_t*)(sector_buffer + entry_offset)) & 0x0FFFFFFF;

    return next_cluster;
}

/*
Reads file and returns it content
*/
uint8_t* fat_read_file(fat_entry_t file) {
    fat_bs_t bs;
    read_boot_sector(&bs);

    uint32_t current_cluster = (file.cluster_high << 16) | file.cluster_low;
    uint32_t file_size = file.file_size;
    uint8_t* file_buffer = (uint8_t*) malloc(file_size + 1);
    uint8_t sector_buffer[512];

    uint32_t bytes_read = 0;
    while (bytes_read < file_size) {
        uint32_t sector = get_sector_for_cluster(&bs, current_cluster);
        read_sectors(sector, sector_buffer, 1);

        uint32_t bytes_to_copy = file_size - bytes_read;
        if (bytes_to_copy > 512) bytes_to_copy = 512;
        memcpy(file_buffer + bytes_read, sector_buffer, bytes_to_copy);
        bytes_read += bytes_to_copy;

        current_cluster = get_next_cluster_from_fat(current_cluster, &bs);
        if (current_cluster >= 0x0FFFFFF8) break;
    }

    file_buffer[file_size] = '\0';
    free(file_buffer);
    return file_buffer;
}

/*
Gets all files in root directory
*/
fat_entry_t* fat_list_files() {
    fat_bs_t bs;
    read_boot_sector(&bs);

    uint32_t root_dir_sectors = ((bs.root_entries * 32) + (bs.bytes_per_sector - 1)) / bs.bytes_per_sector;
    uint32_t first_data_sector = bs.reserved_sectors + (bs.num_fats * bs.fat_size_32) + root_dir_sectors;
    uint32_t root_dir_sector = first_data_sector - root_dir_sectors;
    uint32_t current_cluster = bs.root_cluster;
    
    fat_entry_t dir_entries[16];
    read_sectors(root_dir_sector, (uint8_t*)dir_entries, 1);

    static fat_entry_t files[16];

    for (int i = 0; i < 16; i++) {
        if (dir_entries[i].name[0] == 0x00) {
            break;
        }

        if (dir_entries[i].name[0] != 0xE5 && !(dir_entries[i].attr & 0x10) && !(dir_entries[i].attr & 0x08)) {
            files[i] = dir_entries[i];
        }
    }

    return files;
}

#endif
