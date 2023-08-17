#ifndef __ELFDEF_H__
#define __ELFDEF_H__

#include"type.h"
#define EI_NIDENT 16
#define EI_MAG "\x7f\x45\x4c\x46"  // magic(hex): 7f 45 4c 46(0x7fELF)
#define EI_CLASS 4
#define ELF32    1
#define ELF64    2
#define ELF_RISCV 243

#define PT_LOAD 1
#define PL_X    0x1  // 可执行段
#define PL_W    0x2  // 可写段
#define PL_R    0x4  // 可读段

typedef struct {
    unsigned char e_ident[EI_NIDENT]; // ELF魔数和其他标识信息
    elf64_half    e_type;             // 文件类型，如可执行文件、共享库等
    elf64_half    e_machine;          // 目标体系结构，如x86、ARM等
    elf64_word    e_version;          // ELF文件版本号
    elf64_addr    e_entry;            // 程序执行的起始地址
    elf64_off     e_phoff;            // 程序头表在文件中的偏移量
    elf64_off     e_shoff;            // 节头表在文件中的偏移量
    elf64_word    e_flags;            // 处理器特定标志
    elf64_half    e_ehsize;           // ELF文件头的大小
    elf64_half    e_phentsize;        // 程序头表项的大小
    elf64_half    e_phnum;            // 程序头表中的条目数
    elf64_half    e_shentsize;        // 节头表项的大小
    elf64_half    e_shnum;            // 节头表中的条目数
    elf64_half    e_shstrndx;         // 节头表字符串表的索引
} elf64_ehdr_t;

typedef struct {
    elf64_word    p_type;             // 段类型，如代码段、数据段等
    elf64_word    p_flags;            // 段的属性，如可读、可写、可执行等
    elf64_off     p_offset;           // 段在文件中的偏移量
    elf64_addr    p_vaddr;            // 段在虚拟内存中的起始地址
    elf64_addr    p_paddr;            // 段在物理内存中的起始地址（在大多数情况下与p_vaddr相同）
    elf64_xword   p_filesz;           // 段在文件中的大小
    elf64_xword   p_memsz;            // 段在内存中的大小
    elf64_xword   p_align;            // 段在内存中的对齐方式
} elf64_phdr_t;


#endif