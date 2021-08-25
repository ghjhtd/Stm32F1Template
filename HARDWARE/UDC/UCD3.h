
/*********************************************************************************
  *Copyright(C),2010-2011,Your Company
  *FileName:  UCD3
  *Author:  Eagle's Baby
  *Version:  //�汾
  *Date:  Thu Jul 29 10:43:15 2021
  *Description:
		'Unitive Client Dock' for 3b253Э��
		[Notice]: do not support 4b1021Э��
  *Others:  //��������˵��
  *Function List:  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
     1.��������
     2.��������
  *History:  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
     1.Date:
       Author:
       Modification:
     2.��������
  *�벻Ҫ�ڸ�ע��ǰ��#include
**********************************************************************************/

#ifndef _UCD3_H
#define _UCD3_H

#include "stdint.h"
#include <stdio.h>

#define MPYPORT 0
#define PYPORT 1
#define CPORT 2
#define bufferSize 257 // UCD3��֡���Ȳ�������256�����ܵ���3 + 1

#define DECODESKIP 0  // 0ָ�Զ���ʼ�� 1ָ�̶�����

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
