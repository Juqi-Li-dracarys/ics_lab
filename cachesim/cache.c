#include "common.h"
#include <inttypes.h>
#include <string.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

// cache 每行对应一个 block
typedef struct cache {
  uint8_t data[1 << BLOCK_WIDTH];
  bool valid;
} cache_row;

// cache 总行数
static size_t cache_row_num = 0;

// cache mem ptr
static cache_row *cache_mem = NULL;

// cache 总组数
static uint32_t cache_group_num = 0;

// cache 组相连数
static uint32_t associativity_num = 0;


void cycle_increase(int n) { cycle_cnt += n; }


// TODO: implement the following functions


// 从 cache 中读出 addr 地址处的 4 字节数据
// 若缺失，需要先从内存中读入数据
uint32_t cache_read(uintptr_t addr) {
  return 0;
}

// 往 cache 中 addr 地址所属的块写入数据 data，写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低8比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  return;
}

// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {
    associativity_num = 1 << associativity_width;
    cache_group_num = 1 << (total_size_width - BLOCK_WIDTH);
    cache_row_num = (size_t)0x1 << (total_size_width + associativity_width - BLOCK_WIDTH);
    cache_mem = (cache_row *)realloc((void *)cache_mem, sizeof(cache_row) * cache_row_num);
    memset(cache_mem, 0, sizeof(cache_row) * cache_row_num);
    return;
}


void display_statistic(void) {
}
