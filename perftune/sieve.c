#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#define N 10000000

static bool is_prime[N] = {0};
static int primes[N] = {0};

// define prime is false
// We know the cache L1d size: 128 KiB (4 instances)
// now implement segmented sieve
int *sieve(int n) {
  register int *p = primes;
  assert(n + 1 < N);
  for(register int i = 2; i <= n; i++) {
    // this number is a prime
    register int j = i * i;
    if(is_prime[i] == false) {
      *p++ = i;
      if(j <= n) {
        for(; j <= n; j += i) {
          // this number is not a prime
          is_prime[j] = true;
        }
      }
    }
  }
  *p = 0;
  return primes;
}
