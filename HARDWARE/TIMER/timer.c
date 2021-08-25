#include "timer.h"

//��ʱ��1�����жϳ�ʼ��
void TIM1_Int_Init( u16 arr, u16 psc )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );                  /* ʱ��ʹ�� */

	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms */
	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ�� */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;     /* ����ʱ�ӷָ�:TDTS = Tck_tim */
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;     	
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM���ϼ���ģʽ */
	TIM_TimeBaseInit( TIM1, &TIM_TimeBaseStructure );                       /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */

	TIM_ITConfig( TIM1, TIM_IT_Update, ENABLE );                            /* ʹ��ָ����TIM1�ж�,��������ж� */

	NVIC_InitStructure.NVIC_IRQChannel			= TIM1_UP_IRQn;    /* TIM1�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;            /* ��ռ���ȼ�0�� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 3;            /* �����ȼ�1�� */
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;       /* IRQͨ����ʹ�� */
	NVIC_Init( &NVIC_InitStructure );                                       /* ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� */

	TIM_Cmd( TIM1, ENABLE );                                                /* ʹ��TIMx���� */
}


/* ��ʱ��1�жϷ������ */
//void TIM1_UP_IRQHandler( void )                                                    /* TIM3�ж� */
//{
//	if ( TIM_GetITStatus( TIM1, TIM_IT_Update ) != RESET )                  /* ���ָ����TIM�жϷ������:TIM �ж�Դ */
//	{
//		
//		TIM_ClearITPendingBit( TIM1, TIM_IT_Update );                   /* ���TIMx���жϴ�����λ:TIM �ж�Դ */
//		//LED1 = !LED1;
//	}
//}


void TIM2_Int_Init( u16 arr, u16 psc )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );                  /* ʱ��ʹ�� */

	/* ��ʱ��TIM2��ʼ�� */
	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM���ϼ���ģʽ */
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );                       /* ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */

	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );
	/* ʹ��ָ����TIM2�ж�,��������ж� */

	/* �ж����ȼ�NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel			= TIM2_IRQn;    /* TIM2�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;            /* ��ռ���ȼ�0�� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 3;            /* �����ȼ�3�� */
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;       /* IRQͨ����ʹ�� */
	NVIC_Init( &NVIC_InitStructure );                                       /* ��ʼ��NVIC�Ĵ��� */


	 TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx 
}

//void TIM2_IRQHandler(void)   //TIM3�ж�
//{
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
//		{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
////		LED1=!LED1;
//		}
//}


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������
//void TIM3_IRQHandler(void)   //TIM3�ж�
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
//		{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
////		LED1=!LED1;
//		}
//}


void TIM4_Int_Init( u16 arr, u16 psc )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );                  /* ʱ��ʹ�� */

	/* ��ʱ��TIM4��ʼ�� */
	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 /* ����ʱ�ӷָ�:TDTS = Tck_tim */
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM���ϼ���ģʽ */
	TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure );                       /* ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */

	TIM_ITConfig( TIM4, TIM_IT_Update, ENABLE );
	/* ʹ��ָ����TIM4�ж�,��������ж� */

	/* �ж����ȼ�NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel			= TIM4_IRQn;    /* TIM4�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;            /* ��ռ���ȼ�0�� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 1;            /* �����ȼ�3�� */
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;       /* IRQͨ����ʹ�� */
	NVIC_Init( &NVIC_InitStructure );                                       /* ��ʼ��NVIC�Ĵ��� */


	 TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx 
}

//void TIM4_IRQHandler(void)   //TIM3�ж�
//{
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
//		{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
////		LED1=!LED1;
//		}
//}

//void TIM5_Int_Init( u16 arr, u16 psc )
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	NVIC_InitTypeDef	NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5, ENABLE );                  /* ʱ��ʹ�� */

//	/* ��ʱ��TIM5��ʼ�� */
//	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ */
//	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ */
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 /* ����ʱ�ӷָ�:TDTS = Tck_tim */
//	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM���ϼ���ģʽ */
//	TIM_TimeBaseInit( TIM5, &TIM_TimeBaseStructure );                       /* ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */

//	TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE );
//	/* ʹ��ָ����TIM5�ж�,��������ж� */

//	/* �ж����ȼ�NVIC���� */
//	NVIC_InitStructure.NVIC_IRQChannel			= TIM5_IRQn;    /* TIM5�ж� */
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;            /* ��ռ���ȼ�0�� */
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 3;            /* �����ȼ�3�� */
//	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;       /* IRQͨ����ʹ�� */
//	NVIC_Init( &NVIC_InitStructure );                                       /* ��ʼ��NVIC�Ĵ��� */


//	 TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx 
//}


///*
// * ͨ�ö�ʱ��6�жϳ�ʼ��
// * ����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
// * arr���Զ���װֵ��
// * psc��ʱ��Ԥ��Ƶ��
// * ����ʹ�õ��Ƕ�ʱ��3!
// */
//void TIM6_Int_Init( u16 arr, u16 psc )
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	NVIC_InitTypeDef	NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM6, ENABLE );                  /* ʱ��ʹ�� */

//	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms */
//	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ�� */
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            /* ����ʱ�ӷָ�:TDTS = Tck_tim */
//	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM���ϼ���ģʽ */
//	TIM_TimeBaseInit( TIM6, &TIM_TimeBaseStructure );                       /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */

//	TIM_ITConfig( TIM6, TIM_IT_Update, ENABLE );                            /* ʹ��ָ����TIM3�ж�,��������ж� */

//	NVIC_InitStructure.NVIC_IRQChannel			= TIM6_IRQn;    /* TIM3�ж� */
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;            /* ��ռ���ȼ�0�� */
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;            /* �����ȼ�2�� */
//	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;       /* IRQͨ����ʹ�� */
//	NVIC_Init( &NVIC_InitStructure );                                       /* ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� */

//	TIM_Cmd( TIM6, ENABLE );                                                /* ʹ��TIMx���� */
//}

///*
// * ͨ�ö�ʱ��7�жϳ�ʼ��
// * ����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
// * arr���Զ���װֵ��
// * psc��ʱ��Ԥ��Ƶ��
// * ����ʹ�õ��Ƕ�ʱ��3!
// */
//void TIM7_Int_Init( u16 arr, u16 psc )
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	NVIC_InitTypeDef	NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM7, ENABLE );                  /* ʱ��ʹ�� */

//	TIM_TimeBaseStructure.TIM_Period	= arr;                          /* ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms */
//	TIM_TimeBaseStructure.TIM_Prescaler	= psc;                          /* ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ�� */
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            /* ����ʱ�ӷָ�:TDTS = Tck_tim */
//	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;           /* TIM���ϼ���ģʽ */
//	TIM_TimeBaseInit( TIM7, &TIM_TimeBaseStructure );                       /* ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ */

//	TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE );                            /* ʹ��ָ����TIM3�ж�,��������ж� */

//	NVIC_InitStructure.NVIC_IRQChannel			= TIM7_IRQn;    /* TIM3�ж� */
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;            /* ��ռ���ȼ�0�� */
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 3;            /* �����ȼ�3�� */
//	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;       /* IRQͨ����ʹ�� */
//	NVIC_Init( &NVIC_InitStructure );                                       /* ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� */

//	TIM_Cmd( TIM7, ENABLE );                                                /* ʹ��TIMx���� */
//}



