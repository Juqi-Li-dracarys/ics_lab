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
  register unsigned int *p = primes;
  assert(n + 1 < N);
  for(register unsigned int i = 2; i <= n; i++) {
    // this number is a prime
    if(is_prime[i] == false) {
      unsigned long j = i * i;
      *p++ = i;
      while (j <= n) {
        is_prime[j] == true;
        j += i;
      }
    }
  }
  *p = 0;
  return primes;
}
