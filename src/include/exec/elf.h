#ifndef __ELF_H
#define __ELF_H

#include <stdint.h>

#define ELF_NIDENT 16

#define ELFMAG0	0x7F
#define ELFMAG1	'E'
#define ELFMAG2	'L'
#define ELFMAG3	'F'

#define ELFDATA2LSB	(1)
#define ELFCLASS32	(1)

#define EM_386		(3)
#define EV_CURRENT	(1)

typedef struct {
	uint8_t	 e_ident[ELF_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} elf_header_t;

typedef struct {
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} elf_prog_header_t;

typedef enum {
	EI_MAG0		  = 0,
	EI_MAG1		  = 1,
	EI_MAG2		  = 2,
	EI_MAG3		  = 3,
	EI_CLASS	  = 4,
	EI_DATA		  = 5,
	EI_VERSION	  = 6,
	EI_OSABI	  = 7,
	EI_ABIVERSION = 8,
	EI_PAD		  = 9,
} elf_ident_t;

typedef enum {
	ET_NONE		= 0,
	ET_REL		= 1,
	ET_EXEC		= 2,
} elf_type_t;

typedef void (*entry_point_t)(void);

/**
 * Checks if data is ELF file using magic and supported.
 */
bool elf_check_file(uint8_t* data) {
    elf_header_t* header = (elf_header_t*) data;
    if (!data || !header) return false;

    bool magic = header->e_ident[EI_MAG0] == ELFMAG0 && header->e_ident[EI_MAG1] == ELFMAG1 &&
        header->e_ident[EI_MAG2] == ELFMAG2 && header->e_ident[EI_MAG3] == ELFMAG3;

    bool supported = header->e_ident[EI_CLASS] == ELFCLASS32 && header->e_ident[EI_DATA] == ELFDATA2LSB &&
        header->e_machine == EM_386 && header->e_ident[EI_VERSION] == EV_CURRENT &&
        header->e_type == ET_EXEC;

    return magic && supported;
}

/**
 * Starts ELF executable file.
 */
void elf_start(uint8_t* data) {
	elf_header_t* header = (elf_header_t*) data;

	elf_prog_header_t *ph = (elf_prog_header_t *) (data + header->e_phoff);
	for (uint32_t i = 0; i < header->e_phnum; i++, ph++) {
		if (ph->p_type != 1) continue;
		memcpy((void*) ph->p_vaddr, data + ph->p_offset, ph->p_filesz);
	}

	entry_point_t entry_point = (entry_point_t) header->e_entry;
	entry_point();
}

#endif