#ifndef __TYPT_H__
#define __TYPT_H__

#include<stdint.h>

typedef int8_t   i8;
typedef uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;

// elf
typedef uint16_t elf64_half;   // u16
typedef uint32_t elf64_word;   // u32
typedef uint64_t elf64_xword;  // u64
typedef uint64_t elf64_addr;   // u64
typedef uint64_t elf64_off;    // u64
#endif