#ifndef __RVEMU_H__
#define __RVEMU_H__

#include"type.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>   // fprintf()
#include<fcntl.h>    // open()
#include<errno.h>    // errno
#include<unistd.h>   // close()
#include<sys/mman.h> // mmap()

#define err(M, ...) (fprintf(stderr, "[ERROR]: %s:%d " M "\n", __FILE__, __LINE__, __VA_ARGS__), exit(1))
#define Error(msg) err("%s", msg)

#define BASE 0x80000000000ULL

// state.c
typedef struct {
    u64 gp_regs[32];
    u64 pc;
} state_t;


// mmu.c
typedef struct {
    u64 entry;
    u64 host_alloc;
    u64 guest_alloc;
    u64 base;
} mmu_t;
void mmu_load_elf(mmu_t *, char *);

// machine.c
typedef struct {
    state_t st;
    mmu_t mmu;
} machine_t;
void machine_load_prog(machine_t *, char *);

#endif