#include "usart.h"	  
#include <stdarg.h>

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
volatile u16 USART1_RX_STA=0; 
volatile u16 USART2_RX_STA=0;       //����״̬���	  
volatile u16 USART3_RX_STA=0;       //����״̬���	  
volatile u16 USART4_RX_STA=0;       //����״̬���
volatile u16 USART5_RX_STA=0;       //����״̬���

volatile u8 USART1_RX_BUF[USART1_REC_LEN];
volatile u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
volatile u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
volatile u8 USART4_RX_BUF[USART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
volatile u8 USART5_RX_BUF[USART5_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

volatile u8 USART1_TX_BUF[USART1_REC_LEN];
volatile u8 USART2_TX_BUF[USART2_REC_LEN]; //����2���ͻ���



//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
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
	//GPIO�˿�����
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
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}

void USART1_IRQHandler(void)                	//����1�жϷ������,�����յ�/r/nʱ,USART1_RX_STA���λ��1,���Ҳ��ٽ���
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if((USART1_RX_STA&0x80)==0)//����δ���
			{
			if(USART1_RX_STA&0x40)//���յ���0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else {USART1_RX_STA|=0x80;}	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x40;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3F]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
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

void USART2_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
		
		if((USART2_RX_STA&0x80)==0)//����δ���
			{
			if(USART2_RX_STA&0x40)//���յ���0x0d
				{
				if(Res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x80;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x40;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3F]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
} 


void uart3_init(u32 bound)
{  	 
	  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��UGPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��
	//USART3_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure);     //��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 
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
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
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
 * ��������USART_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART_printf( USART1, "\r\n this is a demo \r\n" );
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
	
	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
	{									  
	switch ( *++Data )
	{
		case 'r':							          //�س���
			USART_SendData(USARTx, 0x0d);
			Data ++;
		break;
		
		case 'n':							          //���з�
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
		case 's':										  //�ַ���
			s = va_arg(ap, const char *);
	for ( ; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;
	
	case 'd':										//ʮ����
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
	 
void UsartSendByte(USART_TypeDef* USARTx,char byte)   //���ڷ���һ���ֽ�
{
        USART_SendData(USARTx, byte);        //ͨ���⺯��  ��������
        while( USART_GetFlagStatus(USARTx,USART_FLAG_TC)!= SET);  
        //�ȴ�������ɡ�   ��� USART_FLAG_TC �Ƿ���1��    //���⺯�� P359 ����
 }

void UsartSendBuf(USART_TypeDef* USARTx,char *buf, u16 len)       //���ڷ���һ��ָ�����ȵ��ַ���
{
	u16 i;
	for(i=0; i<len; i++)UsartSendByte(USARTx,*buf++);
}

void UsartSendStr(USART_TypeDef* USARTx, char *str)              //���ڷ���һ���ַ���
{
	u16 i,len;
	len = strlen(str);
	for(i=0; i<len; i++)UsartSendByte(USARTx,*str++);
}

u8* Usart_Read(u8 USARTx, u8 clear ){     //���ڶ�ȡ�ַ��������ڶ�ȡһ����/r/n��β���ַ���,clear=0,��ȡ�����  clear=1,��ȡ�����
	u8 temp[USART1_REC_LEN]="";
	int t,len;
	switch(USARTx){
		case 1:
			if((USART1_RX_STA&0x80) != 0)//���Է��ڶ�ʱ���ж���ִ��
			{
				len=USART1_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���
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
			if(USART2_RX_STA&0x80)//���Է��ڶ�ʱ���ж���ִ��
			{
				len=USART2_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���
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
			if(USART3_RX_STA&0x80)//���Է��ڶ�ʱ���ж���ִ��
			{
				len=USART3_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���
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
			if(USART4_RX_STA&0x80)//���Է��ڶ�ʱ���ж���ִ��
			{
				len=USART4_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���
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
			if(USART5_RX_STA&0x80)//���Է��ڶ�ʱ���ж���ִ��
			{
				len=USART5_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���
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

int USART_ReadInt(u8 USARTx,u8 first,u8 length,u8 clear){     //���ڶ�ȡ���ӵ�first���ַ�����ʼ��length������   clear=0,��ȡ�����  clear=1,��ȡ�����
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

//���ڶ�ȡ�����ڶ�ȡһ����/r/n��β�Ĵӵ�first���ַ�����ʼ��length������ �ַ��� clear=0,��ȡ�����  clear=1,��ȡ�����,���򷵻�NULL
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
//	USART4_RX_STA=0;		//����

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
//		if((USART4_RX_STA&0x8000)==0)//����δ�������
//			{
//			if(USART4_RX_STA&0x4000)//����յ�0x0d
//				{
//				if(Res!=0x0a)USART4_RX_STA=0;//���δ�յ�0x0a,�����½���
//				else USART4_RX_STA|=0x8000;	//������յ�0x0a�򽫵�15λ��1
//				}
//			else //���δ�յ�0X0D
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
//	USART5_RX_STA=0;		//����

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




