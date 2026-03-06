// servo.c
#include "servo.h"

// TIM2_CH1配置
void Servo_Init() {
  TIM_TimeBaseInitTypeDef TIM_BaseStruct;
  TIM_OCInitTypeDef TIM_OCStruct;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  // PWM输出引脚配置
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  // 定时器配置（20ms周期）
  TIM_BaseStruct.TIM_Prescaler = 72-1; // 72MHz/72=1MHz
  TIM_BaseStruct.TIM_Period = 20000-1; // 20ms
  TIM_TimeBaseInit(TIM2, &TIM_BaseStruct);
  
  // PWM通道配置
  TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCStruct.TIM_Pulse = 1500; // 初始位置1.5ms
  TIM_OC1Init(TIM2, &TIM_OCStruct);
  
  TIM_Cmd(TIM2, ENABLE);
}

void Servo_SetAngle(uint8_t angle) {
  uint16_t pulse = 500 + angle * 11; // 角度转脉冲(0-180度对应500-2500us)
  TIM2->CCR1 = pulse;
}