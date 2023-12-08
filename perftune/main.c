#include <stdint.h>
#include <stdio.h>

int *sieve(int n);

int main() {
  int *primes = sieve(10);
  int count  = 0;
  for (int *cur = primes, i = 0; *cur; cur++, i++) {
    printf("%8d", *cur);
    if (i % 8 == 7 || !*(cur + 1)) printf("\n");
    count++;
  }
  printf("count: %d\n", count);
  unsigned long x = 92683*92683;
  printf("%ld\n", x);
}
