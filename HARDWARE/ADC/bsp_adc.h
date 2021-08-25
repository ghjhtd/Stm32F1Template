#ifndef __BSP_ADC_H
#define __BSP_ADC_H
#include "delay.h"
#include "sys.h"

#define Channel_Num  2			//ת��ͨ������Ŀ
#define Sample_Num  10			//ÿ��ͨ��ת���Ĵ���

extern vu16 ADC_ConvertedValue[Sample_Num][Channel_Num];

 /**************************************************************************
���510������д��adc������������ʵ��ADC������pidѭ��ģ����
**************************************************************************/

void ADC1_Config(void);      //ADC��ʼ����ͨ���������
void ADC_DMA_Config(void);  //DMA��ʼ������ADC����ת�����ֱֵ�ӱ����ڱ�����
u8 ADC_Key(u16 adc,u8 mode);      //adc������̼�⣬mode=0��֧������
uint16_t ReadADCAverageValue(uint16_t Channel);//��ȡת��ƽ��ֵ
#endif


