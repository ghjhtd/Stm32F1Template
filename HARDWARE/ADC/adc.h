#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
/**************************************************************************
 adc��ȡ����
**************************************************************************/
#define Battery_Ch 8		//ADC1 ͨ��8
void Adc_Init(void);
u16 Get_Adc(u8 ch);
float Get_battery_volt(void);  
#endif 
