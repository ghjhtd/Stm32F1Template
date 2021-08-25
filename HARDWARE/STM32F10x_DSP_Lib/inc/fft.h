#ifndef __FFT_H
#define __FFT_H
#include "stm32_dsp.h"
#include "sys.h"

#define NPT 1024     //��������
#define Fs 102400			//����Ƶ��

extern long fft_in[NPT];         //������������
extern long fft_out[NPT/2];          //�����������
extern long fft_fuzhi[NPT/2];          //��ֵ����

void InitBufInArray(long *fft_temp);         //�����ã����ɲɼ�������
void GetPowerMag();
void fft_1024(long *pssIN);        //ÿ�����Ƶ�ʵ���FS/NPT*(��N����)
void fft_256(long *pssIN);        

#endif
