#ifndef __SHOW_H
#define __SHOW_H
#include "sys.h"

//------------------oled��ʾ
void enconder_show(void);
void mpu6050_show(void);
void time_show(void);
void pid_show(u8 mode[4][2],int mode1);
void mpu6050_show_picture(void);


void OLED_Num_Next(u8* px,u8* py,int num,u8 len,u8 size1,u8*len_last,u8*size_last,u8 mode);//oledͬ����ʾ�з�������
void OLED_Num_Newline(u8* px,u8* py,int num,u8 len,u8 size1,u8*len_last,u8*size_last,u8 mode);//oled������ʾ�з�������
void OLED_Str_Next(u8* px,u8* py,char* chr,u8 size1,u8*len_last,u8*size_last,u8 mode);//oledͬ����ʾ�ַ���
void OLED_Str_Newline(u8* px,u8* py,char* chr,u8 size1,u8*len_last,u8*size_last,u8 mode);//oled������ʾ�ַ���
void OLED_Flo_Next(u8* px,u8* py,float num,u8 z_len,u8 f_len,u8 size1,u8*len_last,u8*size_last,u8 mode); //oledͬ����ʾ�з���С��
void OLED_Flo_Newline(u8* px,u8* py,float num,u8 z_len,u8 f_len,u8 size1,u8*len_last,u8*size_last,u8 mode);//oled������ʾ�з���С��
void OLED_Showfloat(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2,u8 mode);    //z_lenΪ������ʾλ����f_lenΪС����ʾλ����size2Ϊ�����С
u32 oled_pow(u8 m,u8 n);
void OLED_Show_Wave(u32* data,u32 max);          //������ʾ������maxΪ������ʾ�����ֵ ����ֵΪһ������ y����ʾ��0-max  x�᣺0-127
void OLED_Show_Vector(u8* x,u8* y,u16 length);        //ʸ��ͼ�λ��ƺ���  ����������������ͻ��Ƶĵ��� max_x:x��������ֵ  max_y��y��������ֵ
void OLED_Rectangle(u8 x,u8 y ,u8 length ,u8 wide);          //���ο���ƺ�����x,y-���γ�ʼ����  length-�� wide-��

//-------------------DataScope����
void Mpu6050_Send(void);
#endif
