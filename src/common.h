/* SPDX-License-Identifier: GPL-2.0 */
// https://github.com/NVSL/OptaneStudy/blob/master/src/kernel/common.h
// This file is derived from the above source code.
#ifndef LATTESTER_COMMON_H
#define LATTESTER_COMMON_H
/*
 * BRIEF DESCRIPTION
 *
 * Header for commonly used defines
 *
 * Copyright 2019 Regents of the University of California,
 * UCSD Non-Volatile Systems Lab
 */
#include <immintrin.h>
#include <x86intrin.h>
__m128 xmm0;
__m256 ymm0;

// #ifdef __KERNEL__
#if 0
  #include <asm/fpu/api.h>
  #define KERNEL_BEGIN \
	 kernel_fpu_begin();
  #define KERNEL_END \
	 kernel_fpu_end();
#else
  #define KERNEL_BEGIN do { } while (0);
  #define KERNEL_END do { } while (0);
#endif

/* Assembly instructions utilize the following registers:
 * rsi: memory address
 * rax, rdx, rcx, r8d and r9d: timing
 * rdx: populating cache-lines
 * ymm0: streaming instructions
 */
#define REGISTERS "rsi", "rax", "rdx", "rcx", "r8", "r9", "ymm0"
#define REGISTERS_NONSSE "rsi", "rax", "rdx", "rcx", "r8", "r9"

/* ymm0: 256-bit register (requires AVX support)
 * vbroadcastsd: VEX.256-bit version (r[0] = r[1] = r[2] = r[3] = v)
 */
#define LOAD_VALUE              "vbroadcastsd %[value], %%ymm0 \n"
#define LOAD_ADDR               "mov %[memarea], %%rsi \n" \
                                "mfence \n"

#define FLUSH_CACHE_LINE        "clflush 0*32(%%rsi) \n" \
                                "clflush 2*32(%%rsi) \n" \
                                "clflush 4*32(%%rsi) \n" \
                                "clflush 6*32(%%rsi) \n" \
                                "mfence \n"

#define LOAD_CACHE_LINE         "movq 0*8(%%rsi), %%rdx \n" \
                                "movq 1*8(%%rsi), %%rdx \n" \
                                "movq 2*8(%%rsi), %%rdx \n" \
                                "movq 3*8(%%rsi), %%rdx \n" \
                                "movq 4*8(%%rsi), %%rdx \n" \
                                "movq 5*8(%%rsi), %%rdx \n" \
                                "movq 6*8(%%rsi), %%rdx \n" \
                                "movq 7*8(%%rsi), %%rdx \n" \
                                "movq 8*8(%%rsi), %%rdx \n" \
                                "movq 9*8(%%rsi), %%rdx \n" \
                                "movq 10*8(%%rsi), %%rdx \n" \
                                "movq 11*8(%%rsi), %%rdx \n" \
                                "movq 12*8(%%rsi), %%rdx \n" \
                                "movq 13*8(%%rsi), %%rdx \n" \
                                "movq 14*8(%%rsi), %%rdx \n" \
                                "movq 15*8(%%rsi), %%rdx \n" \
                                "movq 16*8(%%rsi), %%rdx \n" \
                                "movq 17*8(%%rsi), %%rdx \n" \
                                "movq 18*8(%%rsi), %%rdx \n" \
                                "movq 19*8(%%rsi), %%rdx \n" \
                                "movq 20*8(%%rsi), %%rdx \n" \
                                "movq 21*8(%%rsi), %%rdx \n" \
                                "movq 22*8(%%rsi), %%rdx \n" \
                                "movq 23*8(%%rsi), %%rdx \n" \
                                "movq 24*8(%%rsi), %%rdx \n" \
                                "movq 25*8(%%rsi), %%rdx \n" \
                                "movq 26*8(%%rsi), %%rdx \n" \
                                "movq 27*8(%%rsi), %%rdx \n" \
                                "movq 28*8(%%rsi), %%rdx \n" \
                                "movq 29*8(%%rsi), %%rdx \n" \
                                "movq 30*8(%%rsi), %%rdx \n" \
                                "movq 31*8(%%rsi), %%rdx \n" \
                                "mfence \n"

#define CLEAR_PIPELINE          "nop \nnop \nnop \nnop \nnop \nnop \n" \
                                "nop \nnop \nnop \nnop \nnop \nnop \n" \
                                "nop \nnop \nnop \nnop \nnop \nnop \n" \
                                "nop \nnop \nnop \nnop \nnop \nnop \n" \
                                "nop \nnop \nnop \nnop \nnop \nnop \n" \
                                "nop \nnop \nnop \nnop \nnop \nnop \n" \
                                "nop \nnop \nnop \nnop \nnop \nnop \n"

#define STORE_MOVB_64           "movb %%dl, 0+64(%%rsi) \n" \
        "movb %%dl, 1+64(%%rsi) \n" \
        "movb %%dl, 2+64(%%rsi) \n" \
        "movb %%dl, 3+64(%%rsi) \n" \
        "movb %%dl, 4+64(%%rsi) \n" \
        "movb %%dl, 5+64(%%rsi) \n" \
        "movb %%dl, 6+64(%%rsi) \n" \
        "movb %%dl, 7+64(%%rsi) \n" \
        "movb %%dl, 8+64(%%rsi) \n" \
        "movb %%dl, 9+64(%%rsi) \n" \
        "movb %%dl, 10+64(%%rsi) \n" \
        "movb %%dl, 11+64(%%rsi) \n" \
        "movb %%dl, 12+64(%%rsi) \n" \
        "movb %%dl, 13+64(%%rsi) \n" \
        "movb %%dl, 14+64(%%rsi) \n" \
        "movb %%dl, 15+64(%%rsi) \n" \
        "movb %%dl, 16+64(%%rsi) \n" \
        "movb %%dl, 17+64(%%rsi) \n" \
        "movb %%dl, 18+64(%%rsi) \n" \
        "movb %%dl, 19+64(%%rsi) \n" \
        "movb %%dl, 20+64(%%rsi) \n" \
        "movb %%dl, 21+64(%%rsi) \n" \
        "movb %%dl, 22+64(%%rsi) \n" \
        "movb %%dl, 23+64(%%rsi) \n" \
        "movb %%dl, 24+64(%%rsi) \n" \
        "movb %%dl, 25+64(%%rsi) \n" \
        "movb %%dl, 26+64(%%rsi) \n" \
        "movb %%dl, 27+64(%%rsi) \n" \
        "movb %%dl, 28+64(%%rsi) \n" \
        "movb %%dl, 29+64(%%rsi) \n" \
        "movb %%dl, 30+64(%%rsi) \n" \
        "movb %%dl, 31+64(%%rsi) \n" \
        "movb %%dl, 32+64(%%rsi) \n" \
        "movb %%dl, 33+64(%%rsi) \n" \
        "movb %%dl, 34+64(%%rsi) \n" \
        "movb %%dl, 35+64(%%rsi) \n" \
        "movb %%dl, 36+64(%%rsi) \n" \
        "movb %%dl, 37+64(%%rsi) \n" \
        "movb %%dl, 38+64(%%rsi) \n" \
        "movb %%dl, 39+64(%%rsi) \n" \
        "movb %%dl, 40+64(%%rsi) \n" \
        "movb %%dl, 41+64(%%rsi) \n" \
        "movb %%dl, 42+64(%%rsi) \n" \
        "movb %%dl, 43+64(%%rsi) \n" \
        "movb %%dl, 44+64(%%rsi) \n" \
        "movb %%dl, 45+64(%%rsi) \n" \
        "movb %%dl, 46+64(%%rsi) \n" \
        "movb %%dl, 47+64(%%rsi) \n" \
        "movb %%dl, 48+64(%%rsi) \n" \
        "movb %%dl, 49+64(%%rsi) \n" \
        "movb %%dl, 50+64(%%rsi) \n" \
        "movb %%dl, 51+64(%%rsi) \n" \
        "movb %%dl, 52+64(%%rsi) \n" \
        "movb %%dl, 53+64(%%rsi) \n" \
        "movb %%dl, 54+64(%%rsi) \n" \
        "movb %%dl, 55+64(%%rsi) \n" \
        "movb %%dl, 56+64(%%rsi) \n" \
        "movb %%dl, 57+64(%%rsi) \n" \
        "movb %%dl, 58+64(%%rsi) \n" \
        "movb %%dl, 59+64(%%rsi) \n" \
        "movb %%dl, 60+64(%%rsi) \n" \
        "movb %%dl, 61+64(%%rsi) \n" \
        "movb %%dl, 62+64(%%rsi) \n" \
        "movb %%dl, 63+64(%%rsi) \n" 

#define LOAD_MOVB_64 \
        "movb 0+64(%%rsi), %%dl \n" \
        "movb 1+64(%%rsi), %%dl \n" \
        "movb 2+64(%%rsi), %%dl \n" \
        "movb 3+64(%%rsi), %%dl \n" \
        "movb 4+64(%%rsi), %%dl \n" \
        "movb 5+64(%%rsi), %%dl \n" \
        "movb 6+64(%%rsi), %%dl \n" \
        "movb 7+64(%%rsi), %%dl \n" \
        "movb 8+64(%%rsi), %%dl \n" \
        "movb 9+64(%%rsi), %%dl \n" \
        "movb 10+64(%%rsi), %%dl \n" \
        "movb 11+64(%%rsi), %%dl \n" \
        "movb 12+64(%%rsi), %%dl \n" \
        "movb 13+64(%%rsi), %%dl \n" \
        "movb 14+64(%%rsi), %%dl \n" \
        "movb 15+64(%%rsi), %%dl \n" \
        "movb 16+64(%%rsi), %%dl \n" \
        "movb 17+64(%%rsi), %%dl \n" \
        "movb 18+64(%%rsi), %%dl \n" \
        "movb 19+64(%%rsi), %%dl \n" \
        "movb 20+64(%%rsi), %%dl \n" \
        "movb 21+64(%%rsi), %%dl \n" \
        "movb 22+64(%%rsi), %%dl \n" \
        "movb 23+64(%%rsi), %%dl \n" \
        "movb 24+64(%%rsi), %%dl \n" \
        "movb 25+64(%%rsi), %%dl \n" \
        "movb 26+64(%%rsi), %%dl \n" \
        "movb 27+64(%%rsi), %%dl \n" \
        "movb 28+64(%%rsi), %%dl \n" \
        "movb 29+64(%%rsi), %%dl \n" \
        "movb 30+64(%%rsi), %%dl \n" \
        "movb 31+64(%%rsi), %%dl \n" \
        "movb 32+64(%%rsi), %%dl \n" \
        "movb 33+64(%%rsi), %%dl \n" \
        "movb 34+64(%%rsi), %%dl \n" \
        "movb 35+64(%%rsi), %%dl \n" \
        "movb 36+64(%%rsi), %%dl \n" \
        "movb 37+64(%%rsi), %%dl \n" \
        "movb 38+64(%%rsi), %%dl \n" \
        "movb 39+64(%%rsi), %%dl \n" \
        "movb 40+64(%%rsi), %%dl \n" \
        "movb 41+64(%%rsi), %%dl \n" \
        "movb 42+64(%%rsi), %%dl \n" \
        "movb 43+64(%%rsi), %%dl \n" \
        "movb 44+64(%%rsi), %%dl \n" \
        "movb 45+64(%%rsi), %%dl \n" \
        "movb 46+64(%%rsi), %%dl \n" \
        "movb 47+64(%%rsi), %%dl \n" \
        "movb 48+64(%%rsi), %%dl \n" \
        "movb 49+64(%%rsi), %%dl \n" \
        "movb 50+64(%%rsi), %%dl \n" \
        "movb 51+64(%%rsi), %%dl \n" \
        "movb 52+64(%%rsi), %%dl \n" \
        "movb 53+64(%%rsi), %%dl \n" \
        "movb 54+64(%%rsi), %%dl \n" \
        "movb 55+64(%%rsi), %%dl \n" \
        "movb 56+64(%%rsi), %%dl \n" \
        "movb 57+64(%%rsi), %%dl \n" \
        "movb 58+64(%%rsi), %%dl \n" \
        "movb 59+64(%%rsi), %%dl \n" \
        "movb 60+64(%%rsi), %%dl \n" \
        "movb 61+64(%%rsi), %%dl \n" \
        "movb 62+64(%%rsi), %%dl \n" \
        "movb 63+64(%%rsi), %%dl \n"

#define STORE_MOVD_64 \
        "mov %%edx, 0*4(%%rsi) \n" \
        "mov %%edx, 1*4(%%rsi) \n" \
        "mov %%edx, 2*4(%%rsi) \n" \
        "mov %%edx, 3*4(%%rsi) \n" \
        "mov %%edx, 4*4(%%rsi) \n" \
        "mov %%edx, 5*4(%%rsi) \n" \
        "mov %%edx, 6*4(%%rsi) \n" \
        "mov %%edx, 7*4(%%rsi) \n" \
        "mov %%edx, 8*4(%%rsi) \n" \
        "mov %%edx, 9*4(%%rsi) \n" \
        "mov %%edx, 10*4(%%rsi) \n" \
        "mov %%edx, 11*4(%%rsi) \n" \
        "mov %%edx, 12*4(%%rsi) \n" \
        "mov %%edx, 13*4(%%rsi) \n" \
        "mov %%edx, 14*4(%%rsi) \n" \
        "mov %%edx, 15*4(%%rsi) \n" 

#define LOAD_MOVD_64 \
        "mov 0*4(%%rsi), %%edx \n" \
        "mov 1*4(%%rsi), %%edx \n" \
        "mov 2*4(%%rsi), %%edx \n" \
        "mov 3*4(%%rsi), %%edx \n" \
        "mov 4*4(%%rsi), %%edx \n" \
        "mov 5*4(%%rsi), %%edx \n" \
        "mov 6*4(%%rsi), %%edx \n" \
        "mov 7*4(%%rsi), %%edx \n" \
        "mov 8*4(%%rsi), %%edx \n" \
        "mov 9*4(%%rsi), %%edx \n" \
        "mov 10*4(%%rsi), %%edx \n" \
        "mov 11*4(%%rsi), %%edx \n" \
        "mov 12*4(%%rsi), %%edx \n" \
        "mov 13*4(%%rsi), %%edx \n" \
        "mov 14*4(%%rsi), %%edx \n" \
        "mov 15*4(%%rsi), %%edx \n" 


/* rdtscp: reads current timestamp to EDX:EAX and also sets ECX
 * higher 32-bits of RAX, RDX and RCX are cleared
 */
#define TIMING_BEG              "rdtscp \n" \
                                "lfence \n" \
                                "mov %%edx, %%r9d \n" \
                                "mov %%eax, %%r8d \n"

/* r9d = old EDX
 * r8d = old EAX
 * Here is what we do to compute t1 and t2:
 * - RDX holds t2
 * - RAX holds t1
 */
#define TIMING_END              "mfence \n" \
                                "rdtscp \n" \
                                "lfence \n" \
                                "shl $32, %%rdx \n" \
                                "mov %%eax, %%edx \n" \
                                "mov %%r9d, %%eax \n" \
                                "shl $32, %%rax \n" \
                                "mov %%r8d, %%eax \n" \
                                "mov %%rax, %[t1] \n" \
                                "mov %%rdx, %[t2] \n"

#define TIMING_END_NOFENCE      "rdtscp \n" \
                                "shl $32, %%rdx \n" \
                                "mov %%eax, %%edx \n" \
                                "mov %%r9d, %%eax \n" \
                                "shl $32, %%rax \n" \
                                "mov %%r8d, %%eax \n" \
                                "mov %%rax, %[t1] \n" \
                                "mov %%rdx, %[t2] \n"

/*
 64-byte baseline
 */
uint64_t store_64byte_nofence_baseline(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_nofence_baseline\n");
    KERNEL_BEGIN
    // vmovdqa: 32-byte store to memory
    asm volatile(LOAD_VALUE
        LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_nofence_baseline(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
//    printf("load_64byte_fence\n");
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovdqa 1*32(%%rsi), %%ymm1 \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_nofence_movq_baseline(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_nofence_movq_baseline\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_nofence_movq_baseline(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("load_64byte_nofence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq 0*8(%%rsi), %%rdx \n"
        "movq 1*8(%%rsi), %%rdx \n"
        "movq 2*8(%%rsi), %%rdx \n"
        "movq 3*8(%%rsi), %%rdx \n"
        "movq 4*8(%%rsi), %%rdx \n"
        "movq 5*8(%%rsi), %%rdx \n"
        "movq 6*8(%%rsi), %%rdx \n"
        "movq 7*8(%%rsi), %%rdx \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}


/*
 * 64-byte benchmarks
 */
uint64_t store_64byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_fence\n");
    KERNEL_BEGIN
    // vmovdqa: 32-byte store to memory
    asm volatile(LOAD_VALUE
        LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_nofence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_nofence\n");
    KERNEL_BEGIN
    // vmovdqa: 32-byte store to memory
    asm volatile(LOAD_VALUE
        LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflush(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_clflush\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflush (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflush_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_clflush_fence\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflush (%%rsi) \n"
	"mfence\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clwb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_clwb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clwb (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clwb_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_clwb_fence\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clwb (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflushopt(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_clflushopt\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflushopt (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflushopt_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("store_64byte_clflushopt_fence\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflushopt (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t nstore_64byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("nstore_64byte_fence\n");
    KERNEL_BEGIN
    /*
     * vmovntpd: 32-byte non-temporal store (check below)
     * https://software.intel.com/en-us/node/524246
     */
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovntpd %%ymm0, 0*32(%%rsi) \n"
        "vmovntpd %%ymm0, 1*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t nstore_64byte_nofence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
//    printf("nstore_64byte_nofence\n");
    KERNEL_BEGIN
    /*
     * vmovntpd: 32-byte non-temporal store (check below)
     * https://software.intel.com/en-us/node/524246
     */
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovntpd %%ymm0, 0*32(%%rsi) \n"
        "vmovntpd %%ymm0, 1*32(%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
//    printf("load_64byte_fence\n");
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovdqa 1*32(%%rsi), %%ymm1 \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_nofence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
//    printf("load_64byte_nofence\n");
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovdqa 1*32(%%rsi), %%ymm1 \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_fence_nt(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    /*
     * Requires avx2
     * https://www.felixcloutier.com/x86/MOVNTDQA.html
     */
//    printf("load_64byte_fence_nt\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovntdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovntdqa 1*32(%%rsi), %%ymm1 \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_nofence_nt(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    /*
     * Requires avx2
     * https://www.felixcloutier.com/x86/MOVNTDQA.html
     */
//    printf("load_64byte_nofence_nt\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovntdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovntdqa 1*32(%%rsi), %%ymm1 \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}


uint64_t baseline(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("baseline\n");
    KERNEL_BEGIN
    asm volatile(CLEAR_PIPELINE
        TIMING_BEG
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        :: REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_fence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_nofence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_nofence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflush_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflush_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clflush (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflush_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflush_fence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clflush (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clwb_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clwb_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clwb (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clwb_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clwb_fence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clwb (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("load_64byte_fence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq 0*8(%%rsi), %%rdx \n"
        "movq 1*8(%%rsi), %%rdx \n"
        "movq 2*8(%%rsi), %%rdx \n"
        "movq 3*8(%%rsi), %%rdx \n"
        "movq 4*8(%%rsi), %%rdx \n"
        "movq 5*8(%%rsi), %%rdx \n"
        "movq 6*8(%%rsi), %%rdx \n"
        "movq 7*8(%%rsi), %%rdx \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflushopt_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflushopt_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clflushopt (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflushopt_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflushopt_fence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clflushopt (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_nofence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("load_64byte_nofence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq 0*8(%%rsi), %%rdx \n"
        "movq 1*8(%%rsi), %%rdx \n"
        "movq 2*8(%%rsi), %%rdx \n"
        "movq 3*8(%%rsi), %%rdx \n"
        "movq 4*8(%%rsi), %%rdx \n"
        "movq 5*8(%%rsi), %%rdx \n"
        "movq 6*8(%%rsi), %%rdx \n"
        "movq 7*8(%%rsi), %%rdx \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

// movb

uint64_t store_64byte_fence_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_fence_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVB_64
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_nofence_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_nofence_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVB_64
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflush_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflush_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVB_64
        "clflush (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflush_fence_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflush_fence_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVB_64
        "clflush (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clwb_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clwb_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVB_64
        "clwb (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clwb_fence_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clwb_fence_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVB_64
        "clwb (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflushopt_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflushopt_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVB_64
        "clflushopt (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflushopt_fence_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflushopt_fence_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVB_64
        "clflushopt (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_fence_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("load_64byte_fence_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	LOAD_MOVB_64
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_nofence_movb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("load_64byte_nofence_movb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	LOAD_MOVB_64
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

// movd

uint64_t store_64byte_fence_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_fence_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVD_64
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_nofence_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_nofence_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVD_64
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflush_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflush_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVD_64
        "clflush (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflush_fence_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflush_fence_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVD_64
        "clflush (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clwb_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clwb_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVD_64
        "clwb (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clwb_fence_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clwb_fence_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVD_64
        "clwb (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflushopt_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflushopt_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVD_64
        "clflushopt (%%rsi) \n"
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_64byte_clflushopt_fence_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("store_64byte_clflushopt_fence_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	STORE_MOVD_64
        "clflushopt (%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_fence_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("load_64byte_fence_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	LOAD_MOVD_64
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_64byte_nofence_movd(char *addr) {
    uint64_t t1 = 0, t2 = 0;
//    printf("load_64byte_nofence_movd\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	LOAD_MOVD_64
        TIMING_END_NOFENCE
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

// +++++++++++++++++++++++++++++++
// bandwidth benchmarks
//    &bw_load_64byte_fence,

uint64_t bw_load_64byte_fence(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    long i;
    KERNEL_BEGIN
//    printf("bw_load_64byte_fence\n");
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %4, %4\n\t"
"LOOP_bw_load_64byte_fence:\n\t"
        "vmovdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovdqa 1*32(%%rsi), %%ymm1 \n"
	"inc %4\n\t"
	"cmp %3,%4\n\t"
	"jb LOOP_bw_load_64byte_fence\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
	: [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

//    &bw_load_64byte_fence_nt,

uint64_t bw_load_64byte_fence_nt(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    long i;
    /*
     * Requires avx2
     * https://www.felixcloutier.com/x86/MOVNTDQA.html
     */
//    printf("bw_load_64byte_fence_nt\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %4, %4\n\t"
"LOOP_bw_load_64byte_fence_nt:\n\t"
        "vmovntdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovntdqa 1*32(%%rsi), %%ymm1 \n"
	"inc %4\n\t"
	"cmp %3,%4\n\t"
	"jb LOOP_bw_load_64byte_fence_nt\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

//    &bw_store_64byte_fence,

uint64_t bw_store_64byte_fence(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_store_64byte_fence\n");
    KERNEL_BEGIN
    // vmovdqa: 32-byte store to memory
    asm volatile(LOAD_VALUE
        LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %5, %5\n\t"
"LOOP_bw_store_64byte_fence:\n\t"
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
	"inc %5\n\t"
	"cmp %4,%5\n\t"
	"jb LOOP_bw_store_64byte_fence\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

//    &bw_store_64byte_clflush,

uint64_t bw_store_64byte_clflush(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_store_64byte_clflush\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %5, %5\n\t"
"LOOP_bw_store_64byte_clflush:\n\t"
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflush (%%rsi) \n"
	"inc %5\n\t"
	"cmp %4,%5\n\t"
	"jb LOOP_bw_store_64byte_clflush\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

#ifdef AEP_SUPPORTED
//    &bw_store_64byte_clwb,

uint64_t bw_store_64byte_clwb(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_store_64byte_clwb\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %5, %5\n\t"
"LOOP_bw_store_64byte_clwb:\n\t"
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clwb (%%rsi) \n"
	"inc %5\n\t"
	"cmp %4,%5\n\t"
	"jb LOOP_bw_store_64byte_clwb\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

//    &bw_store_64byte_clflushopt,

uint64_t bw_store_64byte_clflushopt(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_store_64byte_clflushopt\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %5, %5\n\t"
"LOOP_bw_store_64byte_fence_clflushopt:\n\t"
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflushopt (%%rsi) \n"
	"inc %5\n\t"
	"cmp %4,%5\n\t"
	"jb LOOP_bw_store_64byte_fence_clflushopt\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

#endif

//    &bw_nstore_64byte_fence,

uint64_t bw_nstore_64byte_fence(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_nstore_64byte_fence\n");
    KERNEL_BEGIN
    /*
     * vmovntpd: 32-byte non-temporal store (check below)
     * https://software.intel.com/en-us/node/524246
     */
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %5, %5\n\t"
"LOOP_bw_nstore_64byte_fence:\n\t"
        "vmovntpd %%ymm0, 0*32(%%rsi) \n"
        "vmovntpd %%ymm0, 1*32(%%rsi) \n"
	"inc %5\n\t"
	"cmp %4,%5\n\t"
	"jb LOOP_bw_nstore_64byte_fence\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

//    &bw_store_64byte_fence_movq,

uint64_t bw_store_64byte_fence_movq(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    long i;
//    printf("bw_store_64byte_fence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %4, %4\n\t"
"LOOP_bw_store_64byte_fence_movq:\n\t"
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
	"movq %%rdx, 7*8(%%rsi) \n"
// TODO fence
	"inc %4\n\t"
	"cmp %3,%4\n\t"
	"jb LOOP_bw_store_64byte_fence_movq\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

//    &bw_store_64byte_clflush_movq,

uint64_t bw_store_64byte_clflush_movq(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    long i;
//    printf("bw_store_64byte_clflush_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %4, %4\n\t"
"LOOP_bw_store_64byte_clflush_movq:\n\t"
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clflush (%%rsi) \n"
	"inc %4\n\t"
	"cmp %3,%4\n\t"
	"jb LOOP_bw_store_64byte_clflush_movq\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

//    &bw_load_64byte_fence_movq,

uint64_t bw_load_64byte_fence_movq(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    long i;
//    printf("bw_load_64byte_fence_movq\n");
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	"xor %4, %4\n\t"
"LOOP_bw_load_64byte_fence_movq:\n\t"
        "movq 0*8(%%rsi), %%rdx \n"
        "movq 1*8(%%rsi), %%rdx \n"
        "movq 2*8(%%rsi), %%rdx \n"
        "movq 3*8(%%rsi), %%rdx \n"
        "movq 4*8(%%rsi), %%rdx \n"
        "movq 5*8(%%rsi), %%rdx \n"
        "movq 6*8(%%rsi), %%rdx \n"
        "movq 7*8(%%rsi), %%rdx \n"
//TODO fence
	"inc %4\n\t"
	"cmp %3,%4\n\t"
	"jb LOOP_bw_load_64byte_fence_movq\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t bw_baseline(char *addr, long loop_num) {
    uint64_t t1 = 0, t2 = 0;
    long i;
//    printf("bw_baseline\n");
    KERNEL_BEGIN
    asm volatile(CLEAR_PIPELINE
        TIMING_BEG
	"xor %3, %3\n\t"
"LOOP_bw_baseline:\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_baseline\n\t"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : "r"(loop_num), "r" (i)
	: REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

// +++++++++++++++++++++++++++++++
// bandwidth benchmarks, sequential
//    &bw_s_load_64byte_fence,

uint64_t bw_s_load_64byte_fence(char *addr, long loop_num) {
    long i;
    //    printf("bw_s_load_64byte_fence\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_fence:\n\t"
        "vmovdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovdqa 1*32(%%rsi), %%ymm1 \n"
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_fence\n\t"
        : 
	: [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_load_64byte_nofence,

uint64_t bw_s_load_64byte_nofence(char *addr, long loop_num) {
    long i;
    //    printf("bw_s_load_64byte_nofence\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_nofence:\n\t"
        "vmovdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovdqa 1*32(%%rsi), %%ymm1 \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_nofence\n\t"
        : 
	: [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_load_64byte_fence_nt,

uint64_t bw_s_load_64byte_fence_nt(char *addr, long loop_num) {
    long i;
    /*
     * Requires avx2
     * https://www.felixcloutier.com/x86/MOVNTDQA.html
     */
//    printf("bw_s_load_64byte_fence_nt\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_fence_nt:\n\t"
        "vmovntdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovntdqa 1*32(%%rsi), %%ymm1 \n"
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_fence_nt\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_load_64byte_nofence_nt,

uint64_t bw_s_load_64byte_nofence_nt(char *addr, long loop_num) {
    long i;
    /*
     * Requires avx2
     * https://www.felixcloutier.com/x86/MOVNTDQA.html
     */
    //    printf("bw_s_load_64byte_nofence_nt\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_nofence_nt:\n\t"
        "vmovntdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovntdqa 1*32(%%rsi), %%ymm1 \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_nofence_nt\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_store_64byte_fence,

uint64_t bw_s_store_64byte_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_store_64byte_fence\n");
    // vmovdqa: 32-byte store to memory
    asm volatile(LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_store_64byte_fence:\n\t"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_store_64byte_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_store_64byte_nofence,

uint64_t bw_s_store_64byte_nofence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_store_64byte_nofence\n");
    // vmovdqa: 32-byte store to memory
    asm volatile(LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_store_64byte_nofence:\n\t"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_store_64byte_nofence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_store_64byte_clflush,

uint64_t bw_s_store_64byte_clflush(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_store_64byte_clflush\n");
    asm volatile(
        LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_store_64byte_clflush:\n\t"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clflush (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_store_64byte_clflush\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_store_64byte_clflush_fence,

uint64_t bw_s_store_64byte_clflush_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_store_64byte_clflush_fence\n");
    asm volatile(
        LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_store_64byte_clflush_fence:\n\t"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clflush (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_store_64byte_clflush_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

#ifdef AEP_SUPPORTED
//    &bw_s_store_64byte_clwb,

uint64_t bw_s_store_64byte_clwb(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_store_64byte_clwb\n");
    asm volatile(
        LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_store_64byte_clwb:\n\t"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clwb (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_store_64byte_clwb\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_store_64byte_clwb_fence,

uint64_t bw_s_store_64byte_clwb_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_store_64byte_clwb_fence\n");
    asm volatile(
        LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_store_64byte_clwb_fence:\n\t"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clwb (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_store_64byte_clwb_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_store_64byte_clflushopt,

uint64_t bw_s_store_64byte_clflushopt(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_store_64byte_clflushopt\n");
    asm volatile(
        LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_store_64byte_fence_clflushopt:\n\t"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clflushopt (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_store_64byte_fence_clflushopt\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_store_64byte_clflushopt_fence,

uint64_t bw_s_store_64byte_clflushopt_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_store_64byte_clflushopt_fence\n");
    asm volatile(
        LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_store_64byte_fence_clflushopt_fence:\n\t"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clflushopt (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_store_64byte_fence_clflushopt_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

#endif

//    &bw_s_nstore_64byte_fence,

uint64_t bw_s_nstore_64byte_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_nstore_64byte_fence\n");
    /*
     * vmovntpd: 32-byte non-temporal store (check below)
     * https://software.intel.com/en-us/node/524246
     */
    asm volatile(
        LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_nstore_64byte_fence:\n\t"
        "vmovntpd %%ymm0, 2*32(%%rsi) \n"
        "vmovntpd %%ymm0, 3*32(%%rsi) \n"
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_nstore_64byte_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_s_nstore_64byte_nofence,

uint64_t bw_s_nstore_64byte_nofence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
//    printf("bw_s_nstore_64byte_nofence\n");
    /*
     * vmovntpd: 32-byte non-temporal store (check below)
     * https://software.intel.com/en-us/node/524246
     */
    asm volatile(
        LOAD_VALUE
	LOAD_ADDR
	"xor %3, %3\n\t"
"LOOP_bw_s_nstore_64byte_nofence:\n\t"
        "vmovntpd %%ymm0, 2*32(%%rsi) \n"
        "vmovntpd %%ymm0, 3*32(%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_s_nstore_64byte_nofence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_store_64byte_fence_movq,

uint64_t bw_s_store_64byte_fence_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_s_store_64byte_fence_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_fence_movq:\n\t"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_nofence_movq,

uint64_t bw_s_store_64byte_nofence_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_s_store_64byte_nofence_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_nofence_movq:\n\t"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_nofence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflush_movq,

uint64_t bw_s_store_64byte_clflush_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflush_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflush_movq:\n\t"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "clflush (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflush_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflush_fence_movq,

uint64_t bw_s_store_64byte_clflush_fence_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflush_fence_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflush_fence_movq:\n\t"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "clflush (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflush_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clwb_movq,

uint64_t bw_s_store_64byte_clwb_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clwb_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clwb_movq:\n\t"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "clwb (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clwb_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clwb_fence_movq,

uint64_t bw_s_store_64byte_clwb_fence_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clwb_fence_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clwb_fence_movq:\n\t"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "clwb (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clwb_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflushopt_movq,

uint64_t bw_s_store_64byte_clflushopt_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflushopt_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflushopt_movq:\n\t"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "clflushopt (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflushopt_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflushopt_fence_movq,

uint64_t bw_s_store_64byte_clflushopt_fence_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflushopt_fence_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflushopt_fence_movq:\n\t"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "clflushopt (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflushopt_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_load_64byte_fence_movq,

uint64_t bw_s_load_64byte_fence_movq(char *addr, long loop_num) {
    long i;
//    printf("bw_s_load_64byte_fence_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_fence_movq:\n\t"
        "movq 0*8(%%rsi), %%rdx \n"
        "movq 1*8(%%rsi), %%rdx \n"
        "movq 2*8(%%rsi), %%rdx \n"
        "movq 3*8(%%rsi), %%rdx \n"
        "movq 4*8(%%rsi), %%rdx \n"
        "movq 5*8(%%rsi), %%rdx \n"
        "movq 6*8(%%rsi), %%rdx \n"
        "movq 7*8(%%rsi), %%rdx \n"
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_load_64byte_nofence_movq,

uint64_t bw_s_load_64byte_nofence_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_s_load_64byte_nofence_movq\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_nofence_movq:\n\t"
        "movq 0*8(%%rsi), %%rdx \n"
        "movq 1*8(%%rsi), %%rdx \n"
        "movq 2*8(%%rsi), %%rdx \n"
        "movq 3*8(%%rsi), %%rdx \n"
        "movq 4*8(%%rsi), %%rdx \n"
        "movq 5*8(%%rsi), %%rdx \n"
        "movq 6*8(%%rsi), %%rdx \n"
        "movq 7*8(%%rsi), %%rdx \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_nofence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    +++++++++++++++++++++++++++ movb
//    &bw_s_store_64byte_fence_movb,

uint64_t bw_s_store_64byte_fence_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_s_store_64byte_fence_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_fence_movb:\n\t"
	STORE_MOVB_64
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_s_store_64byte_nofence_movb,

uint64_t bw_s_store_64byte_nofence_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_s_store_64byte_nofence_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_nofence_movb:\n\t"
	STORE_MOVB_64
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_nofence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflush_movb,

uint64_t bw_s_store_64byte_clflush_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflush_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflush_movb:\n\t"
	STORE_MOVB_64
        "clflush (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflush_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflush_fence_movb,

uint64_t bw_s_store_64byte_clflush_fence_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflush_fence_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflush_fence_movb:\n\t"
	STORE_MOVB_64
        "clflush (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflush_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clwb_movb,

uint64_t bw_s_store_64byte_clwb_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clwb_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clwb_movb:\n\t"
	STORE_MOVB_64
        "clwb (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clwb_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clwb_fence_movb,

uint64_t bw_s_store_64byte_clwb_fence_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clwb_fence_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clwb_fence_movb:\n\t"
	STORE_MOVB_64
        "clwb (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clwb_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflushopt_movb,

uint64_t bw_s_store_64byte_clflushopt_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflushopt_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflushopt_movb:\n\t"
	STORE_MOVB_64
        "clflushopt (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflushopt_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflushopt_fence_movb,

uint64_t bw_s_store_64byte_clflushopt_fence_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflushopt_fence_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflushopt_fence_movb:\n\t"
	STORE_MOVB_64
        "clflushopt (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflushopt_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_load_64byte_fence_movb,

uint64_t bw_s_load_64byte_fence_movb(char *addr, long loop_num) {
    long i;
//    printf("bw_s_load_64byte_fence_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_fence_movb:\n\t"
	LOAD_MOVB_64
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_load_64byte_nofence_movb,

uint64_t bw_s_load_64byte_nofence_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_s_load_64byte_nofence_movb\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_nofence_movb:\n\t"
	LOAD_MOVB_64
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_nofence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

// movd
// ----------------------------------------------
//    +++++++++++++++++++++++++++ movd double word
//    &bw_s_store_64byte_fence_movd,

uint64_t bw_s_store_64byte_fence_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_s_store_64byte_fence_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_fence_movd:\n\t"
	STORE_MOVD_64
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_s_store_64byte_nofence_movd,

uint64_t bw_s_store_64byte_nofence_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_s_store_64byte_nofence_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_nofence_movd:\n\t"
	STORE_MOVD_64
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_nofence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflush_movd,

uint64_t bw_s_store_64byte_clflush_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflush_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflush_movd:\n\t"
	STORE_MOVD_64
        "clflush (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflush_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflush_fence_movd,

uint64_t bw_s_store_64byte_clflush_fence_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflush_fence_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflush_fence_movd:\n\t"
	STORE_MOVD_64
        "clflush (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflush_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clwb_movd,

uint64_t bw_s_store_64byte_clwb_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clwb_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clwb_movd:\n\t"
	STORE_MOVD_64
        "clwb (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clwb_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clwb_fence_movd,

uint64_t bw_s_store_64byte_clwb_fence_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clwb_fence_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clwb_fence_movd:\n\t"
	STORE_MOVD_64
        "clwb (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clwb_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflushopt_movd,

uint64_t bw_s_store_64byte_clflushopt_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflushopt_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflushopt_movd:\n\t"
	STORE_MOVD_64
        "clflushopt (%%rsi) \n"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflushopt_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_store_64byte_clflushopt_fence_movd,

uint64_t bw_s_store_64byte_clflushopt_fence_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_store_64byte_clflushopt_fence_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_store_64byte_clflushopt_fence_movd:\n\t"
	STORE_MOVD_64
        "clflushopt (%%rsi) \n"
	"mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_store_64byte_clflushopt_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_load_64byte_fence_movd,

uint64_t bw_s_load_64byte_fence_movd(char *addr, long loop_num) {
    long i;
//    printf("bw_s_load_64byte_fence_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_fence_movd:\n\t"
	LOAD_MOVD_64
        "mfence\n\t"
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_load_64byte_nofence_movd,

uint64_t bw_s_load_64byte_nofence_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_s_load_64byte_nofence_movd\n");
    asm volatile(
	LOAD_ADDR
	"xor %2, %2\n\t"
"LOOP_bw_s_load_64byte_nofence_movd:\n\t"
	LOAD_MOVD_64
	"add $64, %%rsi\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_s_load_64byte_nofence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

// movd
// ----------------------------------------------

uint64_t bw_s_baseline(char *addr, long loop_num) {
    long i;
//    printf("bw_baseline\n");
    asm volatile(
	"xor %1, %1\n\t"
"LOOP_bw_s_baseline:\n\t"
	"inc %1\n\t"
	"cmp %0,%1\n\t"
	"jb LOOP_bw_s_baseline\n\t"
        : 
        : "r"(loop_num), "r" (i)
	: REGISTERS_NONSSE);
    return 0;
}

// +++++++++++++++++++++++++++++++++++++++++++
// bandwidth benchmarks random access
//    &bw_r_load_64byte_fence,

uint64_t bw_r_load_64byte_fence(char *addr, long loop_num) {
    long i;
//    printf("bw_r_load_64byte_fence\n");
    asm volatile(
	CLEAR_PIPELINE
	"xor %2, %2\n"
"LOOP_bw_r_load_64byte_fence:\n"
        "vmovdqa 2*32(%0), %%ymm0 \n"
        "vmovdqa 3*32(%0), %%ymm1 \n"
	"mov (%0), %0 \n"
        "mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_fence\n\t"
        : 
	: [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_load_64byte_nofence,

uint64_t bw_r_load_64byte_nofence(char *addr, long loop_num) {
    long i;
//    printf("bw_r_load_64byte_nofence\n");
    asm volatile(
	CLEAR_PIPELINE
	"xor %2, %2\n"
"LOOP_bw_r_load_64byte_nofence:\n"
        "vmovdqa 2*32(%0), %%ymm0 \n"
        "vmovdqa 3*32(%0), %%ymm1 \n"
	"mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_nofence\n\t"
        : 
	: [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_load_64byte_fence_nt,

uint64_t bw_r_load_64byte_fence_nt(char *addr, long loop_num) {
    long i;
    /*
     * Requires avx2
     * https://www.felixcloutier.com/x86/MOVNTDQA.html
     */
//    printf("bw_r_load_64byte_fence_nt\n");
    asm volatile(LOAD_ADDR
        CLEAR_PIPELINE
	"xor %2, %2\n\t"
"LOOP_bw_r_load_64byte_fence_nt:\n\t"
        "vmovntdqa 2*32(%0), %%ymm0 \n"
        "vmovntdqa 3*32(%0), %%ymm1 \n"
        "mov (%0), %0 \n"
        "mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_fence_nt\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_load_64byte_nofence_nt,

uint64_t bw_r_load_64byte_nofence_nt(char *addr, long loop_num) {
    long i;
    /*
     * Requires avx2
     * https://www.felixcloutier.com/x86/MOVNTDQA.html
     */
//    printf("bw_r_load_64byte_nofence_nt\n");
    asm volatile(LOAD_ADDR
        CLEAR_PIPELINE
	"xor %2, %2\n\t"
"LOOP_bw_r_load_64byte_nofence_nt:\n\t"
        "vmovntdqa 2*32(%0), %%ymm0 \n"
        "vmovntdqa 3*32(%0), %%ymm1 \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_nofence_nt\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_store_64byte_fence,

uint64_t bw_r_store_64byte_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_store_64byte_fence\n");
    // vmovdqa: 32-byte store to memory
    asm volatile(
        LOAD_VALUE
        CLEAR_PIPELINE
	"xor %3, %3\n\t"
"LOOP_bw_r_store_64byte_fence:\n\t"
        "vmovdqa %%ymm0, 2*32(%0) \n"
        "vmovdqa %%ymm0, 3*32(%0) \n"
        "mov (%0), %0 \n"
        "mfence\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_store_64byte_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_store_64byte_nofence,

uint64_t bw_r_store_64byte_nofence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_store_64byte_nofence\n");
    // vmovdqa: 32-byte store to memory
    asm volatile(
        LOAD_VALUE
        CLEAR_PIPELINE
	"xor %3, %3\n\t"
"LOOP_bw_r_store_64byte_nofence:\n\t"
        "vmovdqa %%ymm0, 2*32(%0) \n"
        "vmovdqa %%ymm0, 3*32(%0) \n"
        "mov (%0), %0 \n"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_store_64byte_nofence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_store_64byte_clflush,

uint64_t bw_r_store_64byte_clflush(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_store_64byte_clflush\n");
    asm volatile(
        LOAD_VALUE
	"xor %3, %3\n\t"
"LOOP_bw_r_store_64byte_clflush:\n\t"
        "vmovdqa %%ymm0, 2*32(%0) \n"
        "vmovdqa %%ymm0, 3*32(%0) \n"
        "clflush (%0) \n"
        "mov (%0), %0 \n"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_store_64byte_clflush\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_store_64byte_clflush_fence,

uint64_t bw_r_store_64byte_clflush_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_store_64byte_clflush_fence\n");
    asm volatile(
        LOAD_VALUE
	"xor %3, %3\n\t"
"LOOP_bw_r_store_64byte_clflush_fence:\n\t"
        "vmovdqa %%ymm0, 2*32(%0) \n"
        "vmovdqa %%ymm0, 3*32(%0) \n"
        "clflush (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_store_64byte_clflush_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

#ifdef AEP_SUPPORTED
//    &bw_r_store_64byte_clwb,

uint64_t bw_r_store_64byte_clwb(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_store_64byte_clwb\n");
    asm volatile(
        LOAD_VALUE
	"xor %3, %3\n\t"
"LOOP_bw_r_store_64byte_clwb:\n\t"
        "vmovdqa %%ymm0, 2*32(%0) \n"
        "vmovdqa %%ymm0, 3*32(%0) \n"
        "clwb (%0) \n"
        "mov (%0), %0 \n"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_store_64byte_clwb\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_store_64byte_clwb_fence,

uint64_t bw_r_store_64byte_clwb_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_store_64byte_clwb_fence\n");
    asm volatile(
        LOAD_VALUE
	"xor %3, %3\n\t"
"LOOP_bw_r_store_64byte_clwb_fence:\n\t"
        "vmovdqa %%ymm0, 2*32(%0) \n"
        "vmovdqa %%ymm0, 3*32(%0) \n"
        "clwb (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_store_64byte_clwb_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_store_64byte_clflushopt,

uint64_t bw_r_store_64byte_clflushopt(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_store_64byte_clflushopt\n");
    asm volatile(
        LOAD_VALUE
	"xor %3, %3\n\t"
"LOOP_bw_r_store_64byte_fence_clflushopt:\n\t"
        "vmovdqa %%ymm0, 2*32(%0) \n"
        "vmovdqa %%ymm0, 3*32(%0) \n"
        "clflushopt (%0) \n"
        "mov (%0), %0 \n"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_store_64byte_fence_clflushopt\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_store_64byte_clflushopt_fence,

uint64_t bw_r_store_64byte_clflushopt_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_store_64byte_clflushopt_fence\n");
    asm volatile(
        LOAD_VALUE
	"xor %3, %3\n\t"
"LOOP_bw_r_store_64byte_fence_clflushopt_fence:\n\t"
        "vmovdqa %%ymm0, 2*32(%0) \n"
        "vmovdqa %%ymm0, 3*32(%0) \n"
        "clflushopt (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_store_64byte_fence_clflushopt_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

#endif

//    &bw_r_nstore_64byte_fence,

uint64_t bw_r_nstore_64byte_fence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_nstore_64byte_fence\n");
    /*
     * vmovntpd: 32-byte non-temporal store (check below)
     * https://software.intel.com/en-us/node/524246
     */
    asm volatile(
        LOAD_VALUE
	"xor %3, %3\n\t"
"LOOP_bw_r_nstore_64byte_fence:\n\t"
        "vmovntpd %%ymm0, 2*32(%0) \n"
        "vmovntpd %%ymm0, 3*32(%0) \n"
        "mov (%0), %0 \n"
        "mfence\n\t"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_nstore_64byte_fence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_nstore_64byte_nofence,

uint64_t bw_r_nstore_64byte_nofence(char *addr, long loop_num) {
    uint64_t value = 0xC0FFEEEEBABE0000;
    long i;
    //    printf("bw_r_nstore_64byte_nofence\n");
    /*
     * vmovntpd: 32-byte non-temporal store (check below)
     * https://software.intel.com/en-us/node/524246
     */
    asm volatile(
        LOAD_VALUE
	"xor %3, %3\n\t"
"LOOP_bw_r_nstore_64byte_nofence:\n\t"
        "vmovntpd %%ymm0, 2*32(%0) \n"
        "vmovntpd %%ymm0, 3*32(%0) \n"
        "mov (%0), %0 \n"
	"inc %3\n\t"
	"cmp %2,%3\n\t"
	"jb LOOP_bw_r_nstore_64byte_nofence\n\t"
        : 
        : [memarea] "r" (addr), [value] "m" (value), "r"(loop_num), "r" (i)
        : REGISTERS);
    return 0;
}

//    &bw_r_store_64byte_fence_movq,

uint64_t bw_r_store_64byte_fence_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_fence_movq\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_fence_movq:\n\t"
        "movq %%rdx, 8*8(%0) \n"
        "movq %%rdx, 9*8(%0) \n"
        "movq %%rdx, 10*8(%0) \n"
        "movq %%rdx, 11*8(%0) \n"
        "movq %%rdx, 12*8(%0) \n"
        "movq %%rdx, 13*8(%0) \n"
        "movq %%rdx, 14*8(%0) \n"
        "movq %%rdx, 15*8(%0) \n"
        "mov (%0), %0 \n"
        "mfence \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_nofence_movq,

uint64_t bw_r_store_64byte_nofence_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_nofence_movq\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_nofence_movq:\n\t"
        "movq %%rdx, 8*8(%0) \n"
        "movq %%rdx, 9*8(%0) \n"
        "movq %%rdx, 10*8(%0) \n"
        "movq %%rdx, 11*8(%0) \n"
        "movq %%rdx, 12*8(%0) \n"
        "movq %%rdx, 13*8(%0) \n"
        "movq %%rdx, 14*8(%0) \n"
        "movq %%rdx, 15*8(%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_nofence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflush_movq,

uint64_t bw_r_store_64byte_clflush_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflush_movq\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflush_movq:\n\t"
        "movq %%rdx, 8*8(%0) \n"
        "movq %%rdx, 9*8(%0) \n"
        "movq %%rdx, 10*8(%0) \n"
        "movq %%rdx, 11*8(%0) \n"
        "movq %%rdx, 12*8(%0) \n"
        "movq %%rdx, 13*8(%0) \n"
        "movq %%rdx, 14*8(%0) \n"
        "movq %%rdx, 15*8(%0) \n"
        "clflush (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflush_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflush_fence_movq,

uint64_t bw_r_store_64byte_clflush_fence_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflush_fence_movq\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflush_fence_movq:\n\t"
        "movq %%rdx, 8*8(%0) \n"
        "movq %%rdx, 9*8(%0) \n"
        "movq %%rdx, 10*8(%0) \n"
        "movq %%rdx, 11*8(%0) \n"
        "movq %%rdx, 12*8(%0) \n"
        "movq %%rdx, 13*8(%0) \n"
        "movq %%rdx, 14*8(%0) \n"
        "movq %%rdx, 15*8(%0) \n"
        "clflush (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflush_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_load_64byte_fence_movq,

uint64_t bw_r_load_64byte_fence_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_load_64byte_fence_movq\n");
    asm volatile(
        CLEAR_PIPELINE
	"xor %2, %2\n\t"
"LOOP_bw_r_load_64byte_fence_movq:\n\t"
        "movq 8*8(%0), %%rdx \n"
        "movq 9*8(%0), %%rdx \n"
        "movq 10*8(%0), %%rdx \n"
        "movq 11*8(%0), %%rdx \n"
        "movq 12*8(%0), %%rdx \n"
        "movq 13*8(%0), %%rdx \n"
        "movq 14*8(%0), %%rdx \n"
        "movq 15*8(%0), %%rdx \n"
        "mov (%0), %0 \n"
        "mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_load_64byte_nofence_movq,

uint64_t bw_r_load_64byte_nofence_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_load_64byte_nofence_movq\n");
    asm volatile(
        CLEAR_PIPELINE
	"xor %2, %2\n\t"
"LOOP_bw_r_load_64byte_nofence_movq:\n\t"
        "movq 8*8(%0), %%rdx \n"
        "movq 9*8(%0), %%rdx \n"
        "movq 10*8(%0), %%rdx \n"
        "movq 11*8(%0), %%rdx \n"
        "movq 12*8(%0), %%rdx \n"
        "movq 13*8(%0), %%rdx \n"
        "movq 14*8(%0), %%rdx \n"
        "movq 15*8(%0), %%rdx \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_nofence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clwb_movq,

uint64_t bw_r_store_64byte_clwb_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clwb_movq\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clwb_movq:\n\t"
        "movq %%rdx, 8*8(%0) \n"
        "movq %%rdx, 9*8(%0) \n"
        "movq %%rdx, 10*8(%0) \n"
        "movq %%rdx, 11*8(%0) \n"
        "movq %%rdx, 12*8(%0) \n"
        "movq %%rdx, 13*8(%0) \n"
        "movq %%rdx, 14*8(%0) \n"
        "movq %%rdx, 15*8(%0) \n"
        "clwb (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clwb_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clwb_fence_movq,

uint64_t bw_r_store_64byte_clwb_fence_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clwb_fence_movq\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clwb_fence_movq:\n\t"
        "movq %%rdx, 8*8(%0) \n"
        "movq %%rdx, 9*8(%0) \n"
        "movq %%rdx, 10*8(%0) \n"
        "movq %%rdx, 11*8(%0) \n"
        "movq %%rdx, 12*8(%0) \n"
        "movq %%rdx, 13*8(%0) \n"
        "movq %%rdx, 14*8(%0) \n"
        "movq %%rdx, 15*8(%0) \n"
        "clwb (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clwb_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflushopt_movq,

uint64_t bw_r_store_64byte_clflushopt_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflushopt_movq\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflushopt_movq:\n\t"
        "movq %%rdx, 8*8(%0) \n"
        "movq %%rdx, 9*8(%0) \n"
        "movq %%rdx, 10*8(%0) \n"
        "movq %%rdx, 11*8(%0) \n"
        "movq %%rdx, 12*8(%0) \n"
        "movq %%rdx, 13*8(%0) \n"
        "movq %%rdx, 14*8(%0) \n"
        "movq %%rdx, 15*8(%0) \n"
        "clflushopt (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflushopt_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflushopt_fence_movq,

uint64_t bw_r_store_64byte_clflushopt_fence_movq(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflushopt_fence_movq\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflushopt_fence_movq:\n\t"
        "movq %%rdx, 8*8(%0) \n"
        "movq %%rdx, 9*8(%0) \n"
        "movq %%rdx, 10*8(%0) \n"
        "movq %%rdx, 11*8(%0) \n"
        "movq %%rdx, 12*8(%0) \n"
        "movq %%rdx, 13*8(%0) \n"
        "movq %%rdx, 14*8(%0) \n"
        "movq %%rdx, 15*8(%0) \n"
        "clflushopt (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflushopt_fence_movq\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

// bw_r_xxxx

//    &bw_r_store_64byte_fence_movb,

uint64_t bw_r_store_64byte_fence_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_fence_movb\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_fence_movb:\n\t"
	STORE_MOVB_64
        "mov (%0), %0 \n"
        "mfence \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_nofence_movb,

uint64_t bw_r_store_64byte_nofence_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_nofence_movb\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_nofence_movb:\n\t"
	STORE_MOVB_64
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_nofence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflush_movb,

uint64_t bw_r_store_64byte_clflush_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflush_movb\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflush_movb:\n\t"
	STORE_MOVB_64
        "clflush (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflush_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflush_fence_movb,

uint64_t bw_r_store_64byte_clflush_fence_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflush_fence_movb\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflush_fence_movb:\n\t"
	STORE_MOVB_64
        "clflush (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflush_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clwb_movb,

uint64_t bw_r_store_64byte_clwb_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clwb_movb\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clwb_movb:\n\t"
	STORE_MOVB_64
        "clwb (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clwb_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clwb_fence_movb,

uint64_t bw_r_store_64byte_clwb_fence_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clwb_fence_movb\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clwb_fence_movb:\n\t"
	STORE_MOVB_64
        "clwb (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clwb_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflushopt_movb,

uint64_t bw_r_store_64byte_clflushopt_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflushopt_movb\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflushopt_movb:\n\t"
	STORE_MOVB_64
        "clflushopt (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflushopt_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflushopt_fence_movb,

uint64_t bw_r_store_64byte_clflushopt_fence_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflushopt_fence_movb\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflushopt_fence_movb:\n\t"
	STORE_MOVB_64
        "clflushopt (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflushopt_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_load_64byte_fence_movb,

uint64_t bw_r_load_64byte_fence_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_load_64byte_fence_movb\n");
    asm volatile(
        CLEAR_PIPELINE
	"xor %2, %2\n\t"
"LOOP_bw_r_load_64byte_fence_movb:\n\t"
	LOAD_MOVB_64
        "mov (%0), %0 \n"
        "mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_fence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_load_64byte_nofence_movb,

uint64_t bw_r_load_64byte_nofence_movb(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_load_64byte_nofence_movb\n");
    asm volatile(
        CLEAR_PIPELINE
	"xor %2, %2\n\t"
"LOOP_bw_r_load_64byte_nofence_movb:\n\t"
	LOAD_MOVB_64
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_nofence_movb\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

// bw_r_movd -- double word (32 bit)
//    &bw_r_store_64byte_fence_movd,

uint64_t bw_r_store_64byte_fence_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_fence_movd\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_fence_movd:\n\t"
	STORE_MOVD_64
        "mov (%0), %0 \n"
        "mfence \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_nofence_movd,

uint64_t bw_r_store_64byte_nofence_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_nofence_movd\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_nofence_movd:\n\t"
	STORE_MOVD_64
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_nofence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflush_movd,

uint64_t bw_r_store_64byte_clflush_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflush_movd\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflush_movd:\n\t"
	STORE_MOVD_64
        "clflush (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflush_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflush_fence_movd,

uint64_t bw_r_store_64byte_clflush_fence_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflush_fence_movd\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflush_fence_movd:\n\t"
	STORE_MOVD_64
        "clflush (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflush_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clwb_movd,

uint64_t bw_r_store_64byte_clwb_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clwb_movd\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clwb_movd:\n\t"
	STORE_MOVD_64
        "clwb (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clwb_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clwb_fence_movd,

uint64_t bw_r_store_64byte_clwb_fence_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clwb_fence_movd\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clwb_fence_movd:\n\t"
	STORE_MOVD_64
        "clwb (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clwb_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflushopt_movd,

uint64_t bw_r_store_64byte_clflushopt_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflushopt_movd\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflushopt_movd:\n\t"
	STORE_MOVD_64
        "clflushopt (%0) \n"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflushopt_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_store_64byte_clflushopt_fence_movd,

uint64_t bw_r_store_64byte_clflushopt_fence_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_store_64byte_clflushopt_fence_movd\n");
    asm volatile(
	"xor %2, %2\n\t"
"LOOP_bw_r_store_64byte_clflushopt_fence_movd:\n\t"
	STORE_MOVD_64
        "clflushopt (%0) \n"
        "mov (%0), %0 \n"
	"mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_store_64byte_clflushopt_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_load_64byte_fence_movd,

uint64_t bw_r_load_64byte_fence_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_load_64byte_fence_movd\n");
    asm volatile(
        CLEAR_PIPELINE
	"xor %2, %2\n\t"
"LOOP_bw_r_load_64byte_fence_movd:\n\t"
	LOAD_MOVD_64
        "mov (%0), %0 \n"
        "mfence\n\t"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_fence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

//    &bw_r_load_64byte_nofence_movd,

uint64_t bw_r_load_64byte_nofence_movd(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_load_64byte_nofence_movd\n");
    asm volatile(
        CLEAR_PIPELINE
	"xor %2, %2\n\t"
"LOOP_bw_r_load_64byte_nofence_movd:\n\t"
	LOAD_MOVD_64
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_load_64byte_nofence_movd\n\t"
        : 
        : [memarea] "r" (addr), "r"(loop_num), "r" (i)
        : REGISTERS_NONSSE);
    return 0;
}

// --------------------------------

uint64_t bw_r_baseline(char *addr, long loop_num) {
    long i;
    //    printf("bw_r_baseline\n");
    asm volatile(
        CLEAR_PIPELINE
	"xor %1, %1\n\t"
"LOOP_bw_r_baseline:\n\t"
        "mov (%0), %0 \n"
	"inc %2\n\t"
	"cmp %1,%2\n\t"
	"jb LOOP_bw_r_baseline\n\t"
        : 
        : "r" (addr), "r"(loop_num), "r" (i)
	: REGISTERS_NONSSE);
    return 0;
}

uint64_t(*latency_funcs_64byte[])(char *) = {
    &load_64byte_fence, // load + fence
    &load_64byte_fence_nt, // non-temporal load + fence
    &store_64byte_fence, // store + fence
    &store_64byte_clflush, // store + clflush
    &store_64byte_clwb, // store + clwb
    &store_64byte_clflushopt, // store + clflushopt
    &nstore_64byte_fence, // non-temporal store + fence
    &store_64byte_fence_movq, // store + fence (movq)
    &store_64byte_clflush_movq, // store - clflush (movq)
    &load_64byte_fence_movq, // load + fence (movq)
    &store_64byte_nofence_movq_baseline, // store (movq)
    &load_64byte_nofence_movq_baseline, // load (movq)
    &store_64byte_nofence_baseline, // store
    &load_64byte_nofence_baseline, // load 
    &baseline
};

/*
 * 128-byte benchmarks
 */
uint64_t store_128byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_VALUE
        LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_128byte_clflush(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflush (%%rsi) \n"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clflush 2*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_128byte_clwb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clwb (%%rsi) \n"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clwb 2*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_128byte_clflushopt(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflushopt (%%rsi) \n"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clflushopt 2*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t nstore_128byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	// 2205 
        "vmovntpd %%ymm0, 0*32(%%rsi) \n"
        "vmovntpd %%ymm0, 1*32(%%rsi) \n"
        "vmovntpd %%ymm0, 2*32(%%rsi) \n"
        "vmovntpd %%ymm0, 3*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_128byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovdqa 1*32(%%rsi), %%ymm1 \n"
        "vmovdqa 2*32(%%rsi), %%ymm1 \n"
        "vmovdqa 3*32(%%rsi), %%ymm1 \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_128byte_fence_nt(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
	// 2245
        "vmovntdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovntdqa 1*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 2*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 3*32(%%rsi), %%ymm1 \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_128byte_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_128byte_clflush_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clflush (%%rsi) \n"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "clflush 8*8(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_128byte_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq 0*8(%%rsi), %%rdx \n"
        "movq 1*8(%%rsi), %%rdx \n"
        "movq 2*8(%%rsi), %%rdx \n"
        "movq 3*8(%%rsi), %%rdx \n"
        "movq 4*8(%%rsi), %%rdx \n"
        "movq 5*8(%%rsi), %%rdx \n"
        "movq 6*8(%%rsi), %%rdx \n"
        "movq 7*8(%%rsi), %%rdx \n"
        "movq 8*8(%%rsi), %%rdx \n"
        "movq 9*8(%%rsi), %%rdx \n"
        "movq 10*8(%%rsi), %%rdx \n"
        "movq 11*8(%%rsi), %%rdx \n"
        "movq 12*8(%%rsi), %%rdx \n"
        "movq 13*8(%%rsi), %%rdx \n"
        "movq 14*8(%%rsi), %%rdx \n"
        "movq 15*8(%%rsi), %%rdx \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t(*latency_funcs_128byte[])(char *) = {
    &load_128byte_fence,
    &load_128byte_fence_nt,
    &store_128byte_fence,
    &store_128byte_clflush,
    &store_128byte_clwb,
    &store_128byte_clflushopt,
    &nstore_128byte_fence,
    &store_128byte_fence_movq,
    &store_128byte_clflush_movq,
    &load_128byte_fence_movq,
    &baseline
};

/*
 * 256-byte benchmarks
 */
uint64_t store_256byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_VALUE
        LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "vmovdqa %%ymm0, 4*32(%%rsi) \n"
        "vmovdqa %%ymm0, 5*32(%%rsi) \n"
        "vmovdqa %%ymm0, 6*32(%%rsi) \n"
        "vmovdqa %%ymm0, 7*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}



uint64_t store_256byte_clflush(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflush 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clflush 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 4*32(%%rsi) \n"
        "vmovdqa %%ymm0, 5*32(%%rsi) \n"
        "clflush 4*32(%%rsi) \n"
        "vmovdqa %%ymm0, 6*32(%%rsi) \n"
        "vmovdqa %%ymm0, 7*32(%%rsi) \n"
        "clflush 6*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_256byte_clwb(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clwb 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clwb 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 4*32(%%rsi) \n"
        "vmovdqa %%ymm0, 5*32(%%rsi) \n"
        "clwb 4*32(%%rsi) \n"
        "vmovdqa %%ymm0, 6*32(%%rsi) \n"
        "vmovdqa %%ymm0, 7*32(%%rsi) \n"
        "clwb 6*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_256byte_clflushopt(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa %%ymm0, 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 1*32(%%rsi) \n"
        "clflushopt 0*32(%%rsi) \n"
        "vmovdqa %%ymm0, 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 3*32(%%rsi) \n"
        "clflushopt 2*32(%%rsi) \n"
        "vmovdqa %%ymm0, 4*32(%%rsi) \n"
        "vmovdqa %%ymm0, 5*32(%%rsi) \n"
        "clflushopt 4*32(%%rsi) \n"
        "vmovdqa %%ymm0, 6*32(%%rsi) \n"
        "vmovdqa %%ymm0, 7*32(%%rsi) \n"
        "clflushopt 6*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t nstore_256byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_VALUE
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovntpd %%ymm0, 0*32(%%rsi) \n"
        "vmovntpd %%ymm0, 1*32(%%rsi) \n"
        "vmovntpd %%ymm0, 2*32(%%rsi) \n"
        "vmovntpd %%ymm0, 3*32(%%rsi) \n"
        "vmovntpd %%ymm0, 4*32(%%rsi) \n"
        "vmovntpd %%ymm0, 5*32(%%rsi) \n"
        "vmovntpd %%ymm0, 6*32(%%rsi) \n"
        "vmovntpd %%ymm0, 7*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_256byte_fence(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "vmovdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovdqa 1*32(%%rsi), %%ymm1 \n"
        "vmovdqa 2*32(%%rsi), %%ymm1 \n"
        "vmovdqa 3*32(%%rsi), %%ymm1 \n"
        "vmovdqa 4*32(%%rsi), %%ymm1 \n"
        "vmovdqa 5*32(%%rsi), %%ymm1 \n"
        "vmovdqa 6*32(%%rsi), %%ymm1 \n"
        "vmovdqa 7*32(%%rsi), %%ymm1 \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_256byte_fence_nt(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
		 // vmovntdqa 
        "vmovntdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovntdqa 1*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 2*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 3*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 4*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 5*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 6*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 7*32(%%rsi), %%ymm1 \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_256byte_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "movq %%rdx, 16*8(%%rsi) \n"
        "movq %%rdx, 17*8(%%rsi) \n"
        "movq %%rdx, 18*8(%%rsi) \n"
        "movq %%rdx, 19*8(%%rsi) \n"
        "movq %%rdx, 20*8(%%rsi) \n"
        "movq %%rdx, 21*8(%%rsi) \n"
        "movq %%rdx, 22*8(%%rsi) \n"
        "movq %%rdx, 23*8(%%rsi) \n"
        "movq %%rdx, 24*8(%%rsi) \n"
        "movq %%rdx, 25*8(%%rsi) \n"
        "movq %%rdx, 26*8(%%rsi) \n"
        "movq %%rdx, 27*8(%%rsi) \n"
        "movq %%rdx, 28*8(%%rsi) \n"
        "movq %%rdx, 29*8(%%rsi) \n"
        "movq %%rdx, 30*8(%%rsi) \n"
        "movq %%rdx, 31*8(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t store_256byte_clflush_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        LOAD_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq %%rdx, 0*8(%%rsi) \n"
        "movq %%rdx, 1*8(%%rsi) \n"
        "movq %%rdx, 2*8(%%rsi) \n"
        "movq %%rdx, 3*8(%%rsi) \n"
        "movq %%rdx, 4*8(%%rsi) \n"
        "movq %%rdx, 5*8(%%rsi) \n"
        "movq %%rdx, 6*8(%%rsi) \n"
        "movq %%rdx, 7*8(%%rsi) \n"
        "clflush 0*8(%%rsi) \n"
        "movq %%rdx, 8*8(%%rsi) \n"
        "movq %%rdx, 9*8(%%rsi) \n"
        "movq %%rdx, 10*8(%%rsi) \n"
        "movq %%rdx, 11*8(%%rsi) \n"
        "movq %%rdx, 12*8(%%rsi) \n"
        "movq %%rdx, 13*8(%%rsi) \n"
        "movq %%rdx, 14*8(%%rsi) \n"
        "movq %%rdx, 15*8(%%rsi) \n"
        "clflush 8*8(%%rsi) \n"
        "movq %%rdx, 16*8(%%rsi) \n"
        "movq %%rdx, 17*8(%%rsi) \n"
        "movq %%rdx, 18*8(%%rsi) \n"
        "movq %%rdx, 19*8(%%rsi) \n"
        "movq %%rdx, 20*8(%%rsi) \n"
        "movq %%rdx, 21*8(%%rsi) \n"
        "movq %%rdx, 22*8(%%rsi) \n"
        "movq %%rdx, 23*8(%%rsi) \n"
        "clflush 16*8(%%rsi) \n"
        "movq %%rdx, 24*8(%%rsi) \n"
        "movq %%rdx, 25*8(%%rsi) \n"
        "movq %%rdx, 26*8(%%rsi) \n"
        "movq %%rdx, 27*8(%%rsi) \n"
        "movq %%rdx, 28*8(%%rsi) \n"
        "movq %%rdx, 29*8(%%rsi) \n"
        "movq %%rdx, 30*8(%%rsi) \n"
        "movq %%rdx, 31*8(%%rsi) \n"
        "clflush 24*8(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t load_256byte_fence_movq(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    KERNEL_BEGIN
    asm volatile(LOAD_ADDR
        FLUSH_CACHE_LINE
        CLEAR_PIPELINE
        TIMING_BEG
        "movq 0*8(%%rsi), %%rdx \n"
        "movq 1*8(%%rsi), %%rdx \n"
        "movq 2*8(%%rsi), %%rdx \n"
        "movq 3*8(%%rsi), %%rdx \n"
        "movq 4*8(%%rsi), %%rdx \n"
        "movq 5*8(%%rsi), %%rdx \n"
        "movq 6*8(%%rsi), %%rdx \n"
        "movq 7*8(%%rsi), %%rdx \n"
        "movq 8*8(%%rsi), %%rdx \n"
        "movq 9*8(%%rsi), %%rdx \n"
        "movq 10*8(%%rsi), %%rdx \n"
        "movq 11*8(%%rsi), %%rdx \n"
        "movq 12*8(%%rsi), %%rdx \n"
        "movq 13*8(%%rsi), %%rdx \n"
        "movq 14*8(%%rsi), %%rdx \n"
        "movq 15*8(%%rsi), %%rdx \n"
        "movq 16*8(%%rsi), %%rdx \n"
        "movq 17*8(%%rsi), %%rdx \n"
        "movq 18*8(%%rsi), %%rdx \n"
        "movq 19*8(%%rsi), %%rdx \n"
        "movq 20*8(%%rsi), %%rdx \n"
        "movq 21*8(%%rsi), %%rdx \n"
        "movq 22*8(%%rsi), %%rdx \n"
        "movq 23*8(%%rsi), %%rdx \n"
        "movq 24*8(%%rsi), %%rdx \n"
        "movq 25*8(%%rsi), %%rdx \n"
        "movq 26*8(%%rsi), %%rdx \n"
        "movq 27*8(%%rsi), %%rdx \n"
        "movq 28*8(%%rsi), %%rdx \n"
        "movq 29*8(%%rsi), %%rdx \n"
        "movq 30*8(%%rsi), %%rdx \n"
        "movq 31*8(%%rsi), %%rdx \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr)
        : REGISTERS_NONSSE);
    KERNEL_END
    return t2 - t1;
}

uint64_t(*latency_funcs_256byte[])(char *) = {
    &load_256byte_fence,
    &load_256byte_fence_nt,
    &store_256byte_fence,
    &store_256byte_clflush,
    &store_256byte_clwb,
    &store_256byte_clflushopt,
    &nstore_256byte_fence,
    &store_256byte_fence_movq,
    &store_256byte_clflush_movq,
    &load_256byte_fence_movq,
    &baseline
};

// Benchmark functions map
const char *latency_bench_map[] = {
    "load-fence",
    "ntload-fence",
    "store-fence",
    "store-clflush",
    "store-clwb",
    "store-clflushopt",
    "nstore-fence",
    "store-fence-movq",
    "store-clflush-movq",
    "load-fence-movq",
    "store-movq-baseline",
    "load-movq-baseline",
    "store-baseline",
    "load-baseline",
    "baseline"
};

// Kernel-level task lists

enum latency_tasks {
    load_fence_64 = 0,
    ntload_fence_64,
    store_fence_64,
    store_clflush_64,
#ifdef AEP_SUPPORTED
    store_clwb_64,
    store_clflushopt_64,
#endif
    nstore_fence_64,
    store_fence_movq_64,
    store_clflush_movq_64,
    load_fence_movq_64,

    load_nofence_64,
    ntload_nofence_64,
    load_nofence_movq_64,

    store_nofence_64,
    store_clflush_fence_64,
#ifdef AEP_SUPPORTED
    store_clwb_fence_64,
    store_clflushopt_fence_64,
#endif
    nstore_nofence_64,
    store_nofence_movq_64,
    store_clflush_fence_movq_64,
    store_clwb_movq_64,
    store_clwb_fence_movq_64,
    store_clflushopt_movq_64,
    store_clflushopt_fence_movq_64,

    store_fence_movb_64,
    store_nofence_movb_64,
    store_clflush_movb_64,
    store_clflush_fence_movb_64,
    store_clwb_movb_64,
    store_clwb_fence_movb_64,
    store_clflushopt_movb_64,
    store_clflushopt_fence_movb_64,
    load_fence_movb_64,
    load_nofence_movb_64,

    store_fence_movd_64,
    store_nofence_movd_64,
    store_clflush_movd_64,
    store_clflush_fence_movd_64,
    store_clwb_movd_64,
    store_clwb_fence_movd_64,
    store_clflushopt_movd_64,
    store_clflushopt_fence_movd_64,
    load_fence_movd_64,
    load_nofence_movd_64,

    load_fence_128,
    ntload_fence_128,
    store_fence_128,
    store_clflush_128,
#ifdef AEP_SUPPORTED
    store_clwb_128,
    store_clflushopt_128,
#endif
    nstore_fence_128,
    store_fence_movq_128,
    store_clflush_movq_128,
    load_fence_movq_128,

    load_fence_256,
    ntload_fence_256,
    store_fence_256,
    store_clflush_256,
#ifdef AEP_SUPPORTED
    store_clwb_256,
    store_clflushopt_256,
#endif
    nstore_fence_256,
    store_fence_movq_256,
    store_clflush_movq_256,
    load_fence_movq_256,
    store_nofence_movq_baseline,
    load_nofence_movq_baseline,
    store_nofence_baseline,
    load_nofence_baseline,
    task_baseline,

    BASIC_OPS_TASK_COUNT
};

// Bandwidth task list (random, sequential)

enum bandwidth_rs_tasks {
    bw_r_load_fence_64 = 0,
    bw_r_ntload_fence_64,
    bw_r_store_fence_64,
    bw_r_store_clflush_64,
#ifdef AEP_SUPPORTED
    bw_r_store_clwb_64,
    bw_r_store_clflushopt_64,
#endif
    bw_r_nstore_fence_64,
    bw_r_store_fence_movq_64,
    bw_r_store_clflush_movq_64,
    bw_r_load_fence_movq_64,

    bw_r_load_nofence_64,
    bw_r_ntload_nofence_64,
    bw_r_load_nofence_movq_64,

    bw_r_store_nofence_64,
    bw_r_store_clflush_fence_64,
#ifdef AEP_SUPPORTED
    bw_r_store_clwb_fence_64,
    bw_r_store_clflushopt_fence_64,
#endif
    bw_r_nstore_nofence_64,
    bw_r_store_nofence_movq_64,
    bw_r_store_clflush_fence_movq_64,
    bw_r_store_clwb_movq_64,
    bw_r_store_clwb_fence_movq_64,
    bw_r_store_clflushopt_movq_64,
    bw_r_store_clflushopt_fence_movq_64,

    bw_r_store_fence_movb_64,
    bw_r_store_nofence_movb_64,
    bw_r_store_clflush_movb_64,
    bw_r_store_clflush_fence_movb_64,
    bw_r_store_clwb_movb_64,
    bw_r_store_clwb_fence_movb_64,
    bw_r_store_clflushopt_movb_64,
    bw_r_store_clflushopt_fence_movb_64,
    bw_r_load_fence_movb_64,
    bw_r_load_nofence_movb_64,

    bw_r_store_fence_movd_64,
    bw_r_store_nofence_movd_64,
    bw_r_store_clflush_movd_64,
    bw_r_store_clflush_fence_movd_64,
    bw_r_store_clwb_movd_64,
    bw_r_store_clwb_fence_movd_64,
    bw_r_store_clflushopt_movd_64,
    bw_r_store_clflushopt_fence_movd_64,
    bw_r_load_fence_movd_64,
    bw_r_load_nofence_movd_64,

    bw_s_load_fence_64,
    bw_s_ntload_fence_64,
    bw_s_store_fence_64,
    bw_s_store_clflush_64,
#ifdef AEP_SUPPORTED
    bw_s_store_clwb_64,
    bw_s_store_clflushopt_64,
#endif
    bw_s_nstore_fence_64,
    bw_s_store_fence_movq_64,
    bw_s_store_clflush_movq_64,
    //
    bw_s_load_fence_movq_64,
    bw_s_load_nofence_64,
    bw_s_ntload_nofence_64,
    bw_s_load_nofence_movq_64,

    bw_s_store_nofence_64,
    bw_s_store_clflush_fence_64,
#ifdef AEP_SUPPORTED
    bw_s_store_clwb_fence_64,
    bw_s_store_clflushopt_fence_64,
#endif
    bw_s_nstore_nofence_64,
    bw_s_store_nofence_movq_64,
    bw_s_store_clflush_fence_movq_64,
    bw_s_store_clwb_movq_64,
    bw_s_store_clwb_fence_movq_64,
    bw_s_store_clflushopt_movq_64,
    bw_s_store_clflushopt_fence_movq_64,

    bw_s_store_fence_movb_64,
    bw_s_store_nofence_movb_64,
    bw_s_store_clflush_movb_64,
    bw_s_store_clflush_fence_movb_64,
    bw_s_store_clwb_movb_64,
    bw_s_store_clwb_fence_movb_64,
    bw_s_store_clflushopt_movb_64,
    bw_s_store_clflushopt_fence_movb_64,
    //
    bw_s_load_fence_movb_64,
    bw_s_load_nofence_movb_64,
    //
    bw_s_store_fence_movd_64,
    bw_s_store_nofence_movd_64,
    bw_s_store_clflush_movd_64,
    bw_s_store_clflush_fence_movd_64,
    bw_s_store_clwb_movd_64,
    bw_s_store_clwb_fence_movd_64,
    bw_s_store_clflushopt_movd_64,
    bw_s_store_clflushopt_fence_movd_64,
    bw_s_load_fence_movd_64,
    bw_s_load_nofence_movd_64,

    bw_s_task_baseline,

    BW_RS_BASIC_OPS_TASK_COUNT
};


static const int latency_tasks_skip[BASIC_OPS_TASK_COUNT] = {
64,
64,
64,
64,
#ifdef AEP_SUPPORTED
64,
64,
#endif
64,
64,
64,
64,

64,
64,
64,
64,
64,
#ifdef AEP_SUPPORTED
64,
64,
#endif
64,
64,
64,
64,
64,
64,
64, // store_clflushopt_fence_movq_64
64,
64,
64,
64,
64,
64,
64,
64,
64,
64, // load_nofence_movb_64
64,
64,
64,
64,
64,
64,
64,
64,
64,
64, // load_nofence_movd_64

128,
128,
128,
128,
#ifdef AEP_SUPPORTED
128,
128,
#endif
128,
128,
128,
128,

256,
256,
256,
256,
#ifdef AEP_SUPPORTED
256,
256,
#endif
256,
256,
256,
256,

64,
64,
64,
64,
0
};

static const char *latency_tasks_str[BASIC_OPS_TASK_COUNT] = {
    "load-fence-64",
    "ntload-fence-64",
    "store-fence-64",
    "store-clflush-64",
#ifdef AEP_SUPPORTED
    "store-clwb-64",
    "store-clflushopt-64",
#endif
    "nstore-fence-64",
    "store-fence-movq-64",
    "store-clflush-movq-64",
    "load-fence-movq-64",

    "load-nofence-64",
    "ntload-nofence-64",
    "load-nofence-movq-64",

    "store-nofence-64",
    "store-clflush-fence-64",
#ifdef AEP_SUPPORTED
    "store-clwb-fence-64",
    "store-clflushopt-fence-64",
#endif
    "nstore-nofence-64",
    "store-nofence-movq-64",
    "store-clflush-fence-movq-64",
    "store-clwb-movq-64",
    "store-clwb-fence-movq-64",
    "store-clflushopt-movq-64",
    "store-clflushopt-fence-movq-64",

    "store-fence-movb-64",
    "store-nofence-movb-64",
    "store-clflush-movb-64",
    "store-clflush-fence-movb-64",
    "store-clwb-movb-64",
    "store-clwb-fence-movb-64",
    "store-clflushopt-movb-64",
    "store-clflushopt-fence-movb-64",
    "load-fence-movb-64",
    "load-nofence-movb-64",

    "store-fence-movd-64",
    "store-nofence-movd-64",
    "store-clflush-movd-64",
    "store-clflush-fence-movd-64",
    "store-clwb-movd-64",
    "store-clwb-fence-movd-64",
    "store-clflushopt-movd-64",
    "store-clflushopt-fence-movd-64",
    "load-fence-movd-64",
    "load-nofence-movd-64",

    "load-fence-128",
    "ntload-fence-128",
    "store-fence-128",
    "store-clflush-128",
#ifdef AEP_SUPPORTED
    "store-clwb-128",
    "store-clflushopt-128",
#endif
    "nstore-fence-128",
    "store-fence-movq-128",
    "store-clflush-movq-128",
    "load-fence-movq-128",

    "load-fence-256",
    "ntload-fence-256",
    "store-fence-256",
    "store-clflush-256",
#ifdef AEP_SUPPORTED
    "store-clwb-256",
    "store-clflushopt-256",
#endif
    "nstore-fence-256",
    "store-fence-movq-256",
    "store-clflush-movq-256",
    "load-fence-movq-256",

    "store-nofence-movq-64",
    "load-nofence-movq-64",
    "store-nofence-64",
    "load-nofence-64",
    "baseline"
};


static const char *bandwidth_rs_tasks_str[BW_RS_BASIC_OPS_TASK_COUNT] = {
    "bw_r_load-fence-64",
    "bw_r_ntload-fence-64",
    "bw_r_store-fence-64",
    "bw_r_store-clflush-64",
#ifdef AEP_SUPPORTED
    "bw_r_store-clwb-64",
    "bw_r_store-clflushopt-64",
#endif
    "bw_r_nstore-fence-64",
    "bw_r_store-fence-movq-64",
    "bw_r_store-clflush-movq-64",
    "bw_r_load-fence-movq-64",

    "bw_r_load-nofence-64",
    "bw_r_ntload-nofence-64",
    "bw_r_load-nofence-movq-64",

    "bw_r_store-nofence-64",
    "bw_r_store-clflush-fence-64",
#ifdef AEP_SUPPORTED
    "bw_r_store-clwb-fence-64",
    "bw_r_store-clflushopt-fence-64",
#endif
    "bw_r_nstore-nofence-64",
    "bw_r_store-nofence-movq-64",
    "bw_r_store-clflush-fence-movq-64",
    "bw_r_store-clwb-movq-64",
    "bw_r_store-clwb-fence-movq-64",
    "bw_r_store-clflushopt-movq-64",
    "bw_r_store-clflushopt-fence-movq-64",

    "bw_r_store-fence-movb-64",
    "bw_r_store-nofence-movb-64",
    "bw_r_store-clflush-movb-64",
    "bw_r_store-clflush-fence-movb-64",
    "bw_r_store-clwb-movb-64",
    "bw_r_store-clwb-fence-movb-64",
    "bw_r_store-clflushopt-movb-64",
    "bw_r_store-clflushopt-fence-movb-64",
    "bw_r_load-fence-movb-64",
    "bw_r_load-nofence-movb-64",

    "bw_r_store-fence-movd-64",
    "bw_r_store-nofence-movd-64",
    "bw_r_store-clflush-movd-64",
    "bw_r_store-clflush-fence-movd-64",
    "bw_r_store-clwb-movd-64",
    "bw_r_store-clwb-fence-movd-64",
    "bw_r_store-clflushopt-movd-64",
    "bw_r_store-clflushopt-fence-movd-64",
    "bw_r_load-fence-movd-64",
    "bw_r_load-nofence-movd-64",

    "bw_s_load-fence-64",
    "bw_s_ntload-fence-64",
    "bw_s_store-fence-64",
    "bw_s_store-clflush-64",
#ifdef AEP_SUPPORTED
    "bw_s_store-clwb-64",
    "bw_s_store-clflushopt-64",
#endif
    "bw_s_nstore-fence-64",
    "bw_s_store-fence-movq-64",
    "bw_s_store-clflush-movq-64",
    //
    "bw_s_load-fence-movq-64",
    "bw_s_load-nofence-64",
    //
    "bw_s_ntload-nofence-64",
    "bw_s_load-nofence-movq-64",
    //

    "bw_s_store-nofence-64",
    "bw_s_store-clflush-fence-64",
#ifdef AEP_SUPPORTED
    "bw_s_store-clwb-fence-64",
    "bw_s_store-clflushopt-fence-64",
#endif
    "bw_s_nstore-nofence-64",
    "bw_s_store-nofence-movq-64",
    "bw_s_store-clflush-fence-movq-64",
    "bw_s_store-clwb-movq-64",
    "bw_s_store-clwb-fence-movq-64",
    "bw_s_store-clflushopt-movq-64",
    "bw_s_store-clflushopt-fence-movq-64",
    "bw_s_store-fence-movb-64",
    "bw_s_store-nofence-movb-64",
    "bw_s_store-clflush-movb-64",
    "bw_s_store-clflush-fence-movb-64",
    "bw_s_store-clwb-movb-64",
    "bw_s_store-clwb-fence-movb-64",
    "bw_s_store-clflushopt-movb-64",
    "bw_s_store-clflushopt-fence-movb-64",
    "bw_s_load-fence-movb-64",
    "bw_s_load-nofence-movb-64",
    //
    "bw_s_store-fence-movd-64",
    "bw_s_store-nofence-movd-64",
    "bw_s_store-clflush-movd-64",
    "bw_s_store-clflush-fence-movd-64",
    "bw_s_store-clwb-movd-64",
    "bw_s_store-clwb-fence-movd-64",
    "bw_s_store-clflushopt-movd-64",
    "bw_s_store-clflushopt-fence-movd-64",
    "bw_s_load-fence-movd-64",
    "bw_s_load-nofence-movd-64",

    "bw_s_task_baseline"
};

uint64_t (*bench_func[BASIC_OPS_TASK_COUNT])(char *) = {
    &load_64byte_fence,
    &load_64byte_fence_nt,
    &store_64byte_fence,
    &store_64byte_clflush,
#ifdef AEP_SUPPORTED
    &store_64byte_clwb,
    &store_64byte_clflushopt,
#endif
    &nstore_64byte_fence,
    &store_64byte_fence_movq,
    &store_64byte_clflush_movq,
    &load_64byte_fence_movq,

    &load_64byte_nofence,
    &load_64byte_nofence_nt,
    &load_64byte_nofence_movq,
    &store_64byte_nofence,
    &store_64byte_clflush_fence,
#ifdef AEP_SUPPORTED
    &store_64byte_clwb_fence,
    &store_64byte_clflushopt_fence,
#endif
    &nstore_64byte_nofence,
    &store_64byte_nofence_movq,
    &store_64byte_clflush_fence_movq,
    &store_64byte_clwb_movq,
    &store_64byte_clwb_fence_movq,
    &store_64byte_clflushopt_movq,
    &store_64byte_clflushopt_fence_movq,

    &store_64byte_fence_movb,
    &store_64byte_nofence_movb,
    &store_64byte_clflush_movb,
    &store_64byte_clflush_fence_movb,
    &store_64byte_clwb_movb,
    &store_64byte_clwb_fence_movb,
    &store_64byte_clflushopt_movb,
    &store_64byte_clflushopt_fence_movb,
    &load_64byte_fence_movb,
    &load_64byte_nofence_movb,

    &store_64byte_fence_movd,
    &store_64byte_nofence_movd,
    &store_64byte_clflush_movd,
    &store_64byte_clflush_fence_movd,
    &store_64byte_clwb_movd,
    &store_64byte_clwb_fence_movd,
    &store_64byte_clflushopt_movd,
    &store_64byte_clflushopt_fence_movd,
    &load_64byte_fence_movd,
    &load_64byte_nofence_movd,

    &load_128byte_fence,
    &load_128byte_fence_nt,
    &store_128byte_fence,
    &store_128byte_clflush,
#ifdef AEP_SUPPORTED
    &store_128byte_clwb,
    &store_128byte_clflushopt,
#endif
    &nstore_128byte_fence,
    &store_128byte_fence_movq,
    &store_128byte_clflush_movq,
    &load_128byte_fence_movq,


    &load_256byte_fence,
    &load_256byte_fence_nt,
    &store_256byte_fence,
    &store_256byte_clflush,
#ifdef AEP_SUPPORTED
    &store_256byte_clwb,
    &store_256byte_clflushopt,
#endif
    &nstore_256byte_fence,
    &store_256byte_fence_movq,
    &store_256byte_clflush_movq,
    &load_256byte_fence_movq,

    &store_64byte_nofence_movq_baseline,
    &load_64byte_nofence_movq_baseline,
    &store_64byte_nofence_baseline,
    &load_64byte_nofence_baseline,
    &baseline
};

// bandwidth benchmarks random and sequential
uint64_t (*bw_rs_bench_func[BW_RS_BASIC_OPS_TASK_COUNT])(char *, long) = {
    &bw_r_load_64byte_fence,
    &bw_r_load_64byte_fence_nt,
    &bw_r_store_64byte_fence,
    &bw_r_store_64byte_clflush,
#ifdef AEP_SUPPORTED
    &bw_r_store_64byte_clwb,
    &bw_r_store_64byte_clflushopt,
#endif
    &bw_r_nstore_64byte_fence,
    &bw_r_store_64byte_fence_movq,
    &bw_r_store_64byte_clflush_movq,
    &bw_r_load_64byte_fence_movq,

    &bw_r_load_64byte_nofence,
    &bw_r_load_64byte_nofence_nt,
    &bw_r_load_64byte_nofence_movq,

    &bw_r_store_64byte_nofence,
    &bw_r_store_64byte_clflush_fence,
#ifdef AEP_SUPPORTED
    &bw_r_store_64byte_clwb_fence,
    &bw_r_store_64byte_clflushopt_fence,
#endif
    &bw_r_nstore_64byte_nofence,
    &bw_r_store_64byte_nofence_movq,
    &bw_r_store_64byte_clflush_fence_movq,
    &bw_r_store_64byte_clwb_movq,
    &bw_r_store_64byte_clwb_fence_movq,
    &bw_r_store_64byte_clflushopt_movq,
    &bw_r_store_64byte_clflushopt_fence_movq,

    &bw_r_store_64byte_fence_movb,
    &bw_r_store_64byte_nofence_movb,
    &bw_r_store_64byte_clflush_movb,
    &bw_r_store_64byte_clflush_fence_movb,
    &bw_r_store_64byte_clwb_movb,
    &bw_r_store_64byte_clwb_fence_movb,
    &bw_r_store_64byte_clflushopt_movb,
    &bw_r_store_64byte_clflushopt_fence_movb,
    &bw_r_load_64byte_fence_movb,
    &bw_r_load_64byte_nofence_movb,

    &bw_r_store_64byte_fence_movd,
    &bw_r_store_64byte_nofence_movd,
    &bw_r_store_64byte_clflush_movd,
    &bw_r_store_64byte_clflush_fence_movd,
    &bw_r_store_64byte_clwb_movd,
    &bw_r_store_64byte_clwb_fence_movd,
    &bw_r_store_64byte_clflushopt_movd,
    &bw_r_store_64byte_clflushopt_fence_movd,
    &bw_r_load_64byte_fence_movd,
    &bw_r_load_64byte_nofence_movd,

    &bw_s_load_64byte_fence,
    &bw_s_load_64byte_fence_nt,

    &bw_s_store_64byte_fence,
    &bw_s_store_64byte_clflush,
#ifdef AEP_SUPPORTED
    &bw_s_store_64byte_clwb,
    &bw_s_store_64byte_clflushopt,
#endif
    &bw_s_nstore_64byte_fence,
    &bw_s_store_64byte_fence_movq,
    &bw_s_store_64byte_clflush_movq,
    &bw_s_load_64byte_fence_movq,
    // add
    &bw_s_load_64byte_nofence,
    &bw_s_load_64byte_nofence_nt,
    &bw_s_load_64byte_nofence_movq,
    &bw_s_store_64byte_nofence,

    &bw_s_store_64byte_clflush_fence,
#ifdef AEP_SUPPORTED
    &bw_s_store_64byte_clwb_fence,
    &bw_s_store_64byte_clflushopt_fence,
#endif
    &bw_s_nstore_64byte_nofence,
    &bw_s_store_64byte_nofence_movq,
    &bw_s_store_64byte_clflush_fence_movq,
    &bw_s_store_64byte_clwb_movq,
    &bw_s_store_64byte_clwb_fence_movq,
    &bw_s_store_64byte_clflushopt_movq,
    &bw_s_store_64byte_clflushopt_fence_movq,

    &bw_s_store_64byte_fence_movb,
    &bw_s_store_64byte_nofence_movb,
    &bw_s_store_64byte_clflush_movb,
    &bw_s_store_64byte_clflush_fence_movb,
    &bw_s_store_64byte_clwb_movb,
    &bw_s_store_64byte_clwb_fence_movb,
    &bw_s_store_64byte_clflushopt_movb,
    &bw_s_store_64byte_clflushopt_fence_movb,
    &bw_s_load_64byte_fence_movb,
    &bw_s_load_64byte_nofence_movb,

    &bw_s_store_64byte_fence_movd,
    &bw_s_store_64byte_nofence_movd,
    &bw_s_store_64byte_clflush_movd,
    &bw_s_store_64byte_clflush_fence_movd,
    &bw_s_store_64byte_clwb_movd,
    &bw_s_store_64byte_clwb_fence_movd,
    &bw_s_store_64byte_clflushopt_movd,
    &bw_s_store_64byte_clflushopt_fence_movd,
    &bw_s_load_64byte_fence_movd,
    &bw_s_load_64byte_nofence_movd,

    &bw_s_baseline
};

/*
 * 256-byte benchmarks
 */
uint64_t repeat_256byte_ntstore(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_VALUE
        LOAD_ADDR
        TIMING_BEG
        "vmovntpd %%ymm0, 0*32(%%rsi) \n"
        "vmovntpd %%ymm0, 1*32(%%rsi) \n"
        "vmovntpd %%ymm0, 2*32(%%rsi) \n"
        "vmovntpd %%ymm0, 3*32(%%rsi) \n"
        "vmovntpd %%ymm0, 4*32(%%rsi) \n"
        "vmovntpd %%ymm0, 5*32(%%rsi) \n"
        "vmovntpd %%ymm0, 6*32(%%rsi) \n"
        "vmovntpd %%ymm0, 7*32(%%rsi) \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

/*
 * 256-byte benchmarks
 */
uint64_t repeat_256byte_load(char *addr) {
    uint64_t t1 = 0, t2 = 0;
    uint64_t value = 0xC0FFEEEEBABE0000;
    KERNEL_BEGIN
    asm volatile(LOAD_VALUE
        LOAD_ADDR
        TIMING_BEG
        "vmovntdqa 0*32(%%rsi), %%ymm0 \n"
        "vmovntdqa 1*32(%%rsi), %%ymm1 \n"
        "vmovntdqa 2*32(%%rsi), %%ymm2 \n"
        "vmovntdqa 3*32(%%rsi), %%ymm3 \n"
        "vmovntdqa 4*32(%%rsi), %%ymm4 \n"
        "vmovntdqa 5*32(%%rsi), %%ymm5 \n"
        "vmovntdqa 6*32(%%rsi), %%ymm6 \n"
        "vmovntdqa 7*32(%%rsi), %%ymm7 \n"
        TIMING_END
        : [t1] "=r" (t1), [t2] "=r" (t2)
        : [memarea] "r" (addr), [value] "m" (value)
        : REGISTERS);
    KERNEL_END
    return t2 - t1;
}

#endif // LATTESTER_COMMON_H
