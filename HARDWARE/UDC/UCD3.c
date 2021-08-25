
/*********************************************************************************
  *Copyright(C),2010-2011,Your Company
  *FileName:  UCD3
  *Author:  Eagle's Baby
  *Version:  //版本
  *Date:  Thu Jul 29 10:43:15 2021
  *Description:  //用于主要说明此程序文件完成的主要功能
                //与其他模块或函数的接口、输出值、取值范围、
                //含义及参数间的控制、顺序、独立及依赖关系
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

#include "UCD3.h"

// ---- Define Region ------------------------------------------------------Start---------------------------------------
// mode define:
#define __INT 0
#define __FLOAT 1
#define __STRING 2
#define __INTS 16
#define __FLOATS 17
#define __COMPOUND 33  // 16 + 17
// END mode define:

// maxSize define:  // 针对数组而言，定义数组的最大长度
#define _MAX_INT_LEN 15  // 255 / 16
#define _MAX_FLOAT_LEN 15  // 255 / 17
#define _MAX_COMPOUND_LEN 7  // 255 / 33  // 这个表示对数(一对两对的对)
// END maxSize define:

uint8_t skip = 0;
void SetSkip(uint8_t _skip){skip = _skip;}

uint8_t buffer[bufferSize], readLength = 3, writeBuffer[bufferSize], writeLength = 3; 

uint8_t isBasic = 0;
void (*_uputc)(uint8_t ch), (*intFn)(uint8_t command_id, uint32_t int_value), (*floatFn)(uint8_t command_id, float float_value), (*stringFn)(uint8_t command_id, uint8_t* str_value, uint8_t length), (*intsFn)(uint8_t command_id, uint32_t *ints_value, uint8_t length), (*floatsFn)(uint8_t command_id, float *floats_value, uint8_t length), (*compoundFn)(uint8_t command_id, uint32_t *ints_value, float *floats_value, uint8_t length);

void (*errHd)(char* chars), (*lostFn)(uint8_t *buffers, uint16_t length);

uint32_t ints_buffer[_MAX_INT_LEN];
float floats_buffer[_MAX_FLOAT_LEN];
void HandleWithProtocol3b253(uint8_t *buf);  // 解析字符串，并自动调用

void PackInt(uint32_t int_value, uint8_t *_writeBuffer, uint8_t start, uint8_t end);
void PackFloat(float float_value, uint8_t *_writeBuffer, uint8_t start, uint8_t end);

uint32_t UnpackInt(uint8_t *_buffer, uint8_t start, uint8_t end);
float UnpackFloat(uint8_t *_buffer, uint8_t start, uint8_t end);

// ---- Define Region ------------------------------------------------------End---------------------------------------


// ---- Function Region ------------------------------------------------------Start---------------------------------------
void InitUCD3(void (*__uputc)(uint8_t ch), void (*_errHd)(char* chars), void (*_lostFn)(uint8_t *buffers, uint16_t length), void (*_intFn)(uint8_t command_id, uint32_t int_value), void (*_floatFn)(uint8_t command_id, float float_value), void (*_stringFn)(uint8_t command_id, uint8_t* str_value, uint8_t length), void (*_intsFn)(uint8_t command_id, uint32_t *ints_value, uint8_t length), void (*_floatsFn)(uint8_t command_id, float *floats_value, uint8_t length), void (*_compoundFn)(uint8_t command_id, uint32_t *ints_value, float *floats_value, uint8_t length))
{
	_uputc = __uputc;
	errHd = _errHd;
	lostFn = _lostFn;
	intFn = _intFn;
	floatFn = _floatFn;
	stringFn = _stringFn;
	intsFn = _intsFn;
	floatsFn = _floatsFn;
	compoundFn = _compoundFn;
}

void InitBasicUCD3(void (*__uputc)(uint8_t ch), void (*_errHd)(char* chars), void (*_lostFn)(uint8_t *buffers, uint16_t length), void (*_intFn)(uint8_t command_id, uint32_t int_value), void (*_floatFn)(uint8_t command_id, float float_value), void (*_stringFn)(uint8_t command_id, uint8_t* str_value, uint8_t length))
{
	_uputc = __uputc;
	errHd = _errHd;
	lostFn = _lostFn;
	intFn = _intFn;
	floatFn = _floatFn;
	stringFn = _stringFn;
	isBasic = 1;
}

uint8_t bufferSta = 0;
uint8_t bufferIndex=0;
void CaptureChar(uint8_t updatedChar)  // 以\r\n结束
{
	if (bufferSta == 0 && updatedChar == '\r' && bufferIndex >= 3) bufferSta = 1;
	else if (bufferSta == 1)
	{
		bufferSta = 0;
		bufferIndex = 0;
		if (updatedChar == '\n'){
			HandleWithProtocol3b253(buffer);
		}
	}
	else
	{
		buffer[bufferIndex++] = updatedChar;
	}
	
	/*
	if (bufferIndex == 2)  // id type length
	{
		readLength = updatedChar;
	}
		
	if(bufferIndex >= readLength) //接收完成 // [Notice]: 这里的readLength不是读取长度，而是最大读取索引，即length - 1
	{
		//HandleWithProtocol3b253(buffer);
		for(bufferIndex=0;bufferIndex<=255;bufferIndex++)
		{
			printf("%c", buffer[bufferIndex]);
		}
		bufferIndex = 0;
		printf("\r\n");
	} 
	*/
}

uint8_t step, div;
void HandleWithProtocol3b253(uint8_t *chars)
{
	uint8_t i = 0;
		
	uint8_t length, cmd_id, type;  // [Notice]: 0-255 -> 1-256
	
	if (DECODESKIP == 0){
		while (1)
		{
			if(chars[i++] > 0) break;
		}
		chars = chars + i - 1;
	}else chars += skip;
	i = 0;
	
	
	length = chars[2] + 2, cmd_id = chars[0], type = chars[1];
	/*
	for(i=0;i<=chars[2];i++)
	{
		printf("%d ", chars[i]);
	}
	printf("\r\n");
	
	printf("%d, %d, %d   -- ", chars[0], chars[1], length);*/
	if (chars[--length] != (chars[0] + chars[1] + chars[2]) / 3)//
	{
		errHd("丢包/r/n");
		return;
	}
	if (type <= __STRING)
	{
		if (type == __INT) intFn(cmd_id, UnpackInt(chars, 3, length));
		else if (type == __FLOAT) floatFn(cmd_id, UnpackFloat(chars, 3, length));
		else if (type == __STRING) stringFn(cmd_id, &chars[3], length - 3);
	}
	else
	{
		if (type % __INTS == 0)
		{
			div = type / __INTS;
			
			step = (length - 3) / div;
			for (i = 0;i < div;i++)  // 0 1 2为非数据位
			{
				ints_buffer[i] = UnpackInt(chars, 3 + i * step, 3 + i * step + step);
			}
			intsFn(cmd_id, ints_buffer, div);
		}
		else if (type % __FLOATS == 0)
		{
			div = type / __FLOATS;
			
			step = (length - 3) / div;
			if (step != 4){ errHd("float数组的每个元素长度不为4.");return;}
			for (i = 0;i < div;i++)  // 0 1 2为非数据位
			{
				floats_buffer[i] = UnpackFloat(chars, 3 + i * step, 3 + i * step + step);
			}
			floatsFn(cmd_id, floats_buffer, div);
		}
		else if (type % __COMPOUND == 0)
		{
			div = type / __COMPOUND;
			
			step = (length - 3) / div;
			for (i = 0;i < div;i++)  // 0 1 2为非数据位
			{
				ints_buffer[i] = UnpackInt(chars, 3 + i * step, 3 + i * step + step - 4);
				floats_buffer[i] = UnpackFloat(chars, 3 + i * step + step - 4, 3 + i * step + step);
			}
			compoundFn(cmd_id, ints_buffer, floats_buffer, div);
		}
		else 
		{
			errHd("未知的数据类型.");
		}
	}

}

void PackInt(uint32_t int_value, uint8_t *_writeBuffer, uint8_t start, uint8_t end)
{
	uint8_t _bia = 0;
	while (_bia < 4 && (start + _bia) < end)
	{
		_writeBuffer[end - ++_bia] = int_value & 0xff;
		int_value >>= 8;
	}
}
void PackFloat(float float_value, uint8_t *_writeBuffer, uint8_t start, uint8_t end)
{
	fu8 tmp;
	tmp.f = float_value;
	_writeBuffer[start] = tmp.u8[0];
	_writeBuffer[start + 1] = tmp.u8[1];
	_writeBuffer[start + 2] = tmp.u8[2];
	_writeBuffer[start + 3] = tmp.u8[3];
}

void EncodeInt(uint8_t command_id, uint32_t int_value, uint8_t intBitLength)
{
	writeLength = 3 + intBitLength;
	
	writeBuffer[0] = command_id;
	writeBuffer[1] = __INT;
	writeBuffer[2] = writeLength - 1;
	PackInt(int_value, writeBuffer, 3, writeLength);
}

void EncodeFloat(uint8_t command_id, float float_value)
{
	writeLength = 3 + 4;
	
	writeBuffer[0] = command_id;
	writeBuffer[1] = __FLOAT;
	writeBuffer[2] = writeLength - 1;
	PackFloat(float_value, writeBuffer, 3, writeLength);
}

void EncodeString(uint8_t command_id, uint8_t *str_value, uint8_t length)
{
	uint8_t i = 0;
	writeLength = 3 + length;
	
	writeBuffer[0] = command_id;
	writeBuffer[1] = __STRING;
	writeBuffer[2] = writeLength - 1;
	
	if (length > 253) {errHd("字符串长度不能超过253");return;}
	for(;i<length;i++){
		writeBuffer[i + 3] = str_value[i];
	}
}

void EncodeInts(uint8_t command_id, uint32_t *ints_value, uint8_t length, uint8_t maxIntBitLength)
{
	uint8_t i = 0;
	
	if (length > _MAX_INT_LEN){errHd("整形数组长度过长.");return;}
	writeLength = 3 + maxIntBitLength * length;
	
	writeBuffer[0] = command_id;
	writeBuffer[1] = __INTS * length;
	writeBuffer[2] = writeLength - 1;
	for (;i < length; i++)
		PackInt(ints_value[i], writeBuffer, 3 + i * maxIntBitLength, 3 + i * maxIntBitLength + maxIntBitLength);
}

void EncodeFloats(uint8_t command_id, float *floats_value, uint8_t length)
{
	uint8_t i = 0;
	
	if (length > _MAX_FLOAT_LEN){errHd("浮点数组长度过长.");return;}
	writeLength = 3 + 4 * length;
	
	writeBuffer[0] = command_id;
	writeBuffer[1] = __FLOATS * length;
	writeBuffer[2] = writeLength - 1;
	for (;i < length; i++)
		PackFloat(floats_value[i], writeBuffer, 3 + i * 4, 3 + i * 4 + 4);
}

void EncodeCompound(uint8_t command_id, uint32_t *ints_value, float *floats_value, uint8_t length, uint8_t maxIntBitLength)
{
	uint8_t i = 0, step = maxIntBitLength + 4;
	
	if (length > _MAX_COMPOUND_LEN){errHd("Compound的 整数浮点数对 的长度过长.");return;}
	writeLength = 3 + step * length;
	
	writeBuffer[0] = command_id;
	writeBuffer[1] = __COMPOUND * length;
	writeBuffer[2] = writeLength - 1;
	for (;i < length; i++){
		PackInt(ints_value[i], writeBuffer, 3 + i * step, 3 + i * step + step - 4);
		PackFloat(floats_value[i], writeBuffer, 3 + i * step + step - 4, 3 + i * step + step);
	}
}

uint32_t UnpackInt(uint8_t *_buffer, uint8_t start, uint8_t end)
{
	uint8_t _bia = 0;
	uint32_t tmp = 0;
	while (1)
	{
		tmp |= _buffer[start + _bia++];
		if (_bia < 4 && (start + _bia) < end) tmp <<= 8;
		else break;
	}
	return tmp;
}

float UnpackFloat(uint8_t *_buffer, uint8_t start, uint8_t end)
{
	fu8 tmp;
	float result;
	tmp.u8[0] = _buffer[start];
	tmp.u8[1] = _buffer[start + 1];
	tmp.u8[2] = _buffer[start + 2];
	tmp.u8[3] = _buffer[start + 3];
	result = tmp.f;
	return result;
}

void Send(uint8_t to_who) 
{
	uint8_t i = 0;
	for(;i < writeLength; i++){
		//printf("%d ", writeBuffer[i]);
		_uputc(writeBuffer[i]);
	}
	_uputc((writeBuffer[0] + writeBuffer[1] + writeBuffer[2]) / 3);
	if (to_who == CPORT) {_uputc('\r');_uputc('\n');}
		
}

// ---- Function Region ------------------------------------------------------End---------------------------------------
