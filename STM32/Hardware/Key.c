// oled.c
#include "oled.h"
#include "i2c.h"

// SSD1306 OLED驱动实现
void OLED_Init() {
  // I2C初始化代码
  // OLED初始化命令序列
}

void OLED_ShowLockStatus(uint8_t isLocked) {
  if(isLocked) {
    OLED_Clear();
    OLED_ShowString(0, 0, "Status: Locked");
  } else {
    OLED_Clear();
    OLED_ShowString(0, 0, "Status: Unlocked");
  }
}

void OLED_ShowError() {
  OLED_Clear();
  OLED_ShowString(0, 0, "Invalid Card!");
}