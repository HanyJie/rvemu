#ifndef __ELFDEF_H__
#define __ELFDEF_H__

#include"type.h"
#define EI_NIDENT 16
#define EI_MAG "\x7f\x45\x4c\x46"  // magic(hex): 7f 45 4c 46(0x7fELF)
#define EI_CLASS 4
#define ELF32    1
#define ELF64    2
#define ELF_RISCV 243


typedef struct {
    unsigned char e_ident[EI_NIDENT]; // Magic number and other info.
    elf64_half    e_type;             // Identifies object file type.
    elf64_half    e_machine;          // Specifies target ISA.
    elf64_word    e_version;          // File version.
    elf64_addr    e_entry;            // The memory address of the program entry.
    elf64_off     e_phoff;            // Program Header Table offset.
    elf64_off     e_shoff;            // of Section Header Table offset.
    elf64_word    e_flags;            // Processor specific flags.
    elf64_half    e_ehsize;           // ELF header size.(byte)
    elf64_half    e_phentsize;        // Contains the size of a program header table entry.(byte)
    elf64_half    e_phnum;            // Contains the number of entries in the program header table.
    elf64_half    e_shentsize;        // Contains the size of a section header table entry.(byte)
    elf64_half    e_shnum;            // Contains the number of entries in the section header table.
    elf64_half    e_shstrndx;         // Section header string table index
} elf64_ehdr_t;



#endif