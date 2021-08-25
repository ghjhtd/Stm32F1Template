#ifndef __PWM_H
#define __PWM_H


#include "sys.h"
#include "stm32f10x_tim.h" 

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);

#endif
