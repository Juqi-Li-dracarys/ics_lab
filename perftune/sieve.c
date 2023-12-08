#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define N 10000000

// uint_32 is equal to 4 byte
// We know the cache L1d size: 128 KiB (4 instances)
#define cache_size 32768

// define prime is false
static bool is_prime[N] = {false};
static uint32_t primes[N] = {false};

int *sieve(int n) {
  assert(n + 1 < N);
  register uint64_t i = 2;
  register uint64_t j = 0;
  register uint32_t *p = primes;
  register uint32_t limit = (uint32_t)n;
  for(; i <= n; i++) {
    // this number is a prime
    if(is_prime[i] == false) {
      *p++ = i;
      j = i * i;
      while (j <= limit) {
        is_prime[j] = true;
        j += i;
      }
    }
  }
  *p = 0;
  return (int *)primes;
}




