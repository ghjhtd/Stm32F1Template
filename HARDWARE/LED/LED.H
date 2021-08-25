#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED 端口定义
#define LED0 PAout(4) // PA4
#define LED1 PAout(5) // PA5
#define LED2 PBout(4) // PB4
void LED_Init(void);  //初始化
void Led_Flash(u16 time);
#endif
