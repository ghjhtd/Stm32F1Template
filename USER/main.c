/**************************************************************************/
#include "sys.h"

/****************************全局变量*************************************/    
float Voltage;  															 //电池电压采样相关的变量
float pitch,roll,yaw; 								  			 //欧拉角(姿态角)
short aacx,aacy,aacz;													 //加速度传感器原始数据
short gyrox,gyroy,gyroz;											 //陀螺仪原始数据
float UltrasonicWave_Distance;                 //超声波测距
int   enconder_left,enconder_right,all;         		 //左右编码器的脉冲计数
int 	Moto1=0,Moto2=0;												 //计算出来的最终赋给电机的PWM
int 	Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //蓝牙遥控相关的变量
u8    flag_UltrasonicWave=0;									 //超声波是否超出安全距离标志位
u8    key=0;								 									 //按键的键值

int Position_Zero=10000,ZHONGZHI = 3085;      //位置中值和角度中值

short ax,ay,az;
volatile u32 time=0;          //系统运行时间
int Balance_Pwm,Velocity_Pwm,Turn_Pwm;

//float balance_UP_KP=687; 	 // 小车直立环PD参数
//float balance_UP_KD=3.2;
float balance_UP_KP=0; 	 // 小车直立环PD参数
float balance_UP_KD=0;

//float velocity_KP=-111;     // 小车速度环PI参数
//float velocity_KI=0.56;
float velocity_KP= 0;     // 小车速度环PI参数
float velocity_KI= 0;

float velocity_KP_Inc=1;     // 小车速度环PI参数增量
float velocity_KI_Inc= 0.01;
float balance_UP_KD_Inc=0.1;
float balance_UP_KP_Inc=1; 	 // 小车直立环PD参数增量

float Balance_KP=50,Balance_KD=244,Position_KP=25,Position_KD=600;  //PID系数

#define FLASH_SAVE_ADDR  0X0801FF80		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

volatile u8 time_count = 0;   //0.1ms计时开关
volatile u32 count_ms = 0;    //0.1ms计时数

/****************************变量*************************************/ 
//菜单：
#define Y_caidan 4
#define X_caidan 2
int program = 0 ,program_last = 0;  //哪个程序，上一个程序选项
int program1 = 0,program1_last = 0;          //子菜单程序选择
u8 key_bsp = 0;            //板上正常按键值
volatile u8 Adc_Key = 0;   //adc矩阵键盘键值
u8 PS2_KEY = 0;
u8 mode[Y_caidan][X_caidan];    //pid菜单变量
int x_caidan = 0,y_caidan = 0;
u8 shuzi_change=0;       //是否改变数字
float temp_pid[4];
float temp_inc[4];
u8 Is_Save = 1;   //是否保存设置
u8 Is_Save_last = 1;   //上一次设置
int k = 0,index = 0; //循环变量
long fft_temp[1024];
u32 wave_temp[128];  //波形显示变量
u8 which = 0;

u8 wave_x[1024],wave_y[1024];
/***********************************************************************/

List list;


int main(void)	
{ 
	delay_init();	    	           //=====延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级				
	uart1_init(115200);	           //=====串口1初始化
	LED_Init();                    //=====初始化与 LED 连接的IO	
	LED1 =1;
	LED0 =1;
	
//	KEY_Init();                    //=====初始化与按键连接的IO 非ADC按键，正常按键
//	USB_Init();										 //=====初始化与 USB 连接的IO	 检测是否在充电
//	Adc_Init();                    //=====初始化ADC   普通读取ADC 
	
///---------------------ADC_DMP初始化-----------------------------------------------------------------/		
  ADC1_Config();      //ADC初始化，通过软件触发，可配置adc中断和dma中断
	
//u8 ADC_Key(u16 adc,u8 mode);      //adc矩阵键盘检测，mode=0不支持连按
//uint16_t ReadADCAverageValue(uint16_t Channel);//读取转换平均值
///*****************************************************************************/	
	
///--------------------修改蓝牙的默认通信波特率以及蓝牙默认的名字---------------------------------*/	
//	uart3_init(9600);              //=====串口3初始化成能与蓝牙进行通信的波特率,蓝牙默认通信波特率9600
//	USART_printf(USART3,"AT\r\n");
//	USART_printf(USART3,"AT+NAMEcar\r\n");//发送蓝牙模块指令--设置名字为：BIGbalance
//	delay_ms(10);	
//	USART_printf(USART3,"AT\r\n");
//	delay_ms(10);	
//	USART_printf(USART3,"AT+PIN0000\r\n"); 		 //发送蓝牙模块指令,将波特率设置成115200
//	delay_ms(10);		
//	uart3_init(115200);            //=====初始化串口3
///*****************************************************************************/		

///--------------------ucd通讯协议---------------------------------*/	
//InitUCD3(&uputcU3, &whenLost, &whenError, &whenInt, &whenFloat, &whenString, &whenInts, &whenFloats, &whenCompound);
///*****************************************************************************/		

///------------------读取编码器计数值-------------------------------------------------------------/		
//	Encoder_Init_TIM1();           //=====初始化编码器1  PE9 11
//	Encoder_Init_TIM2();           //=====初始化编码器2  PA0 1
//	Encoder_Init_TIM3();           //=====初始化编码器3  PA6 7 
//	Encoder_Init_TIM4();           //=====初始化编码器4	PB6 7
//	Encoder_Init_TIM5();           //=====初始化编码器5
//	TIM3_Encoder_Init();                       //编码器获取脉冲数 PA6 7 
//  TIM4_Encoder_Init();                       //编码器获取脉冲数 PB6 7
//  encoder = Read_Encoder(u8 TIMX);   //读取编码器X的值 并清除计数
//  encoder = Read_Encoder_NoClear(u8 TIMX);   //读取编码器X的值 不清除计数
///*****************************************************************************/

///-----------------MPU6050陀螺仪设置--------------------------------------------------------------/			
//	delay_ms(100);
	MPU_Init();					    			 //=====初始化MPU6050
	delay_ms(100);
	DMP_Init();								 //=====初始化MPU6050的DMP模式	
//	Read_Kalman(u8 who);                     //卡尔曼滤波获取数据  0-pitch 1-roll 2-yaw
//  while(Read_DMP(&pitch,&roll,&yaw) != 0);   //获取DMP数据	
//	MPU6050_EXTI_Init();					 //=====MPU6050 5ms定时中断初始化
///*****************************************************************************/

//-----------------------OLED屏幕设置---------------------------------------------------------------/			
	OLED_Init();                   //=====OLED初始化
	OLED_Clear();									 //=====OLED清屏
	OLED_DisPlay_On();
	OLED_Refresh();
//*****************************************************************************/	

///-----------------------超声波测距-使用TIM3--------------------------------------------------------/		
//	Timer3_Init(4999,7199);	    	 //=====超声波定时器初始化 PWM频率=72000/(7200*5000)=2hz
//	UltrasonicWave_Configuration();//=====初始化超声波的硬件IO口
//	UltrasonicWave_StartMeasure();//======开始超声波测距
///*****************************************************************************/		

///-----------------------两轮pwm输出----------------------------------------------------------------/		
//	TIM2_PWM_Init(7199,0);   			 //=====初始化PWM 10KHZ,用于驱动电机。 PWM频率=72000/(7199+1)=10Khz
//	Motor_Init();									 //=====初始化与电机连接的硬件IO接口 
//	Set_Pwm(0,0);   //=====设置pwm值
///*****************************************************************************/

///----------------------定时器中断-------------------------------------------------------------------/		
//	TIM1_Int_Init(7199,49);           //5ms定时器中断
	TIM1_Int_Init(7199,0);           //0.1ms定时器中断
//	TIM2_Int_Init(u16 arr,u16 psc);
//	TIM3_Int_Init(u16 arr,u16 psc);
//	TIM4_Int_Init(u16 arr,u16 psc);

// //测量时间长短
//	count_ms = 0;
//	time_count = 1;		
//	//需要测量时间的代码放在此处	  
//	time_count = 0;	
///*****************************************************************************/

///---------------------PS2手柄初始化------------------------------------------------------------------/		
	PS2_Init();											//=====ps2驱动端口初始化
	PS2_Init();				
	delay_ms(200);
	PS2_SetInit();		 							//=====ps2配置初始化,配置“红绿灯模式”，并选择是否可以修改
	delay_ms(100);
	PS2_Vibration(0XFF,0XFF)	 ;    //手柄振动
///*****************************************************************************/

///----------------------Flash读取和保存值---0x08000000-0x0801FFFF-------------------------------------/	
	//读pid
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)&balance_UP_KP,2);
	STMFLASH_Read(FLASH_SAVE_ADDR + 32,(u16*)&balance_UP_KD,2);
	STMFLASH_Read(FLASH_SAVE_ADDR + 32*2,(u16*)&velocity_KP,2);
	STMFLASH_Read(FLASH_SAVE_ADDR + 32*3,(u16*)&velocity_KI,2);

//	//写入pid
//  STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&balance_UP_KP,2);
//	STMFLASH_Write(FLASH_SAVE_ADDR + 32,(u16*)&balance_UP_KD,2);
//	STMFLASH_Write(FLASH_SAVE_ADDR + 32*1,(u16*)&velocity_KP,2);
//	STMFLASH_Write(FLASH_SAVE_ADDR + 32*2,(u16*)&velocity_KI,2);
///*****************************************************************************/

///---------------------Minibalance上位机数据发送-------------------------------------------------------/		
//	DataScope_Get_Channel_Data(pitch, 1 );
//	DataScope_Get_Channel_Data(roll, 2 );
//	DataScope_Get_Channel_Data( yaw, 3 ); 
//	DataScope_Get_Channel_Data( sin(time) , 4 );   
//	DataScope_Get_Channel_Data(0, 5 );
//	DataScope_Get_Channel_Data(0 , 6 );
//	DataScope_Get_Channel_Data(0, 7 );
//	DataScope_Get_Channel_Data( 0, 8 ); 
//	DataScope_Get_Channel_Data(0, 9 );  
//	DataScope_Get_Channel_Data( 0 , 10);
//	DataScope_Send(USART1,4)
///*****************************************************************************/

  while(1)
		{
			OLED_Clear();
			OLED_Refresh();		
			while(program==0||(key_bsp != 2 && key != 5 && PS2_KEY != 13)) //------------------一级菜单	
				{
					key = ADC_Key(ADC_ConvertedValue[0][0],0);
					PS2_KEY=PS2_Keyscan(0);
//					key_bsp = KEY_Scan(0);
					if(key_bsp == 1 ||key == 2|| PS2_KEY == 1|| PS2_KEY == 5) 
						{	
							program++;
							if(program>=10)
							program=1;		
						}	
					if(key == 8 || PS2_KEY == 7)
						{	
							program--;
							if(program<=0)
							program=9;							
						}
//					OLED_Clear();									 //=====OLED清屏
					OLED_ShowString( 0, 0, "Which program?", 12,1); 
					OLED_ShowNum(0,16,program,1,12,1);
					switch( program ){
						case 1:
							OLED_ShowString(8, 16, "Program1          ", 12,1); 
							break;
						case 2:
							OLED_ShowString(8, 16, "Program2          ", 12,1); 					
							break;
						case 3:
							OLED_ShowString(8, 16, "Program3          ", 12,1); 
							break;
						case 4:
							OLED_ShowString(8, 16, "Program4          ", 12,1); 
							break;
						case 5:
							OLED_ShowString(8, 16, "Wave_Test          ", 12,1); 
							break;                                    
						case 6:                                     
							OLED_ShowString(8, 16, "FFT_Test          ", 12,1); 
							break;				                            
						case 7:                                     
							OLED_ShowString(8, 16, "PID_Change          ", 12,1); 
							break;                                    
						case 8:                                     
							OLED_ShowString(8, 16, "DataScopeTest          ", 12,1); 
							break;                                    
						case 9:                                     
							OLED_ShowString(8, 16, "ADC_TEST          ", 12,1); 
							break;		
						default:
							OLED_ShowString(8, 16, "SELECT ONE", 12,1);
							OLED_Refresh();	
					}
					if(program_last != program)  //发生改变再刷屏
					OLED_Refresh();	
					program_last = program;
					delay_ms(50);              //这段延时必须加，ps2手柄需要
				}                                                           //------------------一级菜单	
		
		OLED_Clear();    //------------------选择后的显示
		OLED_ShowNum(0,0,program,1,12,0);
    OLED_ShowString(12, 0, "Program", 12,1); 
		OLED_Refresh();	
		delay_ms(1000);
		OLED_Clear();		//------------------选择后的显示
		
    switch (program) //++++++++++++++++++++++++++++++++++++++++++++一级菜单下的控制代码
			{
				case 1:  

					break;
							
				case 2:    

					break;
					
				case 3:   
					while(key != 3 && PS2_KEY != 15){
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							
						}	
						OLED_Clear();
						program = 0;
					break;
					
				case 4:  
					while(key != 3 && PS2_KEY != 15){
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							
						}	
						OLED_Clear();
						program = 0;
					break;
					
				case 5:    		
					while(key != 3 && PS2_KEY != 15){       //--------------------------------二级菜单
						while(program1==0||(key_bsp != 2 && key != 5 && PS2_KEY != 13))	
						{
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							if(key_bsp == 1 ||key == 2|| PS2_KEY == 1|| PS2_KEY == 5)
								{	
									program1++;
									if(program1>=10)
									program1=1;		
								}	
							if(key == 8 || PS2_KEY == 7)
								{	
									program1--;
									if(program1<=0)
									program1=9;							
								}
//					OLED_Clear();									 //=====OLED清屏
						OLED_ShowString( 0, 0, "Which wave?", 12,1); 
						OLED_ShowNum(0,16,program1,1,12,1);
						switch( program1 ){
							case 1:
								OLED_ShowString(8, 16, "Wave_Test          ", 12,1); 
								break;
							case 2:
								OLED_ShowString(8, 16, "Vector_Show         ", 12,1); 					
								break;
							case 3:
								OLED_ShowString(8, 16, "Rectangle_Show          ", 12,1); 
								break;
							case 4:
								OLED_ShowString(8, 16, "Program4          ", 12,1); 
								break;
							case 5:
								OLED_ShowString(8, 16, "Program5          ", 12,1); 
								break;                                    
							case 6:                                     
								OLED_ShowString(8, 16, "Program6          ", 12,1); 
								break;				                            
							case 7:                                     
								OLED_ShowString(8, 16, "Program7         ", 12,1); 
								break;                                    
							case 8:                                     
								OLED_ShowString(8, 16, "Program8          ", 12,1); 
								break;                                    
							case 9:                                     
								OLED_ShowString(8, 16, "Program9          ", 12,1); 
								break;		
							default:
								OLED_ShowString(8, 16, "SELECT ONE", 12,1);
								OLED_Refresh();	
						}                                              //--------------------------------二级菜单
						if(program1_last != program1)  //发生改变再刷屏
						OLED_Refresh();	
						program1_last = program1;
						delay_ms(50);              //这段延时必须加，ps2手柄需要
					}
					switch(program1){              //**************************************二级菜单控制代码
						case 1:    //显示fft波形
							InitBufInArray(fft_temp);
							while(key != 3 && PS2_KEY != 15){
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							for(k = 0;k<128;k++){
								wave_temp[k] = fft_temp[k+index] +1200;
							}
							index++;
							if(index >= 1024 -128) index = 0;
							LED0 =~LED0; 
							OLED_Clear();
							OLED_Show_Wave(wave_temp,2500);          //波形显示函数，max为波形显示的最大值
							}	
							OLED_Clear();
							program1 = 0;
							break;
						case 2:
							wave_x[0] = 0;
							wave_y[0] = 0;
							wave_x[1] = 127;
							wave_y[1] = 0;
							wave_x[2] = 0;
							wave_y[2] = 63;
							wave_x[3] = 127;
							wave_y[3] = 63;
							wave_x[4] = 63;
							wave_y[4] = 32;
							wave_x[5] = 0;
							wave_y[5] = 0;
							while(key != 3 && PS2_KEY != 15){
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							OLED_Clear();
							OLED_Show_Vector(wave_x,wave_y,6);
							}	
							OLED_Clear();
							program1 = 0;
							break;
						case 3:
							while(key != 3 && PS2_KEY != 15){
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							k++;
							if(k > 100)k = 0;
							OLED_Clear();
							OLED_Rectangle(k,10 ,20 ,20);
							delay_ms(100);
							}	
							OLED_Clear();
							program1 = 0;
							break;
						case 4:
							while(key != 3 && PS2_KEY != 15){
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							//控制程序
							delay_ms(100);
							}
							OLED_Clear();
							program1 = 0;
						case 5:
							
							break;                                    
						case 6:                                     
							
							break;				                            
						case 7:                                     
							
							break;                                    
						case 8:                                     
							
							break;                                    
						case 9:                                     
							
							break;		
					}				                  //**************************************二级菜单控制代码
						}	
						OLED_Clear();
						program1 = 0;
						program = 0;
					break;
						
				case 6:   		
					InitBufInArray(fft_temp);
					key = 0;
					while(key != 3 && PS2_KEY != 15){
						key = ADC_Key(ADC_ConvertedValue[0][0],0);
						PS2_KEY=PS2_Keyscan(0);				
						fft_1024(fft_temp);
//						printf("len:%d\r\n",sizeof(fft_fuzhi)/sizeof(long));
						if(PS2_KEY == 16|| key == 1){
							for(k = 0;k < 512;k++){
//								printf("freq:%d  Fuxhi:%d\r\n",k*100,fft_fuzhi[k]);
								delay_ms(20);
							}
						}
						if(PS2_KEY == 14 || key == 2){
							while(PS2_KEY != 15){
								key = ADC_Key(ADC_ConvertedValue[0][0],0);
								PS2_KEY=PS2_Keyscan(0);		
								for(k = 0;k < 1024;k++){
									DataScope_Get_Channel_Data((float)fft_temp[k],1);
									DataScope_Get_Channel_Data(1.0,2);
									DataScope_Get_Channel_Data(2.0,3); 
									DataScope_Get_Channel_Data(3.0,4);   
									DataScope_Send(USART1,4);
									delay_ms(100);
									delay_ms(100);
									delay_ms(100);
									delay_ms(100);
									delay_ms(100);
									delay_ms(100);
								}
								delay_ms(100);
							}							
						}
						OLED_ShowString(0, 0, "100:", 12,1); 
//						OLED_ShowNum(20,0,fft_fuzhi[1]/2,5,12,1);			
						OLED_ShowString(0, 14, "1000:", 12,1); 
//						OLED_ShowNum(20,14,fft_fuzhi[10],5,12,1);			
						OLED_ShowString(0, 28, "5000:", 12,1); 
//						OLED_ShowNum(20,28,fft_fuzhi[50],5,12,1);							
						OLED_Refresh();	
						delay_ms(50);
					}
					OLED_Clear();
					program = 0;
					break;
				case 7:  
					temp_pid[0] = balance_UP_KP;
					temp_pid[1] = balance_UP_KD;
					temp_pid[2] = velocity_KP;
					temp_pid[3] = velocity_KI;
				  temp_inc[0] = balance_UP_KP_Inc;
					temp_inc[1] = balance_UP_KD_Inc;
					temp_inc[2] = velocity_KP_Inc;
					temp_inc[3] = velocity_KI_Inc;
					while(1){
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							if(PS2_KEY == 1 || key == 5)shuzi_change = ~shuzi_change;    //SELECT 和3键 进行数字更改
						  if(PS2_KEY == 13 || key == 3)shuzi_change = ~shuzi_change;   //GREEN 和1键 进行菜单更改
						
							if(key == 2 || PS2_KEY == 5){         //向上
								if(shuzi_change == 0){
									y_caidan--;
								}
								else{
									if(x_caidan == 0){
										temp_pid[y_caidan] += temp_inc[y_caidan];
									}
									else{
										temp_inc[y_caidan] += 0.01;
									}
								}
							}
							
							if(key == 8 || PS2_KEY == 7){        //向下
								if(shuzi_change == 0){
									y_caidan++;
								}
								else{
									if(x_caidan == 0){
										temp_pid[y_caidan] -= temp_inc[y_caidan];
									}
									else{
										temp_inc[y_caidan] -= 0.01;
									}
								}
							}
							
							if(key == 6 || PS2_KEY == 6){       //向右
								if(shuzi_change == 0){
									x_caidan++;
								}
							}
							if(key == 4 || PS2_KEY == 8){      //向左
								if(shuzi_change == 0){
									x_caidan--;
								}
							}
							
							if(key == 3 || PS2_KEY == 15){      //退出	
								OLED_Clear();
								OLED_ShowString(0, 0, "Do you want to save?:", 12,1); 								
								if(Is_Save == 1){
										OLED_ShowString(0, 15, "YES:", 12,0); 
										OLED_ShowString(0, 30, "NO", 12,1); 
									}
									else{
										OLED_ShowString(0, 15, "YES:", 12,1); 
										OLED_ShowString(0, 30, "NO", 12,0);
									}
								OLED_Refresh();
								while(1){
									key = ADC_Key(ADC_ConvertedValue[0][0],0);
									PS2_KEY=PS2_Keyscan(0);
									if(key == 2 || PS2_KEY == 5){      //是
										Is_Save = 1;
									}
									if(key == 8 || PS2_KEY == 7){      //向左
										Is_Save = 0;
									}
									if(key == 5 || PS2_KEY == 1 || PS2_KEY == 13){      //确定
										if(Is_Save = 1){
												//写入pid
												STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&balance_UP_KP,2);
												STMFLASH_Write(FLASH_SAVE_ADDR + 32,(u16*)&balance_UP_KD,2);
												STMFLASH_Write(FLASH_SAVE_ADDR + 32*2,(u16*)&velocity_KP,2);
												STMFLASH_Write(FLASH_SAVE_ADDR + 32*3,(u16*)&velocity_KI,2);
												delay_ms(100);
										}
										OLED_Clear();
										program = 0;
										break;
									}
									if(Is_Save == 1){
										OLED_ShowString(0, 15, "YES", 12,0); 
										OLED_ShowString(0, 30, "NO", 12,1); 
									}
									else{
										OLED_ShowString(0, 15, "YES", 12,1); 
										OLED_ShowString(0, 30, "NO", 12,0);
									}
									delay_ms(50);
									if(Is_Save != Is_Save_last)OLED_Refresh();	//当发生改变时再刷屏
									Is_Save_last = Is_Save;
								}
							break;
							}
							
							//数值检查
							if(x_caidan > X_caidan - 1)x_caidan = 0;
							if(x_caidan < 0)x_caidan = X_caidan - 1;
							if(y_caidan > Y_caidan - 1)y_caidan = 0;
							if(y_caidan < 0)y_caidan = Y_caidan - 1;
							memset(mode,1,sizeof(mode));    //数组全部置0
							mode[y_caidan][x_caidan] = 0;   //选择单元格置0
							//pid赋值
							balance_UP_KP = temp_pid[0];
							balance_UP_KD = temp_pid[1];
							velocity_KP = temp_pid[2];
							velocity_KI = temp_pid[3];
							balance_UP_KP_Inc = temp_inc[0];
							balance_UP_KD_Inc = temp_inc[1];
							velocity_KP_Inc = temp_inc[2];
							velocity_KI_Inc = temp_inc[3]; 
							pid_show(mode,shuzi_change);
							delay_ms(50);
					}	
					OLED_Clear();
					program = 0;
					break;
					
				case 8:  
						while(key != 3 && PS2_KEY != 15){
							key = ADC_Key(ADC_ConvertedValue[0][0],0);
							PS2_KEY=PS2_Keyscan(0);
							DataScope_Get_Channel_Data(pitch,1);
							DataScope_Get_Channel_Data(roll,2);
							DataScope_Get_Channel_Data(yaw,3); 
							DataScope_Get_Channel_Data(sin(time/10000),4);   
							DataScope_Send(USART1,4);
							 //测量时间长短
							count_ms = 0;
							time_count = 1;		
							//需要测量时间的代码放在此处	
//							roll = Read_Kalman(1);
//							Read_DMP(&pitch,&roll,&yaw); 
							time_count = 0;	
							
							mpu6050_show_picture();
						}	
						OLED_Clear();
						program = 0;
					break;
						
				case 9:   
					while(key != 3 && PS2_KEY != 15){
						key = ADC_Key(ADC_ConvertedValue[0][0],0);
						PS2_KEY=PS2_Keyscan(0);
						OLED_Clear();
						OLED_ShowString(0, 0, "ADC0:", 12,1); 
						OLED_ShowNum(60,0,ReadADCAverageValue(0),4,12,1);			
						OLED_ShowNum(0,30,ADC_Key(ReadADCAverageValue(0),0),1,12,1);		
						OLED_ShowNum(30,24,ADC_ConvertedValue[0][1],4,12,1);						
						OLED_ShowString(0, 12, "ADC1", 12,1); 
						OLED_ShowNum(30,12,ReadADCAverageValue(1),4,12,1);							
						OLED_ShowNum(60,12,(ReadADCAverageValue(1)/4095.0)*3.3*4*100,4,12,1);	
//						printf("DC:%f  %d\r\n",(ReadADCAverageValue(1)/4095.0)*3.3*4,ReadADCAverageValue(1));
						OLED_Refresh();	
					}
					OLED_Clear();
					program = 0;
					break;
				}                              //++++++++++++++++++++++++++++++++++++++++++++一级菜单下的控制代码
		}			
}



