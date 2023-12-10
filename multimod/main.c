#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m);

void test(uint64_t a, uint64_t b, uint64_t m) {
  // 获得正确的答案
  #define U64 "%" PRIu64
  uint64_t result = multimod(a, b, m);
  uint64_t answer = 0;
  printf(U64 " * " U64 " mod " U64 " = " U64 "\n", a, b, m, result);
  char buf[100] = {0};
  sprintf(buf, "python3 -c 'print("U64"*"U64"%%"U64")'", a, b ,m);
  FILE *fp = popen(buf, "r");
  assert(fp);
  fscanf(fp, "%s", buf);
  pclose(fp);
  sscanf(buf, U64, &answer);
  printf("The correct answer: %s\n", buf);
  assert(result == answer);
}

uint64_t generate_random_uint64() {
  uint64_t random_value = 0;
  for (int i = 0; i < 4; i++) {
    // Generate a random 32-bit number and shift it into the result.
    uint16_t random_bits = (uint16_t)rand();
    random_value = (random_value << 16) + random_bits;
  }
  return random_value;
}

int main() {
  // 随机测试
  uint64_t a;
  uint64_t b;
  uint64_t c;
  srand((unsigned)time(NULL));
  for(int i = 0; i < 1000; i++) {
    printf("Test %d:\n", i);
    a = generate_random_uint64();
    b = generate_random_uint64();
    c = generate_random_uint64();
    if(c == 0) c++;
    test(a, b, c);
  }
  printf("Test Pass.\n");
  return 0;
}
