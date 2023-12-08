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
  if(n < 2) return (int *)primes;
  register uint64_t i = 3;
  register uint64_t j = 0;
  register uint32_t *p = primes;
  register uint32_t limit = (uint32_t)n;
  for(; i * i <= limit; i = i + 2) {
    // this number is a prime
    if(is_prime[i] == false) {
      for(j = i * i; j <= limit; j = j + 2 * i) {
        is_prime[j] = true;
      }
    }
  }
  // record all primes
  *p++ = 2;
  for(i = 3; i <= limit; i = i + 2) {
    if(!is_prime[i]) {
      *p++ = i;
    }
  }
  return (int *)primes;
}




