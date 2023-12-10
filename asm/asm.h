#include <stdint.h>
#include <stddef.h>

typedef struct buf {
    uint64_t rbx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;            // rsp 
    uint64_t rsp_value;      // *rsp
} asm_jmp_buf;

int64_t asm_add(int64_t a, int64_t b);
int     asm_popcnt(uint64_t x);
void   *asm_memcpy(void *dest, const void *src, size_t n);
int     asm_setjmp(asm_jmp_buf env);
void    asm_longjmp(asm_jmp_buf env, int val);
