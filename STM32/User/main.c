#include "stm32f10x.h"                  // Device header
//#include "rc522.h"
#include "Delay.h"
#include "servo.h"
#include "stdio.h"
#include "rc522.h"
#include "usart.h"

static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数
void delay_init(u8 SYSCLK);
unsigned char buf[20];  //存放卡序号


	

int main(void)
{
	unsigned char status;		//返回符
	unsigned int temp,i;
	delay_init(72);
	Servo_Init();
	Buzzer_Init();
	OLED_Init();
	MFRC522_Init();
	USART1_Config();
	
	OLED_ShowNum(2, 1, 12345, 5);
	printf ( "MFRC522 Test\r\n" );
	while (1)
	{
		status = MFRC522_Request(PICC_REQALL, buf);//寻卡
			if (status != MI_OK)
			{    

					MFRC522_Reset();
					MFRC522_AntennaOff(); 
					MFRC522_AntennaOn(); 
					continue;
			}

			printf("卡的类型:");
			for(i=0;i<2;i++)
			{
					temp=buf[i];
					printf("%02X",temp);

			}
		
			status = MFRC522_Anticoll(buf);//防冲突
			if (status != MI_OK)
			{    
						continue;    
			}
			
			////////以下为超级终端打印出的内容////////////////////////

			printf("卡序列号：");	//超级终端显示,
			for(i=0;i<4;i++)
			{
					temp=buf[i];
					printf("%02X",temp);

			}
			
			printf("\r\n");

//		Servo_SetAngle(0);
//		Delay_ms(500);
//		Buzzer_Beep(1);
//		Servo_SetAngle(90);
//		Delay_ms(500);
//		Buzzer_Beep(3);
//		Servo_SetAngle(0);
//		Delay_ms(500);
	}
}

void delay_init(u8 SYSCLK)
{
//	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								    