// rc522.c
#include "rc522.h"
#include "spi.h"

// SPI硬件配置
void RC522_Init() {
  SPI_InitTypeDef SPI_InitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;
  
  // SPI1初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_Init(SPI1, &SPI_InitStruct);
  SPI_Cmd(SPI1, ENABLE);
  
  // 控制引脚初始化
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3; // SDA和RST
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// 检查卡片有效性（核心函数）
uint8_t RC522_CheckCard(uint8_t* id) {
  // 实现寻卡、防冲突、验证密钥、读取卡号等操作
  // 返回1表示验证成功，0表示失败
}