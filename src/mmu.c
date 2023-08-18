#include"rvemu.h"
#include"elfdef.h"

// 返回一个与addr最近的且小于等于addr的地址
u64 rouddown(u64 addr, long pagesz) {
    return addr & ~(pagesz - 1);
}

// 返回一个与addr最近的且大于等于addr的地址
// 为了防止addr正好是对齐的roudup使地址上移一整个页面所以addr + pagesz - 1这里需要减一
u64 roundup(u64 addr, long pagesz) {
    return (addr + pagesz - 1) & ~(pagesz - 1);
}

static int flag_to_prot(elf64_word flag) {
    return ((flag & PL_X) ? PROT_EXEC : 0) |
           ((flag & PL_W) ? PROT_WRITE: 0) |
           ((flag & PL_R) ? PROT_READ : 0);
}

static void mmu_load_phdr(elf64_ehdr_t *ehdr, elf64_phdr_t *phdr, u64 i, FILE *file) {
    // 将指针移动到每一段的开始位置
    if (fseek(file, ehdr->e_phoff + i * ehdr->e_phentsize, SEEK_SET) != 0) {
        Error("seek failed");
    }

    // 读取段信息
    size_t num_read = fread((void*)phdr, 1, sizeof(elf64_phdr_t), file);
    if (num_read < sizeof(elf64_phdr_t)) Error("File too small!");
}

static void mmu_load_seg(mmu_t *mmu, elf64_ehdr_t *ehdr, elf64_phdr_t *phdr, FILE *file, char* program) {
    int fd = open(program, O_RDONLY);

    long pagesz = getpagesize();  // 4096 4KBma
    elf64_off   offset       = phdr->p_offset;
    elf64_addr  vaddr        = phdr->p_vaddr + BASE;  // 将vaddr映射到运行程序的地址
    elf64_addr  align_vaddr  = rouddown(vaddr, pagesz);  // 将地址按页表对齐
    elf64_xword align_filesz = phdr->p_filesz + (offset - rouddown(offset, pagesz));  // filesz不加后面一段就会少读，因为offset必须对齐会多读一部分上一段的内容
    elf64_xword align_memsz  = phdr->p_memsz + (offset - rouddown(offset, pagesz));
    int prot = flag_to_prot(phdr->p_flags);
    elf64_addr addr = (elf64_addr)mmap((void *)align_vaddr, (size_t)align_filesz, prot, MAP_PRIVATE | MAP_FIXED, fd, rouddown(offset, pagesz));

    if (addr != align_vaddr) Error("mmap error!");

    // memsz >= filesz所以比filesz大的部分应该用0填充
    elf64_xword bss_seg = roundup(align_memsz, pagesz) - roundup(align_filesz, pagesz);
    if (bss_seg > 0) {
        addr = (elf64_addr)mmap((void *)(align_vaddr + roundup(align_filesz, pagesz)), bss_seg,
                                prot, 0x20 | MAP_PRIVATE | MAP_FIXED, -1, 0);
        if (addr != align_vaddr + roundup(align_filesz, pagesz)) Error("mmap error!");
    }

    mmu->host_alloc = mmu->host_alloc > (align_vaddr + roundup(align_filesz, pagesz)) ? mmu->host_alloc : (align_vaddr + roundup(align_filesz, pagesz));
    mmu->guest_alloc = mmu->base = mmu->host_alloc - BASE;
}

void mmu_load_elf(mmu_t *mmu, char *program) {
    u8 buf[sizeof(elf64_ehdr_t)];
    FILE *file = fopen(program, "rb");
    if (file == NULL) Error("File open error");
    
    size_t num_read = fread(buf, 1 , sizeof(elf64_ehdr_t), file);
    if (num_read < sizeof(elf64_ehdr_t)) Error("File too small!");

    elf64_ehdr_t *ehdr = (elf64_ehdr_t*) buf;
    if (*(u32*) ehdr != *(u32*) EI_MAG) Error("Not a ELF file");

    if (ehdr->e_machine != ELF_RISCV || ehdr->e_ident[EI_CLASS] != ELF64)
        Error("Only suppord riscv64 ELF file");
    
    mmu->entry = (u64)ehdr->e_entry;

    elf64_phdr_t phdr;

    for (u64 i = 0; i < ehdr->e_phnum; i ++) {
        mmu_load_phdr(ehdr, &phdr, i, file);
        if (phdr.p_type == PT_LOAD)
            mmu_load_seg(mmu, ehdr, &phdr, file, program);
    }

    fclose(file);
}