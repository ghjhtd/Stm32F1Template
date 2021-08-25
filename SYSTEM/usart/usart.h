#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART1_REC_LEN  			200  	//���ջ�������С
#define EN_USART1_RX 			1		//����ʹ��
#define USART2_REC_LEN  			200  	//���ջ�������С
#define EN_USART2_RX 			1		//����ʹ��
#define USART3_REC_LEN  			200  	//���ջ�������С
#define EN_USART3_RX 			1		//����ʹ��
#define USART4_REC_LEN  			200  	//���ջ�������С
#define EN_USART4_RX 			1		//����ʹ��
#define USART5_REC_LEN  			200  	//���ջ�������С
#define EN_USART5_RX 			1		//����ʹ��


extern volatile u8  USART1_RX_BUF[USART1_REC_LEN]; //���ڽ��ջ�����
extern volatile u16 USART1_RX_STA;         		//���ڽ��ձ�־

extern volatile u8  USART2_RX_BUF[USART2_REC_LEN]; //���ڽ��ջ�����
extern volatile u16 USART2_RX_STA;         		//���ڽ��ձ�־

extern volatile u8  USART3_RX_BUF[USART3_REC_LEN]; //���ڽ��ջ�����
extern volatile u16 USART3_RX_STA;             //���ڽ��ձ�־

extern volatile u8  USART4_RX_BUF[USART4_REC_LEN]; //���ڽ��ջ�����
extern volatile u16 USART4_RX_STA;    					//���ڽ��ձ�־

extern volatile u8  USART5_RX_BUF[USART5_REC_LEN]; //���ڽ��ջ�����
extern volatile u16 USART5_RX_STA;    					//���ڽ��ձ�־

extern volatile u8 USART1_TX_BUF[USART1_REC_LEN]; //���ڷ��ͻ�����
extern volatile u8 USART2_TX_BUF[USART2_REC_LEN]; //���ڷ��ͻ�����


void uart1_init(u32 bound);					//���ڳ�ʼ������
void uart2_init(u32 bound);
void uart3_init(u32 bound);
//void uart4_init(u32 bound);
//void uart5_init(u32 bound);

void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);   //��ʽ�������������C���е�printf��������û���õ�C��
u8* Usart_Read(u8 USARTx,u8 clear);   //���ڶ�ȡ�����ڶ�ȡһ����/r/n��β���ַ��� clear=0,��ȡ�����  clear=1,��ȡ�����,���򷵻�NULL
u8* Usart_ReadChars(u8 USARTx,int first,int length,u8 clear);   //���ڶ�ȡ�����ڶ�ȡһ����/r/n��β�Ĵӵ�first���ַ�����ʼ��length������ �ַ��� clear=0,��ȡ�����  clear=1,��ȡ�����,���򷵻�NULL
int USART_ReadInt(u8 USARTx,u8 first,u8 length,u8 clear);    //���ڶ�ȡ���ӵ�first���ַ�����ʼ��length������   clear=0,��ȡ�����  clear=1,��ȡ�����,���򷵻�NULL

void UsartSendByte(USART_TypeDef* USARTx,char byte);   //���ڷ���һ���ֽ�
void UsartSendBuf(USART_TypeDef* USARTx,char *buf, u16 len);       //���ڷ���һ��ָ�����ȵ��ַ���
void UsartSendStr(USART_TypeDef* USARTx, char *str);              //���ڷ���һ���ַ���

#endif

////���ڽ��ճ���ʾ��
//if(USART5_RX_STA&0x8000) //����5���ճ���
//{					   
//	len=USART5_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���

//	for(t=0;t<len;t++)
//	{
//		 YOU[t]=USART5_RX_BUF[t];				
//	}
//	YOU[t+1]='\0';
//	USART5_RX_STA=0;
//	you = -atoi(YOU);//����ȡ�����ַ�������ת��Ϊ��������
//}

////����4���ճ���
//if(USART4_RX_STA&0x8000)//���Է��ڶ�ʱ���ж���ִ��
//{
//	len=USART4_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���

//	for(t=0;t<len;t++)
//	{
//		 ZUO[t]=USART4_RX_BUF[t];				
//	}
//	ZUO[t+1]='\0';
//	USART4_RX_STA=0;
//  zuo = atoi(ZUO);//����ȡ�����ַ�������ת��Ϊ��������
//}

//if(USART3_RX_STA&0x8000)//���Է��ڶ�ʱ���ж���ִ��
//{
//	len=USART3_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//	for(t=0;t<len;t++)
//	{
//		 QIAN[t]=USART3_RX_BUF[t];				
//	}
//	QIAN[t+1]='\0';
//	USART3_RX_STA=0;
//  qian = atoi(QIAN);//����ȡ�����ַ�������ת��Ϊ��������
//}

