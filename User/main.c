#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include <string.h>
#include "PWM.h"
#include <stdlib.h>


uint8_t BreathingFlag =0;
uint16_t BreathingT=5;

int main(void)
{
	OLED_Init();
	LED_Init();
	PWM_Init();
	Serial_Init();
	
	OLED_ShowString(1,1,"TxPacket");
	OLED_ShowString(3,1,"RxPacket");
	
	
	
	
	while (1)
	{
		if (Serial_RxFlag==1)
		{
			OLED_ShowString(4,1,"                ");
			OLED_ShowString(4,1,Serial_RxPacket);
			
			if (  Serial_RxPacket[   strlen(Serial_RxPacket)-1  ] == '%')   //亮度调节
			{
				Serial_RxPacket[   strlen(Serial_RxPacket)-1  ] ='\0';
				
				
				PWM_SetCompare1( atoi(Serial_RxPacket) );
				printf("LED_%d%%\r\n",atoi(Serial_RxPacket));
				OLED_ShowString(2,1,"                ");
				OLED_ShowString(2,1,"LED_");
				OLED_ShowString(2,5,Serial_RxPacket);
				OLED_ShowString(2,8,"%");
			}
			else if  (strcmp(Serial_RxPacket, "Breathing_ON") == 0)    //打开呼吸灯 默认1s
			{
				BreathingFlag = 1;
				Serial_SendString("Breathing_ON_OK\r\n");
				OLED_ShowString(2,1,"                ");
				OLED_ShowString(2,1,"Breathing_ON_OK");
			}
			else if  (strcmp(Serial_RxPacket, "Breathing_OFF") == 0)    //关闭呼吸灯
			{
				BreathingFlag = 0;
				Serial_SendString("Breathing_OFF_OK\r\n");
				OLED_ShowString(2,1,"                ");
				OLED_ShowString(2,1,"Breathing_OFF_OK");
			}
			
			
			else if  (Serial_RxPacket[   strlen(Serial_RxPacket)-1  ] == 's')    //更改呼吸灯周期
			{
				Serial_RxPacket[   strlen(Serial_RxPacket)-1  ] ='\0';
				BreathingT = 5*atoi(Serial_RxPacket);
				BreathingFlag = 1;
				printf("BreathingT_%ds\r\n",atoi(Serial_RxPacket));
				OLED_ShowString(2,1,"                ");
				OLED_ShowString(2,1,"BreathingT_");
				OLED_ShowString(2,12,Serial_RxPacket);
				OLED_ShowString(2,13,"s");
			}
			
			
			else
			{
				Serial_SendString("ERROR_COMMAND\r\n");
				OLED_ShowString(2,1,"                ");
				OLED_ShowString(2,1,"ERROR_COMMAND");
			}
			Serial_RxFlag=0;
		}
			
			
			
			
			
		if (BreathingFlag==1)
		{
			for (uint8_t i=0;i<100;i++)
			{
				PWM_SetCompare1(i);
				Delay_ms(BreathingT);
			}
	
			for (uint8_t i=100;i>0;i--)
			{
				PWM_SetCompare1(i);
				Delay_ms(BreathingT);
			}
		}
			
		
		
	}
	
	
}
