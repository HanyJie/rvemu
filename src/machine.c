#include"rvemu.h"

void machine_load_prog(machine_t *machine, char *program) {
    mmu_load_elf(&machine->mmu, program);
    machine->st.pc = machine->mmu.entry;  //entry;
}