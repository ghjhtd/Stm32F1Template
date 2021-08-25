#include "fft.h"
#include "table_fft.h"

//   long��32λ
long fft_in[NPT]={0};         //������������
long fft_out[NPT/2];          //�����������
long fft_fuzhi[NPT/2];          //��ֵ

/******************************************************************
 2 ��������:InitBufInArray()
 3 ��������:ģ��������ݣ����������а���3��Ƶ�����Ҳ�(350Hz��8400Hz��18725Hz)
 4 ����˵��:
 5 ��    ע:��lBufInArray�����У�ÿ�����ݵĸ�16λ�洢�������ݵ�ʵ����
 6           ��16λ�洢�������ݵ��鲿(����Ϊ0)
 7 NPT�ǲ�������256��PI2��2�У���6.28318530717959����Fs�ǲ���Ƶ��44800
 8 *******************************************************************/
void InitBufInArray(long *fft_temp)
{
    unsigned short i;
    float fx;
    for(i=0; i<NPT; i++)
    {
        fx = 600 * sin(2*PI * i * 300.0 / Fs) +
             300 * sin(2*PI * i * 500.0 / Fs) +
             400 * sin(2*PI * i * 800.0 / Fs);
				fft_temp[i] = fx;
    }
}

/******************************************************************
��������:GetPowerMag()
��������:�������г����ֵ
����˵��:
������ע:�Ƚ�lBufOutArray�ֽ��ʵ��(X)���鲿(Y)��Ȼ������ֵ(sqrt(X*X+Y*Y)

*******************************************************************/
void GetPowerMag()
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (fft_out[i] << 16) >> 16;
        lY  = (fft_out[i] >> 16);
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            fft_fuzhi[i] = (unsigned long)(Mag * 32768);
        else
            fft_fuzhi[i] = (unsigned long)(Mag * 65536);
    }
}

void fft_1024(long *pssIN){      
	unsigned short i;
  for(i=0; i<1024; i++)
  {
      fft_in[i] = (pssIN[i]) << 16;
  }
	cr4_fft_1024_stm32(fft_out,fft_in, 1024);
	GetPowerMag(fft_out);
}

void fft_256(long *pssIN){      
	unsigned short i;
  for(i=0; i<256; i++)
  {
      fft_in[i] = (pssIN[i]) << 16;
  }
	cr4_fft_256_stm32(fft_out,fft_in,256);
	GetPowerMag(fft_out);
}


