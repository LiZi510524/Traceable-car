#include "stm32f10x.h" 
#include "PWM.h"
// Device header
   //AIN1 :PA4
   //AIN2 :PA5 
   //BIN1 :PB12
	//BIN2 :PB13
	//STBY :PB3
	// 00ֹͣ   01��ת   10��ת
void TB6612_Init()
{          
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	
	//��GPIO��ʱ�ӣ��ȴ򿪸��ò����޸ĸ��ù���   B3  B4
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//Ҫ�ȿ�ʱ�ӣ�����ӳ�䣻����ʾ�ر�jtag��ʹ��swd
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
   GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Moto_G(int PWM)  //ֱ�в�����85
{
	//GPIO_ResetBits(GPIOB,GPIO_Pin_3); //TB6612ʹ��
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	
	PWM_SetCompare3(PWM);
	PWM_SetCompare4(PWM);
}

void Moto_B(int pwm)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);  //0
	GPIO_SetBits(GPIOA,GPIO_Pin_4);    //1
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_13); //0
	GPIO_SetBits(GPIOB,GPIO_Pin_12);   //1
	
	PWM_SetCompare3(pwm);
	PWM_SetCompare4(pwm);
}

void Moto_Stop()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);//TB6612ʧ��
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	PWM_SetCompare3(0);
	PWM_SetCompare4(0);
}


