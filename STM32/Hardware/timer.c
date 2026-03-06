#include "timer.h"

extern vu16 USART2_RX_STA;  // 修改为USART2的接收状态变量

// TIM4中断服务程序（替代原TIM7）
void TIM4_IRQHandler(void)
{ 	
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  // 检查TIM4更新中断
    {	 			   
        USART2_RX_STA |= 1 << 15;      // 标记接收完成（修改为USART2）
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  // 清除中断标志    
        TIM_Cmd(TIM4, DISABLE);        // 关闭TIM4
    }	    
}

// 通用定时器4初始化（STM32F103C8T6可用定时器）
void TIM4_Int_Init(u16 arr, u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // 使能TIM4时钟（注意：TIM4在APB1总线上）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // 配置定时器基础参数
    TIM_TimeBaseStructure.TIM_Period = arr;          // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;       // 预分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  // 初始化TIM4

    // 使能更新中断
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
    // 配置TIM4中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  // TIM4中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM4, ENABLE);  // 启动定时器
}