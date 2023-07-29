#include"rvemu.h"

int main(int argc, char *argv[]) {
    machine_t machine;
    machine_load_prog(&machine, argv[1]);

    printf("%#lx\n", machine.st.pc);

    return 0;
}