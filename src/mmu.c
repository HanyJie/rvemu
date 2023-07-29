#include"rvemu.h"
#include"elfdef.h"

void mmu_load_elf(mmu_t *mmu, char *program) {
    u8 buf[sizeof(elf64_ehdr_t)];
    FILE *file = fopen(program, "rb");
    if (file == NULL) Error("File open error");
    
    size_t num_read = fread(buf, 1 , sizeof(elf64_ehdr_t), file);
    if (num_read < sizeof(elf64_ehdr_t)) Error("File too small!");

    elf64_ehdr_t *ehdr = (elf64_ehdr_t*) buf;
    if (*(u32*) ehdr != *(u32*) buf) Error("Not a ELF file");

    if (ehdr->e_machine != ELF_RISCV || ehdr->e_ident[EI_CLASS] != ELF64)
        Error("Only suppord riscv64 ELF file");
    
    mmu->entry = (u64)ehdr->e_entry;
    fclose(file);
}