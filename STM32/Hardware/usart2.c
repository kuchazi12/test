#include "usart2.h"  // 头文件重命名为usart2.h

#define USART2_RXBUF_LEN 15
u8 t;
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; // 接收缓冲
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; // 发送缓冲
vu16 USART2_RX_STA = 0;

// 初始化串口2，使用PA2(TX)和PA3(RX)
void uart2_init(u32 bound) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  // USART2时钟

    USART_DeInit(USART2);  // 复位串口2

    // 配置PA2为USART2_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置PA3为USART2_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART2参数配置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    // 中断配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);                  // 使能串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 开启接收中断

    TIM4_Int_Init(1000-1, 7200-1);  // 10ms中断（保持原有定时器配置）
    USART2_RX_STA = 0;              // 接收状态清零
    TIM_Cmd(TIM4, DISABLE);         // 先关闭定时器
}

// 发送字符串
void Uart2_SendStr(u8* SendBuf, u8 len) {
    for(t = 0; t < len; t++) {
        USART_SendData(USART2, SendBuf[t]);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET); // 等待发送完成
    }
}

// printf重定向
void u2_printf(char* fmt, ...) {
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)USART2_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char*)USART2_TX_BUF);
    for(j = 0; j < i; j++) {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, USART2_TX_BUF[j]);
    }
}

// 中断服务函数
void USART2_IRQHandler(void) {
    u8 res = 0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        res = USART_ReceiveData(USART2);
        if((USART2_RX_STA & (1 << 15)) == 0) { // 数据未处理完
            if(USART2_RX_STA < USART2_MAX_RECV_LEN) {
                TIM_SetCounter(TIM4, 0);       // 定时器复位
                if(USART2_RX_STA == 0) TIM_Cmd(TIM4, ENABLE);
                USART2_RX_BUF[USART2_RX_STA++] = res;
            } else {
                USART2_RX_STA |= 1 << 15;      // 强制标记接收完成
            }
        }
    }
}