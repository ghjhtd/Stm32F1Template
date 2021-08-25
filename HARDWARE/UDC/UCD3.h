
/*********************************************************************************
  *Copyright(C),2010-2011,Your Company
  *FileName:  UCD3
  *Author:  Eagle's Baby
  *Version:  //版本
  *Date:  Thu Jul 29 10:43:15 2021
  *Description:
		'Unitive Client Dock' for 3b253协议
		[Notice]: do not support 4b1021协议
  *Others:  //其他内容说明
  *Function List:  //主要函数列表，每条记录应包含函数名及功能简要说明
     1.…………
     2.…………
  *History:  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
     1.Date:
       Author:
       Modification:
     2.…………
  *请不要在该注释前面#include
**********************************************************************************/

#ifndef _UCD3_H
#define _UCD3_H

#include "stdint.h"
#include <stdio.h>

#define MPYPORT 0
#define PYPORT 1
#define CPORT 2
#define bufferSize 257 // UCD3的帧长度不允许超过256、不能低于3 + 1

#define DECODESKIP 0  // 0指自动开始， 1指固定跳过

typedef union 
{
    float f;
    uint8_t u8[4];
}fu8; 


void SetSkip(uint8_t _skip);

void InitBasicUCD3(void (*__uputc)(uint8_t ch), void (*_errHd)(char* chars), void (*_lostFn)(uint8_t *buffers, uint16_t length), void (*_intFn)(uint8_t command_id, uint32_t int_value), void (*_floatFn)(uint8_t command_id, float float_value), void (*_stringFn)(uint8_t command_id, uint8_t* str_value, uint8_t length));
void InitUCD3(void (*__uputc)(uint8_t ch), void (*_errHd)(char* chars), void (*_lostFn)(uint8_t *buffers, uint16_t length), void (*_intFn)(uint8_t command_id, uint32_t int_value), void (*_floatFn)(uint8_t command_id, float float_value), void (*_stringFn)(uint8_t command_id, uint8_t* str_value, uint8_t length), void (*_intsFn)(uint8_t command_id, uint32_t *ints_value, uint8_t length), void (*_floatsFn)(uint8_t command_id, float *floats_value, uint8_t length), void (*_compoundFn)(uint8_t command_id, uint32_t *ints_value, float *floats_value, uint8_t length));
void CaptureChar(uint8_t updatedChar);
void HandleWithProtocol3b253(uint8_t *chars);

void EncodeInt(uint8_t command_id, uint32_t int_value, uint8_t intBitLength);
void EncodeFloat(uint8_t command_id, float float_value);
void EncodeString(uint8_t command_id, uint8_t *str_value, uint8_t length);
void EncodeInts(uint8_t command_id, uint32_t *ints_value, uint8_t length, uint8_t maxIntBitLength);
void EncodeFloats(uint8_t command_id, float *floats_value, uint8_t length);
void EncodeCompound(uint8_t command_id, uint32_t *ints_value, float *floats_value, uint8_t length, uint8_t maxIntBitLength);
void Send(uint8_t to_who);
#endif 
