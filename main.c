#include <stdio.h>
#include <string.h>
#include "libiot.h"

/* porting from */
/* https://github.com/Ninune-wa/MSX0-Sensor-Utility/tree/main/MSX0Stack-VibrationMotor */

#define SLAVE_ADDR_AXP192   "34" /* https://github.com/m5stack/M5-Schematic/blob/master/Core/AXP192%20Datasheet_v1.1_en_draft_2211.pdf */

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

void sleep(int frame) {
  unsigned int* timer = (unsigned int*)0xfc9e;
  *timer = 0;
  while(*timer < frame);
}

int main( int argc, char *argv[]) {
  if (init(SLAVE_ADDR_AXP192)) {
    printf("AXP192(0x%s) not found.", SLAVE_ADDR_AXP192);
    return -1;
  }
  unsigned char send = 0x12;
  int ret = iotputb("device/i2c_i" SLAVE_ADDR_AXP192, send, 1);
  unsigned char get[8]; // o:119, x:128
  for(int i = 0; i < 8; i++) {
    get[i] = 0;
  }
  iotgetb("device/i2c_i" SLAVE_ADDR_AXP192, get);
  for(int i = 0; i < 8; i++) {
    printf("get[%d]=%d\n", i, get[i]);
  }
  get[0] |= (1 << 3);
  printf("get[0]=%d\n", get[0]);
  unsigned char buff[8];
  buff[0] = 0x12;
  buff[1] = get[0];
  buff[2] = 0;
  ret     = iotputb("device/i2c_i" SLAVE_ADDR_AXP192, buff, 2);
  printf("ret=%d\n", i, ret);
  return 0;
}
