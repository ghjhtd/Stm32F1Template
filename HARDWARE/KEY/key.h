#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//���������� ��������ʱ�޸�����Ķ���

#define KEY PCin(15)

void KEY_Init(void);          //������ʼ��
int KEY_Scan(void);          //����ɨ��
u8 Long_Press(void);         //�������
u8 click_N_Double (u8 time);  //����ɨ��-˫��


#endif  
