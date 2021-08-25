#include "motor.h"
/**************************************************************************
函数功能：初始化电机
入口参数：无
返回  值：无
**************************************************************************/
void Motor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50MHZ
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
	AIN1=0,AIN2=0;
	BIN1=0,BIN1=0;
}

/**************************************************************************
函数功能：赋值给PWM寄存器    ----------TB6612版本
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
    	if(moto1<0)			AIN2=0,			AIN1=1;
			else 	          AIN2=1,			AIN1=0;
			PWMA=myabs(moto1);
		  if(moto2>0)	BIN1=1,			BIN2=0;
			else        BIN1=0,			BIN2=1;
			PWMB=myabs(moto2);	
}

///**************************************************************************
//函数功能：赋值给PWM寄存器 ----------L298n版本
//入口参数：左轮PWM、右轮PWM
//返回  值：无
//**************************************************************************/
//void Set_Pwm(int moto1,int moto2)
//{
//    	if(moto1<0)			PWMA1=myabs(moto1),PWMA2=0;
//			else 	          PWMA1=0, PWMA2=myabs(moto1);

//		  if(moto2<0)	    PWMB1=myabs(moto1),PWMB2=0;
//			else 	          PWMB1=0, PWMB2=myabs(moto1);
//}



/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
目    的：经过直立环和速度环以及转向环计算出来的PWM有可能为负值
					而只能赋给定时器PWM寄存器只能是正值。故需要对PWM进行绝对值处理
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{
	 //===PWM满幅是7200 限制在7000
    if(Moto1<-7000 ) Moto1=-7000 ;
		if(Moto1>7000 )  Moto1=7000 ;
	  if(Moto2<-7000 ) Moto2=-7000 ;
		if(Moto2>7000 )  Moto2=7000 ;
}
/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：无
**************************************************************************/
void Turn_Off(float angle, float voltage)
{
		if(angle<-40||angle>40||voltage<11.1)	 //电池电压低于11.1V关闭电机
		{	                                   //===倾角大于40度关闭电机																			 
				Moto1=0;
				Moto2=0;
		}		
}
	

//四轮pwm输出-----------------------------------------------------------------------
///**************************************************************************
//函数功能：赋值给PWM寄存器
//入口参数：电机 PWM
//返回  值：无
//**************************************************************************/
//void Set_Pwm(int moto,int pwm)
//{
//    	if(pwm<0)		MOTO_FX(moto,0);
//			else        MOTO_FX(moto,1);
//			pwm=myabs(pwm);
//	    if(pwm>4000)
//				pwm=4000;
//			
//	switch(moto)
//		{
//	case 1:
//		
//	TIM_SetCompare1( TIM8, pwm ); 	//c6789    //四轮pwm
//  break;
//	case 2:
//	TIM_SetCompare2( TIM8, pwm );
//  break;
//	case 3:
//	TIM_SetCompare3( TIM8, pwm );
//  break;
//	case 4:
//	TIM_SetCompare4( TIM8, pwm );
//  break;

//    }
//}
////电机方向控制
//void MOTO_FX(int moto,u8 fx)
//{
//		switch(moto)
//		{
//	case 1:
//	PCout(0)=fx,PCout(1)=!fx;	//c6789    //四轮pwm
//  break;
//	case 2:
//	PEout(7)=fx,PEout(6)=!fx;
//  break;
//	case 3:
//		PBout(1)=fx,PBout(0)=!fx;
//  break;
//	case 4:
//		PEout(0)=fx,PEout(1)=!fx;
//  break;
//    }
//}


///**************************************************************************
//函数功能：电机转向初始化
//入口参数：无
//返回  值：无
//**************************************************************************/
//void Motor_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE); //使能PB PC端口时钟
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
//  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
//   
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//端口配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
//  GPIO_Init(GPIOC, &GPIO_InitStructure);		
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;	//端口配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
//  GPIO_Init(GPIOE, &GPIO_InitStructure);
//	
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);	
//	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	
//	GPIO_ResetBits(GPIOC,GPIO_Pin_1);	
//	GPIO_ResetBits(GPIOE,GPIO_Pin_0);	
//	GPIO_ResetBits(GPIOE,GPIO_Pin_1);	
//	GPIO_ResetBits(GPIOE,GPIO_Pin_6);	
//	GPIO_ResetBits(GPIOE,GPIO_Pin_7);	
//}
//四轮pwm输出-----------------------------------------------------------------------


