#include "asm.h"
#include <assert.h>
#include <stdio.h>

asm_jmp_buf env;

void f(int n) {
  if (n >= 10) asm_longjmp(env, n); // 某个条件达成时，恢复快照
  printf("Call f(%d)\n", n);
  f(n + 1);
}

int main() {
  int r = asm_setjmp(env); // 快照
  if (r == 0) {
    f(1);
  } 
  else { // longjmp goes here
    printf("Recursion reaches %d\n", r);
  }
  return 0;
}