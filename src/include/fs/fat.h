#ifndef __FAT_H
#define __FAT_H

#include <stdint.h>
#include "../lib/string.h"
#include "../devices/ata.h"

/**
 * Structure representing a FAT boot sector.
 */
typedef struct {
    uint8_t jmp[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    // сам жирный лох чмо
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

/**
 * Structure representing a FAT directory entry.
 */
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

/**
 * Reads a specified number of sectors from the disk into a buffer.
 *
 * @param lba The logical block address to start reading from.
 * @param buffer The buffer to store the read data in.
 * @param sector_count The number of sectors to read.
 */
void read_sectors(uint32_t lba, uint8_t* buffer, uint32_t sector_count) {
    ata_pio_read48(lba, sector_count, buffer);
}

void write_sectors(uint32_t lba, uint8_t* buffer, uint32_t sector_count) {
    ata_pio_write48(lba, sector_count, buffer);
}

/**
 * Reads the boot sector from the disk into a fat_bs_t structure.
 *
 * @param bs The fat_bs_t structure to store the boot sector in.
 */
void read_boot_sector(fat_bs_t* bs) {
    read_sectors(0, (uint8_t*)bs, 1);
}

/**
 * Calculates the first data sector of the FAT file system.
 *
 * @param bpb The fat_bs_t structure containing the file system parameters.
 * @return The first data sector of the file system.
 */
uint32_t get_first_data_sector(fat_bs_t* bpb) {
    return bpb->reserved_sectors + (bpb->num_fats * bpb->fat_size_32);
}

/**
 * Calculates the sector corresponding to a given cluster.
 *
 * @param bpb The fat_bs_t structure containing the file system parameters.
 * @param cluster The cluster number to calculate the sector for.
 * @return The sector corresponding to the given cluster.
 */
uint32_t get_sector_for_cluster(fat_bs_t* bpb, uint32_t cluster) {
    return get_first_data_sector(bpb) + (cluster - 2) * bpb->sectors_per_cluster;
}

/**
 * Reads the next cluster from the FAT table.
 *
 * @param current_cluster The current cluster number.
 * @param bpb The fat_bs_t structure containing the file system parameters.
 * @return The next cluster number.
 */
uint32_t get_next_cluster_from_fat(uint32_t current_cluster, fat_bs_t* bpb) {
    uint8_t sector_buffer[512];

    uint32_t fat_offset = current_cluster * 4;
    uint32_t fat_sector = bpb->reserved_sectors + (fat_offset / bpb->bytes_per_sector);
    uint32_t entry_offset = fat_offset % bpb->bytes_per_sector;

    read_sectors(fat_sector, sector_buffer, 1);

    uint32_t next_cluster = *((uint32_t*)(sector_buffer + entry_offset)) & 0x0FFFFFFF;

    return next_cluster;
}

/**
 * Reads a file from the FAT file system and returns its contents.
 *
 * @param file The fat_entry_t structure representing the file to read.
 * @return The contents of the file.
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
Creates file in FAT file system
*/
fat_entry_t fat_create_file(char* filename) {
    fat_bs_t bs;
    read_boot_sector(&bs);

    uint32_t root_dir_sectors = ((bs.root_entries * 32) + (bs.bytes_per_sector - 1)) / bs.bytes_per_sector;
    uint32_t first_data_sector = bs.reserved_sectors + (bs.num_fats * bs.fat_size_32) + root_dir_sectors;
    uint32_t root_dir_sector = first_data_sector - root_dir_sectors;

    fat_entry_t dir_entries[16];
    read_sectors(root_dir_sector, (uint8_t*)dir_entries, 1);

    fat_entry_t* free_entry = NULL;
    for (uint16_t i = 0; i < 16; i++) {
        if (dir_entries[i].name[0] == 0x00 || dir_entries[i].name[0] == 0xE5) {
            free_entry = &dir_entries[i];
            break;
        }
    }

    if (free_entry == NULL) {
        fat_entry_t empty_entry = {0};
        return empty_entry;
    }

    uint32_t clusters_needed = ((bs.bytes_per_sector * bs.sectors_per_cluster - 1)) / (bs.bytes_per_sector * bs.sectors_per_cluster);
    uint32_t current_cluster = 2;
    uint32_t prev_cluster = 0;
    uint8_t sector_buffer[512];

    for (uint32_t i = 0; i < clusters_needed; i++) {
        while (1) {
            uint32_t fat_offset = current_cluster * 4;
            uint32_t fat_sector = bs.reserved_sectors + (fat_offset / bs.bytes_per_sector);
            uint32_t entry_offset = fat_offset % bs.bytes_per_sector;

            read_sectors(fat_sector, sector_buffer, 1);
            uint32_t next_cluster = *((uint32_t*)(sector_buffer + entry_offset)) & 0x0FFFFFFF;

            if (next_cluster == 0) {
                break;
            }

            current_cluster++;
        }

        if (prev_cluster != 0) {
            uint32_t fat_offset = prev_cluster * 4;
            uint32_t fat_sector = bs.reserved_sectors + (fat_offset / bs.bytes_per_sector);
            uint32_t entry_offset = fat_offset % bs.bytes_per_sector;

            read_sectors(fat_sector, sector_buffer, 1);
            *((uint32_t*)(sector_buffer + entry_offset)) = current_cluster;
        }

        prev_cluster = current_cluster;
        current_cluster++;
    }

    uint32_t fat_offset = prev_cluster * 4;
    uint32_t fat_sector = bs.reserved_sectors + (fat_offset / bs.bytes_per_sector);
    uint32_t entry_offset = fat_offset % bs.bytes_per_sector;

    read_sectors(fat_sector, sector_buffer, 1);
    *((uint32_t*)(sector_buffer + entry_offset)) = 0x0FFFFFFF;

    memcpy(free_entry->name, filename, 11);
    free_entry->attr = 0x20;
    free_entry->cluster_high = (prev_cluster >> 16) & 0xFFFF;
    free_entry->cluster_low = prev_cluster & 0xFFFF;
    free_entry->file_size = 0;

    write_sectors(root_dir_sector, (uint8_t*)dir_entries, 1);

    return *free_entry;
}

uint16_t fat_write_file(fat_entry_t* file, uint8_t* data, uint32_t data_size) {
    fat_bs_t bs;
    read_boot_sector(&bs);

    uint32_t current_cluster = (file->cluster_high << 16) | file->cluster_low;
    uint32_t bytes_written = 0;
    uint8_t sector_buffer[512];

    while (bytes_written < data_size) {
        uint32_t sector = get_sector_for_cluster(&bs, current_cluster);
        read_sectors(sector, sector_buffer, 1);

        uint32_t bytes_to_copy = data_size - bytes_written;
        if (bytes_to_copy > 512) bytes_to_copy = 512;
        memcpy(sector_buffer, data + bytes_written, bytes_to_copy);
        write_sectors(sector, sector_buffer, 1);
        bytes_written += bytes_to_copy;

        if (bytes_written < data_size) {
            uint32_t next_cluster = get_next_cluster_from_fat(current_cluster, &bs);
            if (next_cluster >= 0x0FFFFFF8 || next_cluster == 0) {
                uint32_t new_cluster = 2;
                uint32_t found_cluster = 0;

                while (1) {
                    uint32_t fat_offset = new_cluster * 4;
                    uint32_t fat_sector = bs.reserved_sectors + (fat_offset / bs.bytes_per_sector);
                    uint32_t entry_offset = fat_offset % bs.bytes_per_sector;

                    read_sectors(fat_sector, sector_buffer, 1);
                    uint32_t next_cluster_value = *((uint32_t*)(sector_buffer + entry_offset)) & 0x0FFFFFFF;

                    if (next_cluster_value == 0) {
                        *((uint32_t*)(sector_buffer + entry_offset)) = 0x0FFFFFFF;
                        write_sectors(fat_sector, sector_buffer, 1);
                        found_cluster = new_cluster;
                        break;
                    }

                    new_cluster++;
                }

                if (found_cluster == 0) {
                    // No free cluster found, return an error
                    return 1;
                }

                uint32_t fat_offset = current_cluster * 4;
                uint32_t fat_sector = bs.reserved_sectors + (fat_offset / bs.bytes_per_sector);
                uint32_t entry_offset = fat_offset % bs.bytes_per_sector;

                read_sectors(fat_sector, sector_buffer, 1);
                *((uint32_t*)(sector_buffer + entry_offset)) = found_cluster;
                write_sectors(fat_sector, sector_buffer, 1);

                current_cluster = found_cluster;
            } else {
                current_cluster = next_cluster;
            }
        }
    }

    file->file_size = data_size;

    uint32_t root_dir_sectors = ((bs.root_entries * 32) + (bs.bytes_per_sector - 1)) / bs.bytes_per_sector;
    uint32_t first_data_sector = bs.reserved_sectors + (bs.num_fats * bs.fat_size_32) + root_dir_sectors;
    uint32_t root_dir_sector = first_data_sector - root_dir_sectors;

    fat_entry_t dir_entries[16];
    read_sectors(root_dir_sector, (uint8_t*)dir_entries, 1);

    for (uint16_t i = 0; i < 16; i++) {
        if (memcmp(dir_entries[i].name, file->name, 11) == 0) {
            dir_entries[i] = *file;
            write_sectors(root_dir_sector, (uint8_t*)dir_entries, 1);
            break;
        }
    }

    return 0;
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
    
    fat_entry_t dir_entries[16];
    read_sectors(root_dir_sector, (uint8_t*)dir_entries, 1);

    static fat_entry_t files[16];

    for (uint16_t i = 0; i < 16; i++) {
        if (dir_entries[i].name[0] == 0x00) {
            break;
        }

        if (dir_entries[i].name[0] != 0xE5 && !(dir_entries[i].attr & 0x10) && !(dir_entries[i].attr & 0x08)) {
            files[i] = dir_entries[i];
        }
    }

    return files;
}


/*
Finds file in root directory. Returns pointer to file
*/
fat_entry_t* fat_find_file_ptr(uint8_t* name) {
    fat_entry_t* entries = fat_list_files();
    for (uint16_t i = 0; i < 16; i++) {
        if (strncmp(entries[i].name, name, 11)) {
            return &(entries[i]);
        }
    }
}

/*
Finds file in root directory
*/
fat_entry_t fat_find_file(uint8_t* name) {
    fat_entry_t* entries = fat_list_files();
    for (uint16_t i = 0; i < 16; i++) {
        if (strncmp(entries[i].name, name, 11)) {
            return entries[i];
        }
    }
}

/*
Fixes FAT filename. 
In example, SISKI (5 symbols) -> SISKI (11 symbols)
*/
uint8_t* fat_fix_name(uint8_t* str) {
    static uint8_t new_name[11];
    memset(new_name, 0, 11);

    size_t i = 0;
    for (i; i < strlen(str); i++) {
        new_name[i] = str[i];
    }

    size_t old_i = i;

    for (i; i < old_i + 11 - strlen(str); i++) {
        new_name[i] = ' ';
    }

    return new_name;
}

#endif
