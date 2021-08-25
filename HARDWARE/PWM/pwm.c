#include "pwm.h"



//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//TIM1_PWM_Init(7199,0);//PWMƵ��=72000/(7199+1)=10Khz

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
   //���ø�����Ϊ�����������,���TIM1 CH1 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_9|GPIO_Pin_10; //TIM_CH1 //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	PAout(8) = 0;    
	PAout(9) = 0;
  PAout(10) = 0;
	PAout(11) = 0;
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���	  //3
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
}


void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* TIM2 clock enable */
	//PCLK1����2��Ƶ����ΪTIM2��ʱ��Դ����72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  /*GPIOA Configuration: TIM2 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

/* -----------------------------------------------------------------------
    TIM2 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 71999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 1 KHz.
	CC1 update rate = TIM2 counter clock / CCR1_Val
    TIM3 Channelx duty cycle = (TIM2_CCRx/ TIM2_ARR)* 100 = x%
    
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = arr ;       //ARR ����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = psc;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	
  /* PWM2 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //ʹ��ͨ��2
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //ʹ��ͨ��3
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   //ʹ�ܶ�ʱ��2	
}


/*
 * TIM3 PWM���ֳ�ʼ��
 * PWM�����ʼ��
 * arr���Զ���װֵ
 * psc��ʱ��Ԥ��Ƶ��
 */

void TIM3_PWM_Init( u16 arr, u16 psc )
{
	/*
	 * ȫ��ӳ�䣬��TIM3_CH2ӳ�䵽PB5
	 * ����STM32���Ĳο��ֲ�2010�еڵ�119ҳ��֪��
	 * ��û����ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPA6��PA7,PB0,PB1
	 * ��������ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPB4��PB5,PB0,PB1
	 * ����ȫ��ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPC6��PC7,PC8,PC9
	 * Ҳ����˵����ȫ��ӳ��֮���ĸ�ͨ����PWM������ŷֱ�ΪPC6��PC7,PC8,PC9�������õ���ͨ��1��ͨ��2��
	 * ���Զ�Ӧ����ΪPC6��PC7,PC8,PC9�������õ���ͨ��1��ͨ��2�����Զ�Ӧ����Ϊ
	 */

	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;


	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );                          /* ʹ�ܶ�ʱ��3ʱ�� */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE );   /* ʹ��GPIO�����AFIO���ù���ģ��ʱ�� */


                            /* Timer3 ��ȫ��ӳ�� */


	/* ����PC6��PC7��PC8��PC9Ϊ������������4·PWM */
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	/* ��ʼ��TIM3 */
	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM���ϼ���ģʽ */
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );                       /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */


	/* ��ʼ��TIM3 Channel 1 PWMģʽ */
	TIM_OCInitStructure.TIM_OCMode		= TIM_OCMode_PWM1;              /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1  ��С��ʱ��Ч */
	TIM_OCInitStructure.TIM_OutputState	= TIM_OutputState_Enable;       /* �Ƚ����ʹ�� */
	TIM_OCInitStructure.TIM_OCPolarity	= TIM_OCPolarity_High;          /* �������:TIM����Ƚϼ��Ը� ��ЧʱΪ�� */

	TIM_OC1Init( TIM3, &TIM_OCInitStructure );                              /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Enable );                     /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */

	TIM_OC2Init( TIM3, &TIM_OCInitStructure );                              /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	TIM_OC2PreloadConfig( TIM3, TIM_OCPreload_Enable );                     /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */

TIM_OC3Init( TIM3, &TIM_OCInitStructure );                              /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	TIM_OC3PreloadConfig( TIM3, TIM_OCPreload_Enable );                     /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */

TIM_OC4Init( TIM3, &TIM_OCInitStructure );                              /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	TIM_OC4PreloadConfig( TIM3, TIM_OCPreload_Enable );                     /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */

//	                    /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */


	TIM_Cmd( TIM3, ENABLE );                                                /* ʹ��TIM3 */
}

void TIM4_PWM_Init( u16 arr, u16 psc )
{
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );                          /* ʹ�ܶ�ʱ��3ʱ�� */
	RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE );   /* ʹ��GPIO�����AFIO���ù���ģ��ʱ�� */

  GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	/* ��ʼ��TIM */
	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM���ϼ���ģʽ */
	TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure );                       /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */


	/* ��ʼ��TIM Channel  PWMģʽ */
	TIM_OCInitStructure.TIM_OCMode		= TIM_OCMode_PWM1;              /*ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1  ��С��ʱ��Ч */
	TIM_OCInitStructure.TIM_OutputState	= TIM_OutputState_Enable;       /* �Ƚ����ʹ�� */
	TIM_OCInitStructure.TIM_OCPolarity	= TIM_OCPolarity_High;          /* �������:TIM����Ƚϼ��Ը� ��ЧʱΪ�� */

	TIM_OC1Init( TIM4, &TIM_OCInitStructure );                              /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	TIM_OC1PreloadConfig( TIM4, TIM_OCPreload_Enable );                     /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */

	TIM_OC2Init( TIM4, &TIM_OCInitStructure );                              /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	TIM_OC2PreloadConfig( TIM4, TIM_OCPreload_Enable );                     /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */

TIM_OC3Init( TIM4, &TIM_OCInitStructure );                              /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	TIM_OC3PreloadConfig( TIM4, TIM_OCPreload_Enable );                     /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */

TIM_OC4Init( TIM4, &TIM_OCInitStructure );                              /* ����Tָ���Ĳ�����ʼ������TIM3 OC1 */
	TIM_OC4PreloadConfig( TIM4, TIM_OCPreload_Enable );                     /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */

//	                    /* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */


	TIM_Cmd( TIM4, ENABLE );                                                /* ʹ��TIM3 */
}



