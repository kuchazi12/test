#ifndef __USART2_H
#define __USART2_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

#define USART2_MAX_RECV_LEN 1024  // 最大接收长度
#define USART2_MAX_SEND_LEN 1024  // 最大发送长度

// 外部变量声明
extern vu16 USART2_RX_STA;         // 接收状态标记
extern u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; // 接收缓冲区
extern u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; // 发送缓冲区

// 函数声明
void uart2_init(u32 bound);        // 串口2初始化
void u2_printf(char* fmt, ...);    // 格式化输出函数
void Uart2_SendStr(u8* SendBuf, u8 len); // 字符串发送函数

#endif /* __USART2_H */