#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	

#define PWMA   TIM2->CCR3  //PA2
#define PWMA2   TIM1->CCR1  //PA8
#define PWMA1   TIM1->CCR2  //PA9

#define AIN2   PBout(15)
#define AIN1   PBout(14)
#define BIN1   PBout(13)
#define BIN2   PBout(12)

#define PWMB1   TIM1->CCR3  //PA10
#define PWMB2   TIM1->CCR4  //PA11
#define PWMB   TIM2->CCR4  //PA3

void Motor_Init(void);
void Set_Pwm(int moto1,int moto2);
int myabs(int a);
void Xianfu_Pwm(void);
void Turn_Off(float angle, float voltage);

//TIM_SetCompare1(TIMx,value);    //设置定时器x的通道1的占空比

//TIM_Cmd(TIM3,DISABLE);       //设置定时器3的预分频系数
//delay_ms(1);
//TIM_PrescalerConfig(TIM3,psc-1,TIM_PSCReloadMode_Immediate);  //立即装入
//TIM_Cmd(TIM3,ENABLE);

#endif
