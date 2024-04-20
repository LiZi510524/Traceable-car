#include "stm32f10x.h"                  // Device header

void Timer_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//开启时钟
	
	TIM_InternalClockConfig(TIM2);           //TIM2通用定时器
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //分频数值 1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;     //周期，ARR自动重装器的值 此时为100Hz 0.01s 0.001s
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;  //PSC预分频器的值  [取值0~65535]
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值  高级计时器才用，不需要
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	//定时器频率=CK_PSC(72M)/(PSC+1)/(ARR+1)      1MHz=1000kHz  
	//定时频率:1=72 000 000Hz /(7200 * 10000)       72 000    1000
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //定时器开关 【默认关】蓝牙里开
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, DISABLE);  //【默认开】
}

