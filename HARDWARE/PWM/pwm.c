#include "pwm.h"



//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//TIM1_PWM_Init(7199,0);//PWM频率=72000/(7199+1)=10Khz

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
   //设置该引脚为复用输出功能,输出TIM1 CH1 CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_9|GPIO_Pin_10; //TIM_CH1 //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	PAout(8) = 0;    
	PAout(9) = 0;
  PAout(10) = 0;
	PAout(11) = 0;
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR3上的预装载寄存器	  //3
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}


void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* TIM2 clock enable */
	//PCLK1经过2倍频后作为TIM2的时钟源等于72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  /*GPIOA Configuration: TIM2 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
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
  TIM_TimeBaseStructure.TIM_Period = arr ;       //ARR 当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = psc;	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	
  /* PWM2 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //使能通道2
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //使能通道3
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   //使能定时器2	
}


/*
 * TIM3 PWM部分初始化
 * PWM输出初始化
 * arr：自动重装值
 * psc：时钟预分频数
 */

void TIM3_PWM_Init( u16 arr, u16 psc )
{
	/*
	 * 全部映射，将TIM3_CH2映射到PB5
	 * 根据STM32中文参考手册2010中第第119页可知：
	 * 当没有重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PA6，PA7,PB0,PB1
	 * 当部分重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PB4，PB5,PB0,PB1
	 * 当完全重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PC6，PC7,PC8,PC9
	 * 也即是说，完全重映射之后，四个通道的PWM输出引脚分别为PC6，PC7,PC8,PC9，我们用到了通道1和通道2，
	 * 所以对应引脚为PC6，PC7,PC8,PC9，我们用到了通道1和通道2，所以对应引脚为
	 */

	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;


	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );                          /* 使能定时器3时钟 */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE );   /* 使能GPIO外设和AFIO复用功能模块时钟 */


                            /* Timer3 完全重映射 */


	/* 设置PC6、PC7、PC8、PC9为复用输出，输出4路PWM */
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	/* 初始化TIM3 */
	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* 设置用来作为TIMx时钟频率除数的预分频值 */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM向上计数模式 */
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );                       /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */


	/* 初始化TIM3 Channel 1 PWM模式 */
	TIM_OCInitStructure.TIM_OCMode		= TIM_OCMode_PWM1;              /*选择定时器模式:TIM脉冲宽度调制模式1  在小于时有效 */
	TIM_OCInitStructure.TIM_OutputState	= TIM_OutputState_Enable;       /* 比较输出使能 */
	TIM_OCInitStructure.TIM_OCPolarity	= TIM_OCPolarity_High;          /* 输出极性:TIM输出比较极性高 有效时为高 */

	TIM_OC1Init( TIM3, &TIM_OCInitStructure );                              /* 根据T指定的参数初始化外设TIM3 OC1 */
	TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Enable );                     /* 使能TIM3在CCR1上的预装载寄存器 */

	TIM_OC2Init( TIM3, &TIM_OCInitStructure );                              /* 根据T指定的参数初始化外设TIM3 OC1 */
	TIM_OC2PreloadConfig( TIM3, TIM_OCPreload_Enable );                     /* 使能TIM3在CCR1上的预装载寄存器 */

TIM_OC3Init( TIM3, &TIM_OCInitStructure );                              /* 根据T指定的参数初始化外设TIM3 OC1 */
	TIM_OC3PreloadConfig( TIM3, TIM_OCPreload_Enable );                     /* 使能TIM3在CCR1上的预装载寄存器 */

TIM_OC4Init( TIM3, &TIM_OCInitStructure );                              /* 根据T指定的参数初始化外设TIM3 OC1 */
	TIM_OC4PreloadConfig( TIM3, TIM_OCPreload_Enable );                     /* 使能TIM3在CCR1上的预装载寄存器 */

//	                    /* 使能TIM3在CCR1上的预装载寄存器 */


	TIM_Cmd( TIM3, ENABLE );                                                /* 使能TIM3 */
}

void TIM4_PWM_Init( u16 arr, u16 psc )
{
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );                          /* 使能定时器3时钟 */
	RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE );   /* 使能GPIO外设和AFIO复用功能模块时钟 */

  GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	/* 初始化TIM */
	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* 设置在下一个更新事件装入活动的自动重装载寄存器周期的值 */
	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* 设置用来作为TIMx时钟频率除数的预分频值 */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            /* 设置时钟分割:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM向上计数模式 */
	TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure );                       /* 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 */


	/* 初始化TIM Channel  PWM模式 */
	TIM_OCInitStructure.TIM_OCMode		= TIM_OCMode_PWM1;              /*选择定时器模式:TIM脉冲宽度调制模式1  在小于时有效 */
	TIM_OCInitStructure.TIM_OutputState	= TIM_OutputState_Enable;       /* 比较输出使能 */
	TIM_OCInitStructure.TIM_OCPolarity	= TIM_OCPolarity_High;          /* 输出极性:TIM输出比较极性高 有效时为高 */

	TIM_OC1Init( TIM4, &TIM_OCInitStructure );                              /* 根据T指定的参数初始化外设TIM3 OC1 */
	TIM_OC1PreloadConfig( TIM4, TIM_OCPreload_Enable );                     /* 使能TIM3在CCR1上的预装载寄存器 */

	TIM_OC2Init( TIM4, &TIM_OCInitStructure );                              /* 根据T指定的参数初始化外设TIM3 OC1 */
	TIM_OC2PreloadConfig( TIM4, TIM_OCPreload_Enable );                     /* 使能TIM3在CCR1上的预装载寄存器 */

TIM_OC3Init( TIM4, &TIM_OCInitStructure );                              /* 根据T指定的参数初始化外设TIM3 OC1 */
	TIM_OC3PreloadConfig( TIM4, TIM_OCPreload_Enable );                     /* 使能TIM3在CCR1上的预装载寄存器 */

TIM_OC4Init( TIM4, &TIM_OCInitStructure );                              /* 根据T指定的参数初始化外设TIM3 OC1 */
	TIM_OC4PreloadConfig( TIM4, TIM_OCPreload_Enable );                     /* 使能TIM3在CCR1上的预装载寄存器 */

//	                    /* 使能TIM3在CCR1上的预装载寄存器 */


	TIM_Cmd( TIM4, ENABLE );                                                /* 使能TIM3 */
}



