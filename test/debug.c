#include<stdio.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/mman.h>
#include<fcntl.h>

#define EI_NIDENT 16
#define BASE 0x80000000000ULL

#define PL_X    0x1  // 可执行段
#define PL_W    0x2  // 可写段
#define PL_R    0x4  // 可读段

typedef uint16_t elf64_half;   // u16
typedef uint32_t elf64_word;   // u32
typedef uint64_t elf64_xword;  // u64
typedef uint64_t elf64_addr;   // u64
typedef uint64_t elf64_off;    // u64

typedef int8_t   i8;
typedef uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;

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

u64 rounddown(u64 addr, long pagesz) {
    return addr & ~(pagesz - 1);
}

// 返回一个与addr最近的且大于等于addr的地址
// 为了防止addr正好是对齐的roudup使地址上移一整个页面所以addr + pagesz - 1这里需要减一
u64 roundup(u64 addr, long pagesz) {
    return (addr + pagesz - 1) & ~(pagesz - 1);
}

int flag_to_prot(elf64_word flag) {
    return (flag & PL_X) ? PROT_EXEC :
           (flag & PL_W) ? PROT_WRITE:
           (flag & PL_R) ? PROT_READ : PROT_NONE;
}

int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "rb");
    int fd = open(argv[1], O_RDONLY);
    elf64_ehdr_t ehdr;
    elf64_phdr_t phdr;
    fread(&ehdr, 1, sizeof(elf64_ehdr_t), file);
    fseek(file, ehdr.e_phoff + ehdr.e_phentsize, SEEK_SET);
    fread(&phdr, 1, sizeof(elf64_ehdr_t), file);
    elf64_addr vaddr = phdr.p_vaddr + BASE;
    elf64_addr align_vaddr = rounddown(phdr.p_vaddr + BASE, 4096);
    int prot = flag_to_prot(phdr.p_flags);
    elf64_xword filesz = phdr.p_filesz + (vaddr-align_vaddr);
    elf64_off offset = rounddown(phdr.p_offset, 4096);
    elf64_addr addr = (elf64_addr)mmap((void *)align_vaddr, (size_t)filesz, prot, MAP_PRIVATE | MAP_FIXED, fd, offset);
    u64 *point = addr;
    printf("vaddr: %#lx\n", vaddr);
    printf("aling_vaddr: %#lx\n", align_vaddr);
    printf("filesz: %#lx\n", phdr.p_filesz);
    printf("filesz_algin: %#lx\n\n", filesz);
    printf("align_off: %#lx\n", rounddown(offset, 4096));
    printf("addr: %#lx\n", addr);
    printf("%#lx\n", *point);


    return 0;
}
