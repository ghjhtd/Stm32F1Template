#include "show.h"
#include "DataScope_DP.h"

 
 extern float Angle_Balance;
 extern int Encoder;
 
void enconder_show(void)
{
  u8 x=0,y=12;
	u8 size_last=0;
	u8 len_last=0;
	OLED_Clear();
	OLED_Str_Next(&x,&y,"en_left",12,&len_last,&size_last,1);
	OLED_Num_Next(&x,&y,enconder_left,5,12,&len_last,&size_last,1);
	OLED_Str_Newline(&x,&y,"en_right",12,&len_last,&size_last,1);
	OLED_Num_Next(&x,&y,enconder_right,5,12,&len_last,&size_last,1);
	OLED_Num_Newline(&x,&y,count_ms,5,12,&len_last,&size_last,1);
	OLED_Num_Newline(&x,&y,time/10000,12,12,&len_last,&size_last,1);
	OLED_Refresh();
}

void mpu6050_show(void)
{
  u8 x=0,y=12;
	u8 size_last=0;
	u8 len_last=0;
//	OLED_Clear();
	OLED_Str_Next(&x,&y,"pitch",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,pitch,3,2,12,&len_last,&size_last,1);
	OLED_Str_Newline(&x,&y,"roll",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,roll,3,2,12,&len_last,&size_last,1);
	OLED_Str_Newline(&x,&y,"yaw",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,yaw,3,2,12,&len_last,&size_last,1);
	OLED_Num_Newline(&x,&y,enconder_left,5,12,&len_last,&size_last,1);
	OLED_Num_Next(&x,&y,enconder_right,5,12,&len_last,&size_last,1);
	OLED_Num_Newline(&x,&y,count_ms,5,12,&len_last,&size_last,1);
//	OLED_Num_Newline(&x,&y,time/10000,12,12,&len_last,&size_last,1);
	OLED_Refresh();
}

void pid_show(u8 mode[4][2],int mode1)
{
  u8 x=0,y=0;
	u8 size_last=0;
	u8 len_last=0;
	
//	OLED_Clear();
	
	OLED_Str_Next(&x,&y,"B_P:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,balance_UP_KP,3,2,12,&len_last,&size_last,mode[0][0]);
//	OLED_Str_Next(&x,&y,"In:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,balance_UP_KP_Inc,2,2,12,&len_last,&size_last,mode[0][1]);
	
	OLED_Str_Newline(&x,&y,"B_D:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,balance_UP_KD,3,2,12,&len_last,&size_last,mode[1][0]);
//	OLED_Str_Next(&x,&y,"In:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,balance_UP_KD_Inc,2,2,12,&len_last,&size_last,mode[1][1]);
	
	OLED_Str_Newline(&x,&y,"V_P:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,velocity_KP,3,2,12,&len_last,&size_last,mode[2][0]);
//	OLED_Str_Next(&x,&y,"Inc:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,velocity_KP_Inc,2,2,12,&len_last,&size_last,mode[2][1]);
	
	OLED_Str_Newline(&x,&y,"V_I:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,velocity_KI,3,2,12,&len_last,&size_last,mode[3][0]);
//	OLED_Str_Next(&x,&y,"In:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,velocity_KI_Inc,2,2,12,&len_last,&size_last,mode[3][1]);
	
	if(mode1 == 0){OLED_Str_Newline(&x,&y,"Menu     ",12,&len_last,&size_last,1);}
	else {OLED_Str_Newline(&x,&y,"Number     ",12,&len_last,&size_last,1);}
	OLED_Refresh();
}

void time_show(void)
{
  OLED_ShowNum(67,0,count_ms,5,8,0);
	
	OLED_Refresh();
}

//void xy_show(void)
//{
//  u8 x=0,y=12;
//	u8 size_last=0;
//	u8 len_last=0;
//	OLED_Clear();
//	OLED_Str_Next(&x,&y,"X",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,X,5,12,&len_last,&size_last);
//	OLED_Str_Next(&x,&y,"Y",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,Y,5,12,&len_last,&size_last);
//	OLED_Str_Next(&x,&y,"temp",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,temp_Dot,5,12,&len_last,&size_last);
//	OLED_Refresh();
//}

//void xunji_show(void)
//{
//	u8 x=0,y=12;
//	u8 size_last=0;
//	u8 len_last=0;
//  OLED_Clear();
//	OLED_Str_Next(&x,&y,"Left",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,zuo,5,12,&len_last,&size_last);
//	OLED_Str_Next(&x,&y,"Right",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,you,5,12,&len_last,&size_last);
//	OLED_Str_Next(&x,&y,"Front",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,qian,5,12,&len_last,&size_last);
//	OLED_Str_Next(&x,&y,"tempY1",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,tempY1,5,12,&len_last,&size_last);

//	OLED_Str_Next(&x,&y,"temp",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,temp_Dot,5,12,&len_last,&size_last);
//	
////	OLED_Str_Next(&x,&y,"temp_Turn",12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,temp_Turn,5,12,&len_last,&size_last);
////	OLED_Str_Next(&x,&y,"temp_Sudu",12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,temp_Sudu,5,12,&len_last,&size_last);
////	OLED_Str_Newline(&x,&y,"right",12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,you,5,12,&len_last,&size_last);
////	OLED_Str_Newline(&x,&y,"qian",12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,qian,5,12,&len_last,&size_last);
////	OLED_Str_Newline(&x,&y,YOU,12,&len_last,&size_last);
////	OLED_Str_Newline(&x,&y,ZUO,12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,zuo,5,12,&len_last,&size_last);
////	OLED_Str_Newline(&x,&y,"ENC3",12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,Encoder3,3,12,&len_last,&size_last);
////	OLED_Str_Next(&x,&y,"ENC4",12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,Encoder4,3,12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,pwm1,4,12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,pwm2,4,12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,pwm3,4,12,&len_last,&size_last);
////	OLED_Num_Next(&x,&y,pwm4,4,12,&len_last,&size_last);
//	 OLED_Refresh();

////	
////	OLED_ShowString( 0, 0, "left:", 12,1); 
////	OLED_ShowNum(30,0,zuo,5,12,1);
////	OLED_ShowString( 0, 20, "Right:", 12,1); 
////	OLED_ShowNum(30,20,you,5,12,1);
////	OLED_Refresh();
//}

//void xunjiY_show(void)
//{
//	u8 x=0,y=12;
//	u8 size_last=0;
//	u8 len_last=0;
//  OLED_Clear();
//	OLED_Str_Next(&x,&y,"Left",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,zuo,5,12,&len_last,&size_last);
//	OLED_Str_Next(&x,&y,"Right",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,you,5,12,&len_last,&size_last);
//	OLED_Str_Next(&x,&y,"Front",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,qian,5,12,&len_last,&size_last);
//	OLED_Str_Next(&x,&y,"Mode",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,Mode,5,12,&len_last,&size_last);

//	OLED_Str_Next(&x,&y,"temp",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,temp_DotY,5,12,&len_last,&size_last);
//	
//	 OLED_Refresh();

//}

//void usart_show(void)
//{
//	u8 x=0,y=12;
//	u8 size_last=0;
//	u8 len_last=0;
//  OLED_Clear();
//	OLED_Str_Next(&x,&y,"UART1:",12,&len_last,&size_last);
//	OLED_Num_Next(&x,&y,uart_show,5,12,&len_last,&size_last);
//	 OLED_Refresh();

//}

//oled同行显示有符号整数
void OLED_Num_Next(u8* px,u8* py,int num,u8 len,u8 size1,u8*len_last,u8*size_last,u8 mode)
{
	 *px=*px+(*len_last)*(*size_last/2);
	 if(*px+(len*size1)>128)
	 {
		 *px=0;
		 (*py)+=*size_last;
	 }
	 if(num>=0)
	 {
		 OLED_ShowString(*px, *py, " ", size1,mode); 
		 OLED_ShowNum(*px+size1/2,*py,num,len,size1,mode);
	 }
	 else
	 {
		OLED_ShowString(*px, *py, "-", size1,mode); 
		 OLED_ShowNum(*px+size1/2,*py,-num,len,size1,mode);
	 }
	 *len_last=len+1;
	 *size_last=size1;
	 
}
//oled换行显示有符号整数
void OLED_Num_Newline(u8* px,u8* py,int num,u8 len,u8 size1,u8*len_last,u8*size_last,u8 mode)
{
	 *px=0;
	 (*py)+=*size_last;
	 if(num>=0)
	 {
		 OLED_ShowString(*px, *py, " ", size1,mode); 
		 OLED_ShowNum(*px+size1/2,*py,num,len,size1,mode);
	 }
	 else
	 {
		OLED_ShowString(*px, *py, "-", size1,mode); 
		 OLED_ShowNum(*px+size1/2,*py,-num,len,size1,mode);
	 }
	 *len_last=len+1;
	 *size_last=size1;
 }
//oled同行显示字符串
 void OLED_Str_Next(u8* px,u8* py,char* chr,u8 size1,u8*len_last,u8*size_last,u8 mode)
 {
	 *px=*px+(*len_last+1)*(*size_last/2);
	  if(*px+(strlen(chr)*size1)>128)
	 {
		 *px=0;
		 (*py)+=*size_last;
	 }
	 OLED_ShowString(*px, *py,(u8*)chr, size1,mode);
	 *len_last=strlen(chr);
	 *size_last=size1;
 }
//oled换行显示字符串
 void OLED_Str_Newline(u8* px,u8* py,char* chr,u8 size1,u8*len_last,u8*size_last,u8 mode)
 {
	  *px=0;
	  (*py)+=*size_last;
	 OLED_ShowString(*px, *py,(u8*)chr, size1,mode);
	 *len_last=strlen(chr);
	 *size_last=size1;
 }

 //z_len为整数显示位数，f_len为小数显示位数，size2为字体大小
void OLED_Showfloat(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2,u8 mode)
{         	
	u8 t,temp;
	u8 enshow;
	int z_temp,f_temp;      
	z_temp=(int)num;
	//整数部分
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2,mode);
				continue;
			}
			else
			enshow=1;
		}
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2,mode); 
	}
	//小数点
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2,mode); 
	
	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //小数部分
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2,mode); 
	}
}
 
//oled同行显示有符号小数
void OLED_Flo_Next(u8* px,u8* py,float num,u8 z_len,u8 f_len,u8 size1,u8*len_last,u8*size_last,u8 mode)
{
	u8 len = z_len + f_len+1;
	 *px=*px+(*len_last)*(*size_last/2);
	 if(*px+(len*size1)>128)
	 {
		 *px=0;
		 (*py)+=*size_last;
	 }
	 if(num>=0)
	 {
		 OLED_ShowString(*px, *py, " ", size1,mode);
		 OLED_Showfloat(*px+size1/2,*py,num,z_len,f_len,size1,mode);
	 }
	 else
	 {
		 OLED_ShowString(*px, *py, "-", size1,mode); 
		 OLED_Showfloat(*px+size1/2,*py,-num,z_len,f_len,size1,mode);
	 }
	 *len_last=len+1;
	 *size_last=size1;
	 
}
//oled换行显示有符号小数
void OLED_Flo_Newline(u8* px,u8* py,float num,u8 z_len,u8 f_len,u8 size1,u8*len_last,u8*size_last,u8 mode)
{
	u8 len = z_len + f_len+1;
	 *px=0;
	 (*py)+=*size_last;
	 if(num>=0)
	 {
		 OLED_ShowString(*px, *py, " ", size1,mode); 
		 OLED_Showfloat(*px+size1/2,*py,num,z_len,f_len,size1,mode);
	 }
	 else
	 {
		 OLED_ShowString(*px, *py, "-", size1,mode); 
		 OLED_Showfloat(*px+size1/2,*py,-num,z_len,f_len,size1,mode);
	 }
	 *len_last=len+1;
	 *size_last=size1;
 }

 u32 oled_pow(u8 m,u8 n)
{
u32 result=1;  
while(n--)result*=m;   
return result;
}

//波形显示函数，max为波形显示的最大值
void OLED_Show_Wave(u32* data,u32 max){
	int k;
//	OLED_Clear();
	for(k = 0;k < 128;k++){
		data[k] = ((float)data[k] / (float)max) * 63.0;
	}
	for(k = 0;k < 128;k++){
		OLED_DrawPoint(k,data[k],1);
	}
	OLED_Refresh();
}     

///**************************************************************************
//函数功能：虚拟示波器往上位机发送数据 关闭显示屏
//入口参数：无
//返回  值：无
//作    者：平衡小车之家
//**************************************************************************/
void Mpu6050_Send(void)
{   
		DataScope_Get_Channel_Data(pitch, 1 );
		DataScope_Get_Channel_Data(roll, 2 );
		DataScope_Get_Channel_Data( yaw, 3 ); 
		DataScope_Send(USART1,4);
}

//矢量图形绘制函数  传入两个坐标数组和绘制的点数 
void OLED_Show_Vector(u8* x,u8* y,u16 length){
	int k;
//	OLED_Clear();
	for(k = 0;k < length -1 ;k++){
		OLED_DrawLine(x[k],y[k],x[k+1],y[k+1],1);
	}
	OLED_Refresh();
}

 //矩形框绘制函数，x,y-矩形初始坐标  length-长 wide-宽
void OLED_Rectangle(u8 x,u8 y ,u8 length ,u8 wide){
	OLED_DrawLine(x,y,x+length,y,1);
	OLED_DrawLine(x+length,y,x+length,y+wide,1);
	OLED_DrawLine(x+length,y+wide,x,y+wide,1);
	OLED_DrawLine(x,y,x,y+wide,1);
	OLED_Refresh();
}         

void mpu6050_show_picture(void)
{
	u8 x=0,y=46;
	u8 size_last=0;
	u8 len_last=0;
	OLED_Clear();
	OLED_DrawCircle(22,25,20);
	if(myabs(pitch) <= 90)
		OLED_DrawLine(22,25,22 + 20 * sin(pitch / 180.0 * PI),25.0+20.0*cos(pitch / 180.0 * PI),1);  //pitch  pitch / 360.0 * PI
	else
		OLED_DrawLine(22 + 20 * sin(pitch / 180.0 * PI),25+20.0*cos(pitch / 180.0 * PI),22,25,1);  //pitch  pitch / 360.0 * PI
	OLED_DrawCircle(65,25,20);
	if(myabs(roll) <= 90)
		OLED_DrawLine(65,25,65 + 20 * sin(roll / 180.0 * PI),25.0+20.0*cos(roll / 180.0 * PI),1);  //roll
	else
		OLED_DrawLine(65 + 20 * sin(roll / 180.0 * PI),25+20.0*cos(roll / 180.0 * PI),65,25,1);  //roll
	OLED_DrawCircle(108,25,20);
	if(myabs(yaw) <= 90)
		OLED_DrawLine(108,25,108 + 20 * sin(yaw / 180.0 * PI),25.0+20.0*cos(yaw / 180.0 * PI),1);  //yaw
	else
		OLED_DrawLine(108 + 20 * sin(yaw / 180.0 * PI),25+20.0*cos(yaw / 180.0 * PI),108,25,1);  //yaw
	
	
//	OLED_Str_Next(&x,&y,"P:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,pitch,2,1,12,&len_last,&size_last,1);
//	OLED_Str_Next(&x,&y,"R:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,roll,3,1,12,&len_last,&size_last,1);
//	OLED_Str_Next(&x,&y,"Y:",12,&len_last,&size_last,1);
	OLED_Flo_Next(&x,&y,yaw,3,1,12,&len_last,&size_last,1);
	OLED_Refresh();
}

