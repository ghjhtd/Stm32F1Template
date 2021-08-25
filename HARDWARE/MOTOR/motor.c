#include "motor.h"
/**************************************************************************
�������ܣ���ʼ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void Motor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50MHZ
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
	AIN1=0,AIN2=0;
	BIN1=0,BIN1=0;
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���    ----------TB6612�汾
��ڲ���������PWM������PWM
����  ֵ����
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
//�������ܣ���ֵ��PWM�Ĵ��� ----------L298n�汾
//��ڲ���������PWM������PWM
//����  ֵ����
//**************************************************************************/
//void Set_Pwm(int moto1,int moto2)
//{
//    	if(moto1<0)			PWMA1=myabs(moto1),PWMA2=0;
//			else 	          PWMA1=0, PWMA2=myabs(moto1);

//		  if(moto2<0)	    PWMB1=myabs(moto1),PWMB2=0;
//			else 	          PWMB1=0, PWMB2=myabs(moto1);
//}



/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
Ŀ    �ģ�����ֱ�������ٶȻ��Լ�ת�򻷼��������PWM�п���Ϊ��ֵ
					��ֻ�ܸ�����ʱ��PWM�Ĵ���ֻ������ֵ������Ҫ��PWM���о���ֵ����
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{
	 //===PWM������7200 ������7000
    if(Moto1<-7000 ) Moto1=-7000 ;
		if(Moto1>7000 )  Moto1=7000 ;
	  if(Moto2<-7000 ) Moto2=-7000 ;
		if(Moto2>7000 )  Moto2=7000 ;
}
/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������Ǻ͵�ѹ
����  ֵ����
**************************************************************************/
void Turn_Off(float angle, float voltage)
{
		if(angle<-40||angle>40||voltage<11.1)	 //��ص�ѹ����11.1V�رյ��
		{	                                   //===��Ǵ���40�ȹرյ��																			 
				Moto1=0;
				Moto2=0;
		}		
}
	

//����pwm���-----------------------------------------------------------------------
///**************************************************************************
//�������ܣ���ֵ��PWM�Ĵ���
//��ڲ�������� PWM
//����  ֵ����
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
//	TIM_SetCompare1( TIM8, pwm ); 	//c6789    //����pwm
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
////����������
//void MOTO_FX(int moto,u8 fx)
//{
//		switch(moto)
//		{
//	case 1:
//	PCout(0)=fx,PCout(1)=!fx;	//c6789    //����pwm
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
//�������ܣ����ת���ʼ��
//��ڲ�������
//����  ֵ����
//**************************************************************************/
//void Motor_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE); //ʹ��PB PC�˿�ʱ��
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//�˿�����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
//  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
//   
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//�˿�����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
//  GPIO_Init(GPIOC, &GPIO_InitStructure);		
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;	//�˿�����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
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
//����pwm���-----------------------------------------------------------------------


