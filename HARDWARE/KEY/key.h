#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//按键检测程序 更换按键时修改下面的定义

#define KEY PCin(15)

void KEY_Init(void);          //按键初始化
int KEY_Scan(void);          //按键扫描
u8 Long_Press(void);         //长按检测
u8 click_N_Double (u8 time);  //按键扫描-双击


#endif  
