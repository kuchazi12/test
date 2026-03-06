// rc522.h
#ifndef __RC522_H
#define __RC522_H

#include "stm32f10x.h"

#define RC522_AUTH_KEYA 0x60
#define RC522_AUTH_KEYB 0x61

void RC522_Init(void);
uint8_t RC522_CheckCard(uint8_t* id);

#endif