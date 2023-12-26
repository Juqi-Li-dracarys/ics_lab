
/*
 * @Author: Juqi Li @ NJU 
 * @Date: 2023-12-26 17:27:31 
 * @Last Modified by:   Juqi Li @ NJU 
 * @Last Modified time: 2023-12-26 17:27:31 
 */

#include <stdint.h>
#include <stdio.h>
#include <stdint.h>

int *sieve(int n);

int main() {
  int *primes = sieve(10000000-2);
  int count  = 0;
  for (int *cur = primes, i = 0; *cur; cur++, i++) {
    printf("%8d", *cur);
    if (i % 8 == 7 || !*(cur + 1)) printf("\n");
    count++;
  }
  printf("count: %d\n", count);
}
