#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "motor.h"
#include "LED.h"
#include "PWM.h"
#include "Time.h"

#include "OLED.h"
#include "Infrared.h"
#include "Control.h"
#include "BuleT.h"
int monment=1;
int num;
int main()
{
	
	char BuleDate;
	extern  int OUTPUT1,OUTPUT2;
	LED_Init();
	OLED_Init();
	Motor_Init();
	
	PWM_Init();
	//BuleInit();
	Timer_Init();
	redInit();
	TB6612_Init();//先关
	//PWM_SetCompare4(50);
	//PWM_SetCompare3(50);  //right右边
	OLED_ShowString(1, 1, "RxData:");
	OLED_ShowString(3, 1, "U1:");
	OLED_ShowString(3, 9, "U2:");
	
	Delay_s(5);
	TIM_Cmd(TIM2,ENABLE);    //开启定时器
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	
	while(1)
	{
		//GPIO_SetBits(GPIOB,GPIO_Pin_3);
		read_sensor();
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		Moto_G(50);
		//BuleDate=BuleT_GetRxData();
		//BulCon(BuleDate);
		OLED_ShowNum(1, 8,BuleDate, 4);
		
		OLED_ShowHexNum(2, 1,read(1), 1);
		OLED_ShowHexNum(2, 4,read(2), 1);
		OLED_ShowHexNum(2, 7,read(3), 1);
		OLED_ShowHexNum(2, 10,read(4), 1);
		OLED_ShowHexNum(2, 13,read(5), 1);
		
		OLED_ShowSignedNum(3, 4,OUTPUT1, 4);
		OLED_ShowSignedNum(3, 11,OUTPUT2, 4);
		OLED_ShowSignedNum(1, 13,num, 3);
		OLED_ShowSignedNum(4, 1,monment, 1);
		//OLED_ShowNum(4, 1,Hcsr04GetLength(),4);
		//BZconl();
	}
}
