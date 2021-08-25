#ifndef __BSP_ADC_H
#define __BSP_ADC_H
#include "delay.h"
#include "sys.h"

#define Channel_Num  2			//转换通道的数目
#define Sample_Num  10			//每个通道转换的次数

extern vu16 ADC_ConvertedValue[Sample_Num][Channel_Num];

 /**************************************************************************
针对510驱动板写的adc驱动函数，可实现ADC按键和pid循迹模块检测
**************************************************************************/

void ADC1_Config(void);      //ADC初始化，通过软件触发
void ADC_DMA_Config(void);  //DMA初始化，将ADC连续转换后的值直接保存在变量里
u8 ADC_Key(u16 adc,u8 mode);      //adc矩阵键盘检测，mode=0不支持连按
uint16_t ReadADCAverageValue(uint16_t Channel);//读取转换平均值
#endif


