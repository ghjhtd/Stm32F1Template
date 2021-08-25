#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"

//------------------oled显示
void enconder_show(void);
void mpu6050_show(void);
void time_show(void);
void pid_show(u8 mode[4][2],int mode1);
void mpu6050_show_picture(void);


void OLED_Num_Next(u8* px,u8* py,int num,u8 len,u8 size1,u8*len_last,u8*size_last,u8 mode);//oled同行显示有符号整数
void OLED_Num_Newline(u8* px,u8* py,int num,u8 len,u8 size1,u8*len_last,u8*size_last,u8 mode);//oled换行显示有符号整数
void OLED_Str_Next(u8* px,u8* py,char* chr,u8 size1,u8*len_last,u8*size_last,u8 mode);//oled同行显示字符串
void OLED_Str_Newline(u8* px,u8* py,char* chr,u8 size1,u8*len_last,u8*size_last,u8 mode);//oled换行显示字符串
void OLED_Flo_Next(u8* px,u8* py,float num,u8 z_len,u8 f_len,u8 size1,u8*len_last,u8*size_last,u8 mode); //oled同行显示有符号小数
void OLED_Flo_Newline(u8* px,u8* py,float num,u8 z_len,u8 f_len,u8 size1,u8*len_last,u8*size_last,u8 mode);//oled换行显示有符号小数
void OLED_Showfloat(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2,u8 mode);    //z_len为整数显示位数，f_len为小数显示位数，size2为字体大小
u32 oled_pow(u8 m,u8 n);
void OLED_Show_Wave(u32* data,u32 max);          //波形显示函数，max为波形显示的最大值 传入值为一个数组 y轴显示：0-max  x轴：0-127
void OLED_Show_Vector(u8* x,u8* y,u16 length);        //矢量图形绘制函数  传入两个坐标数组和绘制的点数 max_x:x坐标的最大值  max_y：y坐标的最大值
void OLED_Rectangle(u8 x,u8 y ,u8 length ,u8 wide);          //矩形框绘制函数，x,y-矩形初始坐标  length-长 wide-宽

//-------------------DataScope发送
void Mpu6050_Send(void);
#endif
