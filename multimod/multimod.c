
#include <stdint.h>

// 存放256位数, 防止移位溢出
uint8_t num_1[256] = {0};
uint8_t num_2[256] = {0};
uint8_t public_data_1[256] = {0};
uint8_t public_data_2[256] = {0};

uint8_t *add(uint8_t data_1[256], uint8_t data_2[256], uint8_t cout_flag);
uint8_t *sub(uint8_t data_1[256], uint8_t data_2[256]);
uint8_t *shl(uint8_t delta, uint8_t data_1[256]);
uint8_t *write_array(uint64_t data, uint8_t data_array[256]);
uint64_t read_array(uint8_t data_array[256]);
uint8_t *mul(uint8_t data_1[256], uint8_t data_2[256]);
uint8_t *mod(uint8_t data_1[256], uint8_t data_2[256]);
uint8_t *div(uint8_t data_1[256], uint8_t data_2[256]);
uint8_t cmp(uint8_t data_1[256], uint8_t data_2[256]);
uint64_t multimod(uint64_t a, uint64_t b, uint64_t m);

// 两个 256 位的无符号数相加, 保存到data2中
uint8_t *add(uint8_t data_1[256], uint8_t data_2[256], uint8_t cout_flag) {
  uint8_t temp[256] = {0};
  uint8_t cout = cout_flag; //进位标志位
  for(int i = 0; i < 256; i++) {
    temp[i] = data_1[i] ^ data_2[i] ^ cout;
    cout = (data_1[i] & data_2[i]) | (data_1[i] & cout) | (data_2[i] & cout);
  }
  for(int i = 0; i < 256; i++) {
    data_2[i] = temp[i];
  }
  return data_2;
}

// 两个 256 位的无符号数相减data2-data1, 保存到data2中
uint8_t *sub(uint8_t data_1[256], uint8_t data_2[256]) {
  for(int i = 0; i < 256; i++) {
    public_data_1[i] = (!data_1[i]);
  }
  return add(public_data_1, data_2, 1);
}

// 逻辑左移, 将结果保存在public_data中
uint8_t *shl(uint8_t delta, uint8_t data_1[256]) {
  for(int i = 255; i >= 0; i--) {
    if(i >= delta) {
      public_data_2[i] = data_1[i - delta];
    }
    else {
      public_data_2[i] = 0;
    }
  }
  return public_data_2;
}

// 两个256位数相比较，data_1 > data_2 则返回 1
uint8_t cmp(uint8_t data_1[256], uint8_t data_2[256]) {
  for(int i = 255; i >= 0; i--) {
    if(data_1[i] > data_2[i]){
      return 1;
    }
    else if(data_1[i] < data_2[i]) {
      return 0;
    }
  }
  return 0;
}

// 将 64 位数写入数组
uint8_t *write_array(uint64_t data, uint8_t data_array[256]) {
  for (int i = 0; i < 256; i++) {
      if(i < 64) {
        data_array[i] = (data & 0x01);
        data = data >> 1;
      }
      else {
        data_array[i] = 0;
      }
  }
  return data_array;
}

// 在数组中读出 64 位数
uint64_t read_array(uint8_t data_array[256]) {
  uint64_t result = 0;
  for (int i = 63; i >= 0; i--) {
    result = result << 1;
    result = result + data_array[i];
  }
  return result;
}

// 两个 256 位数相乘，低 256 位放在data_2
uint8_t *mul(uint8_t data_1[256], uint8_t data_2[256]) {
  uint8_t temp [256] = {0};
  for(int i = 0; i < 256; i++) {
    if(data_1[i]) {
      add(shl(i, data_2), temp, 0);
    }
  }
  for(int i = 0; i < 256; i++) {
    data_2[i] = temp[i];
  }
  return data_2;
}

// 256位数取余数 data2%data1， 放在data_2, 注意：data_1 < 2^64
uint8_t *mod(uint8_t data_1[256], uint8_t data_2[256]) {
  uint64_t result = read_array(data_1);
  if(result == 0 || result == 1) {
    for(int i = 255; i >= 0; i--) {
      data_2[i] = 0;
    }
  }
  // data_1 >= 2, 故除法结果一定小于 2^64 
  else {
    for(int i = 127; i >= 0; i--) {
      // data_1 << i  >  data_2
      if(cmp(shl(i, data_1), data_2) == 0) {
        sub(shl(i, data_1), data_2);
      }
    }
  }
  return data_2;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  // 读入 64 位数据
  write_array(a, num_1);
  write_array(b, num_2);
  mul(num_1, num_2);
  write_array(m, num_1);
  return read_array(mod(num_1, num_2));
}

