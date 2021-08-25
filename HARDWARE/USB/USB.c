#include "usb.h"


/**************************************************************************
函数功能：USB的IO口初始化
入口参数：无 
返回  值：无
**************************************************************************/
void USB_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	            //端口配置  usb检测io
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;         //下拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
} 

/**************************************************************************
函数功能：检查USB是否插入
入口参数：无 
返回  值：无
目		的：当检查USB插入时，进入充电模式。可把电机关闭掉。
					检测USB的IO口电平变化可由电路原理图得到，参考资料中的电路图
**************************************************************************/
void USB_TEST()
{
		if(USB_TEST_IO==1) //插入	
		{
			Moto1=0;
			Moto2=0;
		}
		if(USB_TEST_IO==0) //拔出	
		{

		}
}
