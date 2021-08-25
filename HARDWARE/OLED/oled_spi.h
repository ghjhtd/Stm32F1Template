#ifndef __OLED_SPI_H
#define __OLED_SPI_H 

#include "sys.h"
#include "stdlib.h"	

//-----------------OLED�˿ڶ���---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_4)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_5)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_13)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOC,GPIO_Pin_13)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOC,GPIO_Pin_14)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOC,GPIO_Pin_14)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOC,GPIO_Pin_15)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOC,GPIO_Pin_15)


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

//void OLED_fuhao_write(unsigned char x,unsigned char y,unsigned char asc);
//void OLED_Num_write(unsigned char x,unsigned char y,unsigned char asc) ;
//void OLED_Float(unsigned char Y,unsigned char X,double real,unsigned char N);
//void OLED_Float2(unsigned char Y,unsigned char X,double real,unsigned char N1,unsigned char N2);
//void OLED_Num2(unsigned char x,unsigned char y, int number);
//void OLED_Num3(unsigned char x,unsigned char y,int number); 
//void OLED_Num4(unsigned char x,unsigned char y, int number);
//void OLED_Num5(unsigned char x,unsigned char y,unsigned int number);
//void oled_show(void);
//void oled_first_show(void);	
#endif
