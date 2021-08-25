#include "fft.h"
#include "table_fft.h"

//   long是32位
long fft_in[NPT]={0};         //定义输入数组
long fft_out[NPT/2];          //定义输出数组
long fft_fuzhi[NPT/2];          //幅值

/******************************************************************
 2 函数名称:InitBufInArray()
 3 函数功能:模拟采样数据，采样数据中包含3种频率正弦波(350Hz，8400Hz，18725Hz)
 4 参数说明:
 5 备    注:在lBufInArray数组中，每个数据的高16位存储采样数据的实部，
 6           低16位存储采样数据的虚部(总是为0)
 7 NPT是采样点数256，PI2是2π（即6.28318530717959），Fs是采样频率44800
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
函数名称:GetPowerMag()
函数功能:计算各次谐波幅值
参数说明:
备　　注:先将lBufOutArray分解成实部(X)和虚部(Y)，然后计算幅值(sqrt(X*X+Y*Y)

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


