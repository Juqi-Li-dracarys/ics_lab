#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N] = {0};
static int primes[N] = {0};


// we define prime is false

int *sieve(int n) {
  // assert(n + 1 < N);
  // is_prime[2] = true;
  // for (int i = 3; i <= n; i++)
  //   is_prime[i] = (i % 2 == 0) ? false : true;

  // for (int i = 3; i <= n / 2 + 1; i = i + 2) {
  //   for (int j = i + i; j <= n; j += i) {
  //     is_prime[j] = false;
  //   }
  // }

  // int *p = primes;
  // for (int i = 2; i <= n; i++)
  //   if (is_prime[i]) {
  //     *p++ = i;
  //   }
  // *p = 0;
  // return primes;

  register int *p = primes;
  assert(n + 1 < N);
  for(register int i = 2; i <= n; i++) {
    // this number is a prime
    if(is_prime[i] == false) {
      *p++ = i;
      for(register int j = i << 1; j <= n; j += i) {
        // this number is not a prime
        is_prime[j] = true;
      }
    }
  }
  *p = 0;
  return p;
}
