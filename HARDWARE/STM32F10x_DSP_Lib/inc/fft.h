#ifndef __FFT_H
#define __FFT_H
#include "stm32_dsp.h"
#include "sys.h"

#define NPT 1024     //采样点数
#define Fs 102400			//采样频率

extern long fft_in[NPT];         //定义输入数组
extern long fft_out[NPT/2];          //定义输出数组
extern long fft_fuzhi[NPT/2];          //幅值数组

void InitBufInArray(long *fft_temp);         //测试用，生成采集点数组
void GetPowerMag();
void fft_1024(long *pssIN);        //每个点的频率等于FS/NPT*(第N个点)
void fft_256(long *pssIN);        

#endif
