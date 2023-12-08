#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#define N 10000000

static bool is_prime[N] = {0};
static unsigned int primes[N] = {0};

// define prime is false
// We know the cache L1d size: 128 KiB (4 instances)
// now implement segmented sieve
unsigned int *sieve(unsigned int n) {
  assert(n + 1 < N);
  register unsigned int *p = primes;
  register unsigned int i = 2;
  register unsigned long j = 0;
  for(; i <= n; i++) {
    // this number is a prime
    if(is_prime[i] == false) {
      *p++ = i;
      j = i * i;
      while (j <= n) {
        is_prime[4] == true;
        j += i;
      }
    }
  }
  *p = 0;
  return primes;
}




