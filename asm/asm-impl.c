
#include "asm.h"

// GDB：set disassemble-next-line on

int64_t asm_add(int64_t a, int64_t b) {
  int64_t result;
  asm (
    "addq %2, %1\n\t"
    "movq %1, %0"
    : "=m"(result)
    : "rm"(a), "rm"(b)
  );
  return result;
}

int asm_popcnt(uint64_t x) {
  uint32_t result = 0;
  uint32_t i = 0;
  asm (
    "movl $0, %0 \n\t"
    "movl $0, %1 \n\t"
    "jmp .L1\n\t"
    ".L2:\n\t"
    "movq	%2, %%rax\n\t"
    "andq	$1, %%rax\n\t"
    "testq %%rax, %%rax\n\t"
    "je	.L3\n\t"
    "addl	$1, %0\n\t"
    ".L3:\n\t"
    "addl	$1, %1\n\t"
    "shrq	$1, %2\n\t"
    ".L1:\n\t"
    "cmpl	$63, %1\n\t"
    "jle .L2"
    : "=m"(result)
    : "rm"(i), "rm"(x)
    : "%rax"
  );
  return result;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  asm (
    "movq $0, %%rax\n\t"
    ".L0:\n\t"
    "movb (%1, %%rax, 1), %%bl\n\t"
    "movb %%bl, (%0, %%rax, 1)\n\t"
    "addq $1, %%rax\n\t"
    "cmpq %%rax, %2\n\t"
    "ja .L0\n\t"
    :
    : "rm"(dest), "rm"(src), "rm"(n)
    : "%rax", "%bl"
  );
  return dest;
}

  /*
  ****************************** 
  BULLSHIT!
  ******************************
  注意，这里的函数调用和书上讲的不一样
  不是常规的压栈出栈操作，正确解释如下：
  1. 在原函数（main）中移动栈指针 rsp, 
  为输入参数开辟 5*8 的栈空间
  2. call 指令向栈压入一个地址，该地址的
  的值为下一条指令的 PC 值，压栈 rsp 自动
  +8
  3.函数结束执行 ret, 该指令自动出栈取出
  下一条指令的地址，并使得 PC = *rsp, 
  出栈 rsp 自动 -8.
  至此，全部步骤结束，栈空间的释放或者其他步骤
  在原函数中进行！
  */

static asm_jmp_buf data_buf;
static int temp_data = 0;

int asm_setjmp(asm_jmp_buf env) {
  asm (
    "leaq %0, %%rax\n\t"
    "movq %%rbx, 0x0(%%rax)\n\t"
    "movq %%rsi, 0x8(%%rax)\n\t"
    "movq %%rdi, 0x10(%%rax)\n\t"
    "movq %%rbp, 0x18(%%rax)\n\t"
    "movq %%rsp, 0x20(%%rax)\n\t"
    "movq 0x0(%%rsp), %%rcx \n\t"
    "movq %%rcx, 0x28(%%rax)\n\t"
    :
    : "m"(data_buf)
    : "%rax", "%rcx"
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  temp_data = val;
  asm (
    "leaq %0, %%rax\n\t"
    "movq 0x0(%%rax), %%rbx\n\t"
    "movq 0x8(%%rax), %%rsi\n\t"
    "movq 0x10(%%rax), %%rdi\n\t"
    "movq 0x18(%%rax), %%rbp\n\t"
    "movq 0x20(%%rax), %%rsp\n\t"
    "movq 0x28(%%rax), %%rcx\n\t"
    "addq $8, %%rsp\n\t"
    "movl %1, %%eax\n\t"
    "jmp *%%rcx\n\t"
    :
    : "m"(data_buf), "m"(temp_data)
    : "%rax", "%rbx", "%rsi", "%rdi", "%rcx"
  );
}
