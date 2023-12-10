#include <stdio.h>
#include <string.h>
#include "libiot.h"

#define SLAVE_ADDR_AXP192   "34" /* https://github.com/m5stack/M5-Schematic/blob/master/Core/AXP192%20Datasheet_v1.1_en_draft_2211.pdf */

void sleep(int frame) {
  unsigned int* timer = (unsigned int*)0xfc9e;
  *timer = 0;
  while(*timer < frame);
}

int init(char* addr) {
  int num = iotfindi("device/i2c_i");
  if (num == -1) { return -1; }
  char** str = malloc(sizeof(char*) * num);
  for(int i = 0; i < num; i++) {
    str[i] = malloc(sizeof(char) * 64);
  }
  int cnt = iotfinds("device/i2c_i", str, num);
  int found = -1;
  for(int i = 0; i < num; i++) {
    if (strcmp(str[i], addr) == 0){
      found = 0;
    }
    free(str[i]);
  }
  free(str);
  return found;
}

int main( int argc, char *argv[]) {
  if (init(SLAVE_ADDR_AXP192)) {
    printf("AXP192(0x%s) not found.", SLAVE_ADDR_AXP192);
    return -1;
  }
  int ret = iotputb("device/i2c_i" SLAVE_ADDR_AXP192, 0x12, 1);
  int get = iotgeti("device/i2c_i" SLAVE_ADDR_AXP192);
  printf("get=%d\n", get);
  get |= 0b00001000; // o:119, x:0
  char buff[2];
  buff[0] = 0x12;
  buff[1] = get;
  ret     = iotputb("device/i2c_i" SLAVE_ADDR_AXP192, buff, 2);
  sleep(120);
  return 0;
}
