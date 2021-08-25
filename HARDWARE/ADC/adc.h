#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
/**************************************************************************
 adc读取函数
**************************************************************************/
#define Battery_Ch 8		//ADC1 通道8
void Adc_Init(void);
u16 Get_Adc(u8 ch);
float Get_battery_volt(void);  
#endif 
