#include "stm32f10x.h"                  // Device header
#include "Time.h"
#include "PWM.h"
#include "Infrared.h"
#include "OLED.h"
 int OUTPUT1,OUTPUT2;
float Integral_bias = 0.0;
extern int num;
extern float error;
extern int a;
int PID_di=0;
int PID_ba=0;

void Xianfu_PID(void);
void Motor(void);
int Position_PD (int Encoder,int Target);
int Position_PID (float Encoder,int Target);


//定时中断执行10ms
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		PID_ba=Position_PD (a,0);
		PID_di=Position_PID (error,0);
		Xianfu_PID(); 
		//USART_SendData(USART1, OUTPUT1|OUTPUT2);
		//OLED_ShowHexNum(3, 6,OUTPUT1, 3);
		//OLED_ShowHexNum(3, 14,OUTPUT2, 3);
		Motor();
		num+=1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

int Position_PD (int Encoder,int Target)   //基础速度PID控制器//这里Encoder=a，Target=0;
{  
	 float Position_bKP=10,Position_bKD=0; //参数整定，需要调到适合自己小车的数值大小  //因为没有用到KI(积分)，所以这里去除了和KI相关的式子
	 static float Bias,Pwm,Last_Bias;
	 Bias=Encoder-Target; //偏差=实际值-目标值
	 Pwm=100-Position_bKP*Bias+Position_bKD*(Bias-Last_Bias);//位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //返回PWM值
}

int Position_PID (float Encoder,int Target)   //差速PID控制器//这里Encoder=error，Target=0;
{ 	
	 float Position_KP=8,Position_KI=0.0066,Position_KD=0;
	 static float Bias,Pwm,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);//位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //返回PWM值
}

void Motor()  //将OUTPUT，OUTPUT1，OUTPUT2输出出去，使小车电机受到相应控制
{
     TIM_SetCompare3(TIM3,OUTPUT1-5);//左边电机控制
	  TIM_SetCompare4(TIM3,OUTPUT2);//右边电机控制
}	

void Xianfu_PID()//限幅
{	
	  OUTPUT1=PID_ba+PID_di;//基础速度+速度偏差量 //基础速度也可以恒定为某个值，这样调差速和舵机就比较好调
	  OUTPUT2=PID_ba-PID_di;//基础速度-速度偏差量
	
	   /*控制小车电机的PWM有效值范围为0-100*/
	  if(OUTPUT1>100) OUTPUT1=100;
     if(OUTPUT1<10) OUTPUT1=10;
	  if(OUTPUT2>100) OUTPUT2=100;
     if(OUTPUT2<10) OUTPUT2=10;
		
}


