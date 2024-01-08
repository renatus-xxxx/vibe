#include <stdio.h>
#include <string.h>
#include "libiot.h"

/* porting from */
/* https://github.com/Ninune-wa/MSX0-Sensor-Utility/tree/main/MSX0Stack-VibrationMotor */

#define SLAVE_ADDR_AXP192 "34" /* https://github.com/m5stack/M5-Schematic/blob/master/Core/AXP192%20Datasheet_v1.1_en_draft_2211.pdf */

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
  unsigned char buff[3];
  buff[0] = 0x12;
  buff[1] = 0;
  int ret = iotputs("device/i2c_i/" SLAVE_ADDR_AXP192, buff);
  unsigned char get = (unsigned char)iotgeti("device/i2c_i/" SLAVE_ADDR_AXP192);
  get ^=  (1 << 3); // (on/off) toggle
  buff[0] = 0x12;
  buff[1] = get;
  buff[2] = 0;
  ret     = iotputs("device/i2c_i/" SLAVE_ADDR_AXP192, buff);
  return 0;
}
