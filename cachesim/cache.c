/*
 * @Author: Juqi Li @ NJU 
 * @Date: 2023-12-26 17:26:04 
 * @Last Modified by: Juqi Li @ NJU
 * @Last Modified time: 2023-12-26 20:00:14
 */

#include "common.h"
#include <inttypes.h>
#include <string.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

// cache 每行对应一个 block
// block size = 64B
typedef struct cache {
  uint8_t data[1 << BLOCK_WIDTH]; // 数据块
  bool valid;                     // 有效位
  uint32_t lable;                 // 标记
  bool dirty;                     // dirty
} cache_row;


/*
    Cache basic information
    替换策略：随机替换 + write back
*/

// cache 总行数
static size_t cache_row_num = 0;

// cache mem_ptr
static cache_row *cache_mem = NULL;

// cache 组数位宽
static uint32_t cache_group_width = 0;

// cache 组相连位宽
static uint32_t asso_width = 0;


void cycle_increase(int n) { cycle_cnt += n; }


// TODO: implement the following functions


inline uint32_t get_bits(uint32_t number, int high, int low) {
  assert(high < 32 && low >= 0);
  uint32_t mask = (1 << (high - low + 1)) - 1;
  mask <<= low; 
  return (number & mask) >> low;
}


// 从 cache 中读出 addr 地址处的 4 字节数据
// 若缺失，需要先从内存中读入数据
uint32_t cache_read(uintptr_t addr) {
  // paring the address
  assert(cache_mem);
  uintptr_t block_addr = get_bits(addr, BLOCK_WIDTH - 1, 0);
  uintptr_t group_addr = get_bits(addr, cache_group_width + BLOCK_WIDTH - 1, BLOCK_WIDTH);
  uintptr_t lable = get_bits(addr, 31, cache_group_width + BLOCK_WIDTH);
  assert(block_addr <= (1<<BLOCK_WIDTH) - 4);
  // cache 组内遍历
  if((group_addr + 1) * (1 << asso_width) > cache_row_num) {
    printf("addr:%08x block_addr:%08x  group_addr:%08x  lable:%08x\n", (uint32_t)addr, (uint32_t)block_addr, (uint32_t)group_addr, (uint32_t)lable);
    assert(0);
  }
  for(uint32_t i = group_addr * (1 << asso_width); i < (group_addr + 1) * (1 << asso_width); i++) {
    // Hit
    if(cache_mem[i].lable == lable && cache_mem[i].valid) {
      return *((uint32_t *)((cache_mem[i].data) + block_addr));
    }
  }
  // miss:先将该块更新到主存后覆写 cache 内容
  uintptr_t lucky_num = group_addr * (1 << asso_width) + rand() % (1 << asso_width);
  if(cache_mem[lucky_num].dirty) {
    mem_write(cache_mem[lucky_num].lable * cache_row_num + group_addr, cache_mem[lucky_num].data);
  }
  cache_mem[lucky_num].valid = true;
  cache_mem[lucky_num].lable = lable;
  mem_read(cache_mem[lucky_num].lable * cache_row_num + group_addr, cache_mem[lucky_num].data);
  return *((uint32_t *)((cache_mem[lucky_num].data) + block_addr));
}


// 往 cache 中 addr 地址所属的块写入数据 data，写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低 8 比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  assert(cache_mem);
  // paring the address
  uintptr_t block_addr = get_bits(addr, BLOCK_WIDTH - 1, 0);
  uintptr_t group_addr = get_bits(addr, cache_group_width + BLOCK_WIDTH - 1, BLOCK_WIDTH);
  uintptr_t lable = get_bits(addr, 31, cache_group_width + BLOCK_WIDTH);
  assert(block_addr <= (1<<BLOCK_WIDTH) - 4);
  // cache 组内遍历
  if((group_addr + 1) * (1 << asso_width) > cache_row_num) {
    printf("addr:%08x block_addr:%08x  group_addr:%08x  lable:%08x\n", (uint32_t)addr, (uint32_t)block_addr, (uint32_t)group_addr, (uint32_t)lable);
    assert(0);
  }
  for(uint32_t i = group_addr * (1 << asso_width); i < (group_addr + 1) * (1 << asso_width); i++) {
    // Hit
    if(cache_mem[i].lable == lable && cache_mem[i].valid) {
      *((uint32_t *)(cache_mem[i].data + block_addr)) = ((*((uint32_t *)(cache_mem[i].data + block_addr))) & (~wmask)) | (data & wmask);
      cache_mem[i].dirty = true;
    }
  }
  // miss:先将该块更新到主存后覆写 cache 内容
  uintptr_t lucky_num = group_addr * (1 << asso_width) + rand() % (1 << asso_width);
  if(cache_mem[lucky_num].dirty) {
    mem_write(cache_mem[lucky_num].lable * cache_row_num + group_addr, cache_mem[lucky_num].data);
  }
  cache_mem[lucky_num].valid = true;
  cache_mem[lucky_num].lable = lable;
  mem_read(cache_mem[lucky_num].lable * cache_row_num + group_addr, cache_mem[lucky_num].data);
  *((uint32_t *)(cache_mem[lucky_num].data + block_addr)) = ((*((uint32_t *)(cache_mem[lucky_num].data + block_addr))) & (~wmask)) | (data & wmask);
  mem_write(cache_mem[lucky_num].lable * cache_row_num + group_addr, cache_mem[lucky_num].data);
  cache_mem[lucky_num].dirty = false;
  return;
}

// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {
    asso_width = associativity_width;
    cache_group_width = total_size_width - BLOCK_WIDTH;
    cache_row_num = (size_t)0x1 << (total_size_width + associativity_width - BLOCK_WIDTH);
    cache_mem = (cache_row *)realloc((void *)cache_mem, sizeof(cache_row) * cache_row_num);
    assert(cache_mem);
    // reset valid
    memset(cache_mem, false, sizeof(cache_row) * cache_row_num);
    printf("cache asso_width = %d, cache_group_width = %d, cache_row_num = %d\n", asso_width, cache_group_width, (uint32_t)cache_row_num);
    return;
}


void display_statistic(void) {
  printf("Total cycle: %d\n", (uint32_t)cycle_cnt);
  return;
}
