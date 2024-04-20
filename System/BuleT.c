#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "motor.h"
#include "Control.h"

float sendRoad = 0;
extern int monment;
void BuleInit()       
{         //PB10  PB11
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;  
   NVIC_InitTypeDef NVIC_InitStrue;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  //复用推挽输出
   GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;  
   GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;  
   GPIO_Init(GPIOB,&GPIO_InitStrue);  

	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING; //浮空输入模式
   GPIO_InitStrue.GPIO_Pin=GPIO_Pin_11;  
   GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;  
   GPIO_Init(GPIOB,&GPIO_InitStrue);  

	USART_InitStrue.USART_BaudRate=9600;  //设置波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //不使用流控 
   USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  //设置串口模式
	USART_InitStrue.USART_Parity=USART_Parity_No;       //不需要校验
   USART_InitStrue.USART_StopBits=USART_StopBits_1;   //停止位数
   USART_InitStrue.USART_WordLength=USART_WordLength_8b;  //设置接受的长度

	USART_Init(USART3,&USART_InitStrue);

	
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//开启接收中断  
      
   NVIC_InitStrue.NVIC_IRQChannel=USART3_IRQn;  
   NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
   NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=2;  
   NVIC_InitStrue.NVIC_IRQChannelSubPriority=2;  
   NVIC_Init(&NVIC_InitStrue);  

	USART_Cmd(USART3,ENABLE); //使能串口1
}
unsigned char RxDate;
//返回接收到的数据
uint8_t BuleT_GetRxData()
{
	return RxDate;
}

void BulCon(char Date)
{
	
	if(Date==0)
	{
		//TIM_Cmd(TIM2, DISABLE);
		//Moto_Stop();
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);//TB6612失能
		monment=1;   //路口检测重新回退
	}
	if(Date==1)
	{  
		GPIO_SetBits(GPIOB,GPIO_Pin_3);
		PWM_SetCompare3(100-5);
		PWM_SetCompare4(100);
	}
	if(Date==2)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_3);
		
		PWM_SetCompare3(95);
		PWM_SetCompare4(100);
	}
	if(Date==3)
	{
		//GPIO_SetBits(GPIOB,GPIO_Pin_3);
		PWM_SetCompare3(90);
		PWM_SetCompare4(20);
	}
	if(Date==4)
	{
		//GPIO_SetBits(GPIOB,GPIO_Pin_3);
		Moto_B(50);
	}
   if(Date==5)  //遥控模式
	{
	   TIM_Cmd(TIM2, DISABLE);  //关闭定时器
		
		//GPIO_SetBits(GPIOB,GPIO_Pin_3);
	}
	if(Date==6)  //循迹模式
	{
		//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM2,ENABLE);    //开启定时器
		GPIO_SetBits(GPIOB,GPIO_Pin_3);
	}
	
}

void USART3_IRQHandler()
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!= RESET)    /*接收中断*/
	  {
			
	      RxDate =USART_ReceiveData (USART3);
			//OLED_ShowHexNum(1, 8,RxDate, 2);
			USART_ClearITPendingBit(USART3,USART_IT_RXNE);      /*清楚接收中断标志*/
			
			   
			//USART_SendData(USART1,RxDate);  
	 }

}
