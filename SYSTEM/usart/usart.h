#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART1_REC_LEN  			200  	//接收缓冲区大小
#define EN_USART1_RX 			1		//接收使能
#define USART2_REC_LEN  			200  	//接收缓冲区大小
#define EN_USART2_RX 			1		//接收使能
#define USART3_REC_LEN  			200  	//接收缓冲区大小
#define EN_USART3_RX 			1		//接收使能
#define USART4_REC_LEN  			200  	//接收缓冲区大小
#define EN_USART4_RX 			1		//接收使能
#define USART5_REC_LEN  			200  	//接收缓冲区大小
#define EN_USART5_RX 			1		//接收使能


extern volatile u8  USART1_RX_BUF[USART1_REC_LEN]; //串口接收缓冲区
extern volatile u16 USART1_RX_STA;         		//串口接收标志

extern volatile u8  USART2_RX_BUF[USART2_REC_LEN]; //串口接收缓冲区
extern volatile u16 USART2_RX_STA;         		//串口接收标志

extern volatile u8  USART3_RX_BUF[USART3_REC_LEN]; //串口接收缓冲区
extern volatile u16 USART3_RX_STA;             //串口接收标志

extern volatile u8  USART4_RX_BUF[USART4_REC_LEN]; //串口接收缓冲区
extern volatile u16 USART4_RX_STA;    					//串口接收标志

extern volatile u8  USART5_RX_BUF[USART5_REC_LEN]; //串口接收缓冲区
extern volatile u16 USART5_RX_STA;    					//串口接收标志

extern volatile u8 USART1_TX_BUF[USART1_REC_LEN]; //串口发送缓冲区
extern volatile u8 USART2_TX_BUF[USART2_REC_LEN]; //串口发送缓冲区


void uart1_init(u32 bound);					//串口初始化函数
void uart2_init(u32 bound);
void uart3_init(u32 bound);
//void uart4_init(u32 bound);
//void uart5_init(u32 bound);

void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);   //格式化输出，类似于C库中的printf，但这里没有用到C库
u8* Usart_Read(u8 USARTx,u8 clear);   //串口读取，用于读取一串以/r/n结尾的字符串 clear=0,读取后不清除  clear=1,读取后清除,空则返回NULL
u8* Usart_ReadChars(u8 USARTx,int first,int length,u8 clear);   //串口读取，用于读取一串以/r/n结尾的从第first个字符串开始的length个整数 字符串 clear=0,读取后不清除  clear=1,读取后清除,空则返回NULL
int USART_ReadInt(u8 USARTx,u8 first,u8 length,u8 clear);    //串口读取，从第first个字符串开始的length个整数   clear=0,读取后不清除  clear=1,读取后清除,空则返回NULL

void UsartSendByte(USART_TypeDef* USARTx,char byte);   //串口发送一个字节
void UsartSendBuf(USART_TypeDef* USARTx,char *buf, u16 len);       //串口发送一个指定长度的字符串
void UsartSendStr(USART_TypeDef* USARTx, char *str);              //串口发送一个字符串

#endif

////串口接收程序示例
//if(USART5_RX_STA&0x8000) //串口5接收程序
//{					   
//	len=USART5_RX_STA&0x3fff;//得到此次接收到的数据长度

//	for(t=0;t<len;t++)
//	{
//		 YOU[t]=USART5_RX_BUF[t];				
//	}
//	YOU[t+1]='\0';
//	USART5_RX_STA=0;
//	you = -atoi(YOU);//将获取到的字符串数据转化为整形数据
//}

////串口4接收程序
//if(USART4_RX_STA&0x8000)//可以放在定时器中断里执行
//{
//	len=USART4_RX_STA&0x3fff;//得到此次接收到的数据长度

//	for(t=0;t<len;t++)
//	{
//		 ZUO[t]=USART4_RX_BUF[t];				
//	}
//	ZUO[t+1]='\0';
//	USART4_RX_STA=0;
//  zuo = atoi(ZUO);//将获取到的字符串数据转化为整形数据
//}

//if(USART3_RX_STA&0x8000)//可以放在定时器中断里执行
//{
//	len=USART3_RX_STA&0x3fff;//得到此次接收到的数据长度
//	for(t=0;t<len;t++)
//	{
//		 QIAN[t]=USART3_RX_BUF[t];				
//	}
//	QIAN[t+1]='\0';
//	USART3_RX_STA=0;
//  qian = atoi(QIAN);//将获取到的字符串数据转化为整形数据
//}

