#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 
/**************************************************************************
AB相编码器库
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(65535)   //103的定时器是16位 2的16次方最大是65536
void Encoder_Init_TIM1(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Encoder_Init_TIM5(void);

void TIM3_Encoder_Init(void);
void TIM4_Encoder_Init(void);

int Read_Encoder(u8 TIMX);
int Read_Encoder_NoClear(u8 TIMX);
	
void TIM1_IRQHandler(void);
void TIM2_IRQHandler(void);
//void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM5_IRQHandler(void);
#endif
