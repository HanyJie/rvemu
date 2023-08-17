#include"rvemu.h"

int main(int argc, char *argv[]) {
    machine_t machine;
    machine_load_prog(&machine, argv[1]);

    printf("%#lx\n", machine.st.pc);
    printf("machine address: %#lx\n", (u64)&machine);
    printf("%#lx\n", machine.mmu.host_alloc);

    return 0;
}