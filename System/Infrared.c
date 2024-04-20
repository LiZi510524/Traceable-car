#include "stm32f10x.h"  // Device header
#include "Control.h"
#include "motor.h"
#include "PWM.h"
#include "Delay.h"

float error=0;  //误差值
int a=0;      //基速
int redData;

extern int monment;   //用来测定不同的路口时刻
int can=0;
void redInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//           [  1     2     3     4      5   ]
	//  B5   B4  [ A15   A12   A11    A8    B15  ]   B14
	GPIO_InitTypeDef GPIO_InitStrue;
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IPU;  //上拉输入
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_8;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IPU;   //浮空输入GPIO_Mode_IN_FLOATING
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_15|GPIO_Pin_14;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStrue);
}

int read(int Data)
{
	if(Data==1)
	{
		redData=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
		return redData;
	}
	else if(Data==2)
	{
		redData=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
		return redData;
	}
	else if(Data==3)
	{
		redData=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
		return redData;
	}
	else if(Data==4)
	{
		redData=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
		return redData;
	}
	else if(Data==5)
	{
		redData=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
		return redData;
	}
	else
	{
		return 0;
	}
}

void read_sensor()
{
	if(read(1)==1 &&read(2)==1 &&read(3)==0 &&read(4)==1 &&read(5)==1)
	{//    【中线】
		error=0;  a=0;     can=1;
		Integral_bias = 0.0;  
	}
	else if(read(1)==1 &&read(2)==0 &&read(3)==0 &&read(4)==1 &&read(5)==1)
	{//   【左0.5】
		error=-1;  a=1;   can=1;
	}
	else if(read(1)==1 &&read(2)==1 &&read(3)==0 &&read(4)==0 &&read(5)==1)
	{//   【右0.5】
		error=1;  a=1;    can=1;
	}
	else if(read(1)==1 &&read(2)==0 &&read(3)==1 &&read(4)==1 &&read(5)==1)
	{//   【左1】
		error=-2.5;  a=2;  can=1;
	}
	else if(read(1)==1 &&read(2)==1 &&read(3)==1 &&read(4)==0 &&read(5)==1)
	{//   【右1】
		error=2.5;  a=2;  can=1;
	}
	else if(read(1)==0 &&read(2)==0 &&read(3)==1 &&read(4)==1 &&read(5)==1)
	{//   【左1.5】
		error=-3;   a=3;   can=1;
	}
	else if(read(1)==1 &&read(2)==1 &&read(3)==1 &&read(4)==0 &&read(5)==0)
	{//   【右1.5】
		error=3;  a=3;   can=1;
	}
	else if(read(1)==0 &&read(2)==1 &&read(3)==1 &&read(4)==1 &&read(5)==1)
	{//   【左2】
		error=-5;   a=4;   can=1;
	}
	else if(read(1)==1 &&read(2)==1 &&read(3)==1 &&read(4)==1 &&read(5)==0)
	{//   【右2】
		error=5;  a=4;     can=1;
	}
	
	else if(read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==1 &&read(5)==1 )
	{//   【直角弯向左】
		error=-8;   a=5;   can=1;
	}
	else if(read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==1 &&monment==4)
	{//   【直角弯向左】
		error=-8;   a=5;   can=1;
	}
	
	
	else if((read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==1 &&can==1)||
		      (read(1)==1 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==1 &&can==1)||
				(read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==1 &&monment==1 &&can==1))
	{//  【第一个十字路口】 【向右转】
		can=0;
		monment++;
		//error = 7;   a=4;
		TIM_Cmd(TIM2, DISABLE);  //关闭定时器
		PWM_SetCompare3(95);
		PWM_SetCompare4(10);
		Delay_ms(440);
		TIM_Cmd(TIM2,ENABLE);   //开启定时器
	}
	else if((read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==2 &&can==1)||
		      (read(1)==1 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==2 &&can==1)||
	        (read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==1 &&monment==2 &&can==1))
	{//  【第二个十字路口】 【向右转】
		can=0;
		monment++;
		//error = 7;   a=4;  
		TIM_Cmd(TIM2, DISABLE);  //关闭定时器
		PWM_SetCompare3(95);
		PWM_SetCompare4(5);
		Delay_ms(440);
		TIM_Cmd(TIM2,ENABLE);   //开启定时器
	}
	else if((read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==3 &&can==1)||
		      (read(1)==1 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==3 &&can==1)||
	        (read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==1 &&monment==3 &&can==1))
	{//  【第三个十字路口】 【向右转】
		can=0;
		monment++;
		//error = 7;   a=4;  
		TIM_Cmd(TIM2, DISABLE);  //关闭定时器
		PWM_SetCompare3(95);
		PWM_SetCompare4(10);
		Delay_ms(410);
		TIM_Cmd(TIM2,ENABLE);   //开启定时器
	}
	
	//【停车线检测】
	else if((read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==4 &&can==1)||
		      (read(1)==1 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==4 &&can==1)||
	        (read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==1 &&monment==4 &&can==1))
	{//  【第四】 
		can=0;
		monment++;
		//error = 7;   a=4;  
		TIM_Cmd(TIM2, DISABLE);  //关闭定时器
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);//TB6612失能
	}
	else if((read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==5 &&can==1)||
		      (read(1)==1 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==0 &&monment==5 &&can==1)||
	        (read(1)==0 &&read(2)==0 &&read(3)==0 &&read(4)==0 &&read(5)==1 &&monment==5 &&can==1))
	{//  【第五】 
		can=0;
		monment++;
		//error = 7;   a=4;  
		TIM_Cmd(TIM2, DISABLE);  //关闭定时器
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);//TB6612失能
	}
}
