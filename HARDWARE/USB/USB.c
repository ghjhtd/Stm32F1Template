#include "usb.h"


/**************************************************************************
�������ܣ�USB��IO�ڳ�ʼ��
��ڲ������� 
����  ֵ����
**************************************************************************/
void USB_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	            //�˿�����  usb���io
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;         //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB
} 

/**************************************************************************
�������ܣ����USB�Ƿ����
��ڲ������� 
����  ֵ����
Ŀ		�ģ������USB����ʱ��������ģʽ���ɰѵ���رյ���
					���USB��IO�ڵ�ƽ�仯���ɵ�·ԭ��ͼ�õ����ο������еĵ�·ͼ
**************************************************************************/
void USB_TEST()
{
		if(USB_TEST_IO==1) //����	
		{
			Moto1=0;
			Moto2=0;
		}
		if(USB_TEST_IO==0) //�γ�	
		{

		}
}