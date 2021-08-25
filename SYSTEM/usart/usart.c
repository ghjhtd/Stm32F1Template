#include "usart.h"	  
#include <stdarg.h>

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
volatile u16 USART1_RX_STA=0; 
volatile u16 USART2_RX_STA=0;       //接收状态标记	  
volatile u16 USART3_RX_STA=0;       //接收状态标记	  
volatile u16 USART4_RX_STA=0;       //接收状态标记
volatile u16 USART5_RX_STA=0;       //接收状态标记

volatile u8 USART1_RX_BUF[USART1_REC_LEN];
volatile u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
volatile u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
volatile u8 USART4_RX_BUF[USART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
volatile u8 USART5_RX_BUF[USART5_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

volatile u8 USART1_TX_BUF[USART1_REC_LEN];
volatile u8 USART2_TX_BUF[USART2_REC_LEN]; //串口2发送缓冲



//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 

void uart1_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	//Usart1 NVIC 
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
}

void USART1_IRQHandler(void)                	//串口1中断服务程序,当接收到/r/n时,USART1_RX_STA最高位置1,并且不再接收
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		if((USART1_RX_STA&0x80)==0)//接收未完成
			{
			if(USART1_RX_STA&0x40)//接收到了0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
				else {USART1_RX_STA|=0x80;}	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x40;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3F]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
} 

void uart2_init(u32 bound){
  //GPIO????
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//??USART1,GPIOA??
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.9
   
  //USART1_RX	  GPIOA.10???
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
  GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.10  

  //Usart1 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
  
   //USART ?????

	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

  USART_Init(USART2, &USART_InitStructure); //?????1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//????????
  USART_Cmd(USART2, ENABLE);                    //????1 

}

void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		
		if((USART2_RX_STA&0x80)==0)//接收未完成
			{
			if(USART2_RX_STA&0x40)//接收到了0x0d
				{
				if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x80;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x40;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3F]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
} 


void uart3_init(u32 bound)
{  	 
	  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能UGPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	//USART3_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???

   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure);     //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
}
  
void USART3_IRQHandler(void)                	
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
		{
		Res =USART_ReceiveData(USART3);
		if((USART3_RX_STA&0x80)==0)
			{
			if(USART3_RX_STA&0x40)
				{
				if(Res!=0x0a)USART3_RX_STA=0;
				else USART3_RX_STA|=0x80;	
				}
			else 
				{	
				if(Res==0x0d)USART3_RX_STA|=0x40;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3F]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;
					}		 
				}
			}   		 
     } 
	 }
	
/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}
	
	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}
	
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
	    *ptr++ = '-';
	
	    /* Make the value positive. */
	    value *= -1;
	}
	
	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;
	
	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	
	/* Null terminate the string. */
	*ptr = 0;
	
	return string;

} /* NCL_Itoa */

/*
 * 函数名：USART_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART_printf( USART1, "\r\n %s \r\n", j );
 */
 
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
	{									  
	switch ( *++Data )
	{
		case 'r':							          //回车符
			USART_SendData(USARTx, 0x0d);
			Data ++;
		break;
		
		case 'n':							          //换行符
			USART_SendData(USARTx, 0x0a);	
			Data ++;
		break;
		
		default:
			Data ++;
		break;
	}			 
	}
	else if ( *Data == '%')
	{									  //
	switch ( *++Data )
	{				
		case 's':										  //字符串
			s = va_arg(ap, const char *);
	for ( ; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;
	
	case 'd':										//十进制
	d = va_arg(ap, int);
	itoa(d, buf, 10);
	for (s = buf; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
	Data++;
	break;
		 default:
				Data++;
		    break;
	}		 
	} // end of else if 
	else USART_SendData(USARTx, *Data++);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}	 
	 
void UsartSendByte(USART_TypeDef* USARTx,char byte)   //串口发送一个字节
{
        USART_SendData(USARTx, byte);        //通过库函数  发送数据
        while( USART_GetFlagStatus(USARTx,USART_FLAG_TC)!= SET);  
        //等待发送完成。   检测 USART_FLAG_TC 是否置1；    //见库函数 P359 介绍
 }

void UsartSendBuf(USART_TypeDef* USARTx,char *buf, u16 len)       //串口发送一个指定长度的字符串
{
	u16 i;
	for(i=0; i<len; i++)UsartSendByte(USARTx,*buf++);
}

void UsartSendStr(USART_TypeDef* USARTx, char *str)              //串口发送一个字符串
{
	u16 i,len;
	len = strlen(str);
	for(i=0; i<len; i++)UsartSendByte(USARTx,*str++);
}

u8* Usart_Read(u8 USARTx, u8 clear ){     //串口读取字符串，用于读取一串以/r/n结尾的字符串,clear=0,读取后不清除  clear=1,读取后清除
	u8 temp[USART1_REC_LEN]="";
	int t,len;
	switch(USARTx){
		case 1:
			if((USART1_RX_STA&0x80) != 0)//可以放在定时器中断里执行
			{
				len=USART1_RX_STA&0x3f;//得到此次接收到的数据长度
				for(t=0;t<len;t++)
				{
					temp[t]=USART1_RX_BUF[t];				
				}
				temp[t]='\0';
				if(clear != 0)
				USART1_RX_STA=0;
				return temp;
			}
			else
				return '\0';
			
		case 2:
			if(USART2_RX_STA&0x80)//可以放在定时器中断里执行
			{
				len=USART2_RX_STA&0x3f;//得到此次接收到的数据长度
				for(t=0;t<len;t++)
				{
					temp[t]=USART2_RX_BUF[t];				
				}
				temp[t]='\0';
				if(clear != 0)
				USART2_RX_STA=0;
				return temp;
			}
			else
				return '\0';
			
		case 3:
			if(USART3_RX_STA&0x80)//可以放在定时器中断里执行
			{
				len=USART3_RX_STA&0x3f;//得到此次接收到的数据长度
				for(t=0;t<len;t++)
				{
					temp[t]=USART3_RX_BUF[t];				
				}
				temp[t]='\0';
				if(clear != 0)
				USART3_RX_STA=0;
				return temp;
			}
			else
				return '\0';
		case 4:
			if(USART4_RX_STA&0x80)//可以放在定时器中断里执行
			{
				len=USART4_RX_STA&0x3f;//得到此次接收到的数据长度
				for(t=0;t<len;t++)
				{
					temp[t]=USART4_RX_BUF[t];				
				}
				temp[t]='\0';
				if(clear != 0)
				USART4_RX_STA=0;
				return temp;
			}
			else
				return '\0';
		case 5:
			if(USART5_RX_STA&0x80)//可以放在定时器中断里执行
			{
				len=USART5_RX_STA&0x3f;//得到此次接收到的数据长度
				for(t=0;t<len;t++)
				{
					temp[t]=USART5_RX_BUF[t];				
				}
				temp[t]='\0';
				if(clear != 0)
				USART5_RX_STA=0;
				return temp;
			}
			else
				return '\0';
			break;
		default:
			return '\0';
		
	}
}   

int USART_ReadInt(u8 USARTx,u8 first,u8 length,u8 clear){     //串口读取，从第first个字符串开始的length个整数   clear=0,读取后不清除  clear=1,读取后清除
	u8 temp[20];
	u8 temp_int[20];
	int t,len,k;
	strcpy(temp,Usart_Read(USARTx,clear));
	len = strlen(temp);
	if((first + length) > len) length = len - first;
	if(temp != "" && first <= length){
				for(t=0;t<len;t++)
				{
					if(t >= first && t < (first + length)){
						temp_int[t - first] = temp[t];
					}			
				}
				temp_int[length]='\0';
			  return atoi(temp_int);
			}
	else
		return NULL;
}

//串口读取，用于读取一串以/r/n结尾的从第first个字符串开始的length个整数 字符串 clear=0,读取后不清除  clear=1,读取后清除,空则返回NULL
u8* Usart_ReadChars(u8 USARTx,int first,int length,u8 clear){
	u8 temp[50];
	u8 temp_chars[50];
	int t,len,k;
	strcpy(temp,Usart_Read(USARTx,clear));
	len = strlen(temp);
	if((first + length) > len) length = len - first;
	if(temp != "" && first <= length){
				for(t=0;t<len;t++)
				{
					if(t >= first && t < (first + length)){
						temp_chars[t - first] = temp[t];
					}			
				}
				temp_chars[length]='\0';
			  return temp_chars;
			}
	else
		return '\0';
}
	 
//	 void uart4_init(u32 bound){
//  //GPIO????
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);	//??USART1,GPIOA??
//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	
//	//USART1_TX   GPIOA.9
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
//  GPIO_Init(GPIOC, &GPIO_InitStructure);//???GPIOA.9
//   
//  //USART1_RX	  GPIOA.10???
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
//  GPIO_Init(GPIOC, &GPIO_InitStructure);//???GPIOA.10  

//  //Usart1 NVIC ??
//  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
//	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
//  
//   //USART ?????

//	USART_InitStructure.USART_BaudRate = bound;//?????
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
//	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

//  USART_Init(UART4, &USART_InitStructure); //?????1
//  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//????????
//  USART_Cmd(UART4, ENABLE);                    //????1 
//	USART4_RX_STA=0;		//清零

//}

//void UART4_IRQHandler(void)                	//??1??????
//	{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//??SYSTEM_SUPPORT_OS??,?????OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
//		{
//		Res =USART_ReceiveData(UART4);
//			
//		if((USART4_RX_STA&0x8000)==0)//串口未接收完成
//			{
//			if(USART4_RX_STA&0x4000)//如果收到0x0d
//				{
//				if(Res!=0x0a)USART4_RX_STA=0;//如果未收到0x0a,则重新接收
//				else USART4_RX_STA|=0x8000;	//如果接收到0x0a则将第15位置1
//				}
//			else //如果未收到0X0D
//				{	
//				if(Res==0x0d)USART4_RX_STA|=0x4000;
//				else
//					{
//					USART4_RX_BUF[USART4_RX_STA&0X3FFF]=Res ;
//					USART4_RX_STA++;
//					if(USART4_RX_STA>(USART4_REC_LEN-1))USART4_RX_STA=0;//??????,??????	  
//					}		 
//				}
//			} 
//		}
//		
//	 }

	 
//	 	 void uart5_init(u32 bound){
//  //GPIO????
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	//??USART1,GPIOA??
//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	
//	//USART1_TX   GPIOA.9
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PA.9
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
//  GPIO_Init(GPIOC, &GPIO_InitStructure);//???GPIOA.9
//   
//  //USART1_RX	  GPIOA.10???
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PA10
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
//  GPIO_Init(GPIOD, &GPIO_InitStructure);//???GPIOA.10  

//  //Usart1 NVIC ??
//  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
//	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
//  
//   //USART ?????

//	USART_InitStructure.USART_BaudRate = bound;//?????
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
//	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

//  USART_Init(UART5, &USART_InitStructure); //?????1
//  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//????????
//  USART_Cmd(UART5, ENABLE);                    //????1 
//	USART5_RX_STA=0;		//清零

//}

//void UART5_IRQHandler(void)                	//??1??????
//	{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//??SYSTEM_SUPPORT_OS??,?????OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
//		{
//		Res =USART_ReceiveData(UART5);
//		if((USART5_RX_STA&0x8000)==0)//?????
//			{
//			if(USART5_RX_STA&0x4000)//????0x0d
//				{
//				if(Res!=0x0a)USART5_RX_STA=0;//????,????
//				else USART5_RX_STA|=0x8000;	//????? 
//				}
//			else //????0X0D
//				{	
//				if(Res==0x0d)USART5_RX_STA|=0x4000;
//				else
//					{
//					USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
//					USART5_RX_STA++;
//					if(USART5_RX_STA>(USART5_REC_LEN-1))USART5_RX_STA=0;//??????,??????	  
//					}		 
//				}
//			}   		 
//     } 
//}




