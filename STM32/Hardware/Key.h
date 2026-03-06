// oled.h
#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

void OLED_Init(void);
void OLED_ShowLockStatus(uint8_t isLocked);
void OLED_ShowError(void);

#endif