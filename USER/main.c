/**************************************************************************/
#include "sys.h"

/****************************ȫ�ֱ���*************************************/    
float Voltage;  															 //��ص�ѹ������صı���
float pitch,roll,yaw; 								  			 //ŷ����(��̬��)
short aacx,aacy,aacz;													 //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;											 //������ԭʼ����
float UltrasonicWave_Distance;                 //���������
int   enconder_left,enconder_right,all;         		 //���ұ��������������
int 	Moto1=0,Moto2=0;												 //������������ո��������PWM
int 	Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //����ң����صı���
u8    flag_UltrasonicWave=0;									 //�������Ƿ񳬳���ȫ�����־λ
u8    key=0;								 									 //�����ļ�ֵ

int Position_Zero=10000,ZHONGZHI = 3085;      //λ����ֵ�ͽǶ���ֵ

short ax,ay,az;
volatile u32 time=0;          //ϵͳ����ʱ��
int Balance_Pwm,Velocity_Pwm,Turn_Pwm;

//float balance_UP_KP=687; 	 // С��ֱ����PD����
//float balance_UP_KD=3.2;
float balance_UP_KP=0; 	 // С��ֱ����PD����
float balance_UP_KD=0;

//float velocity_KP=-111;     // С���ٶȻ�PI����
//float velocity_KI=0.56;
float velocity_KP= 0;     // С���ٶȻ�PI����
float velocity_KI= 0;

float velocity_KP_Inc=1;     // С���ٶȻ�PI��������
float velocity_KI_Inc= 0.01;
float balance_UP_KD_Inc=0.1;
float balance_UP_KP_Inc=1; 	 // С��ֱ����PD��������

float Balance_KP=50,Balance_KD=244,Position_KP=25,Position_KD=600;  //PIDϵ��

#define FLASH_SAVE_ADDR  0X0801FF80		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)

volatile u8 time_count = 0;   //0.1ms��ʱ����
volatile u32 count_ms = 0;    //0.1ms��ʱ��

/****************************����*************************************/ 
//�˵���
#define Y_caidan 4
#define X_caidan 2
int program = 0 ,program_last = 0;  //�ĸ�������һ������ѡ��
int program1 = 0,program1_last = 0;          //�Ӳ˵�����ѡ��
u8 key_bsp = 0;            //������������ֵ
volatile u8 Adc_Key = 0;   //adc������̼�ֵ
u8 PS2_KEY = 0;
u8 mode[Y_caidan][X_caidan];    //pid�˵�����
int x_caidan = 0,y_caidan = 0;
u8 shuzi_change=0;       //�Ƿ�ı�����
float temp_pid[4];
float temp_inc[4];
u8 Is_Save = 1;   //�Ƿ񱣴�����
u8 Is_Save_last = 1;   //��һ������
int k = 0,index = 0; //ѭ������
long fft_temp[1024];
u32 wave_temp[128];  //������ʾ����
u8 which = 0;

u8 wave_x[1024],wave_y[1024];
/***********************************************************************/

List list;


int main(void)	
{ 
	delay_init();	    	           //=====��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�				
	uart1_init(115200);	           //=====����1��ʼ��
	LED_Init();                    //=====��ʼ���� LED ���ӵ�IO	
	LED1 =1;
	LED0 =1;
	
//	KEY_Init();                    //=====��ʼ���밴�����ӵ�IO ��ADC��������������
//	USB_Init();										 //=====��ʼ���� USB ���ӵ�IO	 ����Ƿ��ڳ��
//	Adc_Init();                    //=====��ʼ��ADC   ��ͨ��ȡADC 
	
///---------------------ADC_DMP��ʼ��-----------------------------------------------------------------/		
  ADC1_Config();      //ADC��ʼ����ͨ�����������������adc�жϺ�dma�ж�
	
//u8 ADC_Key(u16 adc,u8 mode);      //adc������̼�⣬mode=0��֧������
//uint16_t ReadADCAverageValue(uint16_t Channel);//��ȡת��ƽ��ֵ
///*****************************************************************************/	
	
///--------------------�޸�������Ĭ��ͨ�Ų������Լ�����Ĭ�ϵ�����---------------------------------*/	
//	uart3_init(9600);              //=====����3��ʼ����������������ͨ�ŵĲ�����,����Ĭ��ͨ�Ų�����9600
//	USART_printf(USART3,"AT\r\n");
//	USART_printf(USART3,"AT+NAMEcar\r\n");//��������ģ��ָ��--��������Ϊ��BIGbalance
//	delay_ms(10);	
//	USART_printf(USART3,"AT\r\n");
//	delay_ms(10);	
//	USART_printf(USART3,"AT+PIN0000\r\n"); 		 //��������ģ��ָ��,�����������ó�115200
//	delay_ms(10);		
//	uart3_init(115200);            //=====��ʼ������3
///*****************************************************************************/		

///--------------------ucdͨѶЭ��---------------------------------*/	
//InitUCD3(&uputcU3, &whenLost, &whenError, &whenInt, &whenFloat, &whenString, &whenInts, &whenFloats, &whenCompound);
///*****************************************************************************/		

///------------------��ȡ����������ֵ-------------------------------------------------------------/		
//	Encoder_Init_TIM1();           //=====��ʼ��������1  PE9 11
//	Encoder_Init_TIM2();           //=====��ʼ��������2  PA0 1
//	Encoder_Init_TIM3();           //=====��ʼ��������3  PA6 7 
//	Encoder_Init_TIM4();           //=====��ʼ��������4	PB6 7
//	Encoder_Init_TIM5();           //=====��ʼ��������5
//	TIM3_Encoder_Init();                       //��������ȡ������ PA6 7 
//  TIM4_Encoder_Init();                       //��������ȡ������ PB6 7
//  encoder = Read_Encoder(u8 TIMX);   //��ȡ������X��ֵ ���������
//  encoder = Read_Encoder_NoClear(u8 TIMX);   //��ȡ������X��ֵ ���������
///*****************************************************************************/

///-----------------MPU6050����������--------------------------------------------------------------/			
//	delay_ms(100);
	MPU_Init();					    			 //=====��ʼ��MPU6050
	delay_ms(100);
	DMP_Init();								 //=====��ʼ��MPU6050��DMPģʽ	
//	Read_Kalman(u8 who);                     //�������˲���ȡ����  0-pitch 1-roll 2-yaw
//  while(Read_DMP(&pitch,&roll,&yaw) != 0);   //��ȡDMP����	
//	MPU6050_EXTI_Init();					 //=====MPU6050 5ms��ʱ�жϳ�ʼ��
///*****************************************************************************/

//-----------------------OLED��Ļ����---------------------------------------------------------------/			
	OLED_Init();                   //=====OLED��ʼ��
	OLED_Clear();									 //=====OLED����
	OLED_DisPlay_On();
	OLED_Refresh();
//*****************************************************************************/	

///-----------------------���������-ʹ��TIM3--------------------------------------------------------/		
//	Timer3_Init(4999,7199);	    	 //=====��������ʱ����ʼ�� PWMƵ��=72000/(7200*5000)=2hz
//	UltrasonicWave_Configuration();//=====��ʼ����������Ӳ��IO��
//	UltrasonicWave_StartMeasure();//======��ʼ���������
///*****************************************************************************/		

///-----------------------����pwm���----------------------------------------------------------------/		
//	TIM2_PWM_Init(7199,0);   			 //=====��ʼ��PWM 10KHZ,������������� PWMƵ��=72000/(7199+1)=10Khz
//	Motor_Init();									 //=====��ʼ���������ӵ�Ӳ��IO�ӿ� 
//	Set_Pwm(0,0);   //=====����pwmֵ
///*****************************************************************************/

///----------------------��ʱ���ж�-------------------------------------------------------------------/		
//	TIM1_Int_Init(7199,49);           //5ms��ʱ���ж�
	TIM1_Int_Init(7199,0);           //0.1ms��ʱ���ж�
//	TIM2_Int_Init(u16 arr,u16 psc);
//	TIM3_Int_Init(u16 arr,u16 psc);
//	TIM4_Int_Init(u16 arr,u16 psc);

// //����ʱ�䳤��
//	count_ms = 0;
//	time_count = 1;		
//	//��Ҫ����ʱ��Ĵ�����ڴ˴�	  
//	time_count = 0;	
///*****************************************************************************/

///---------------------PS2�ֱ���ʼ��------------------------------------------------------------------/		
	PS2_Init();											//=====ps2�����˿ڳ�ʼ��
	PS2_Init();				
	delay_ms(200);
	PS2_SetInit();		 							//=====ps2���ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	delay_ms(100);
	PS2_Vibration(0XFF,0XFF)	 ;    //�ֱ���
///*****************************************************************************/

///----------------------Flash��ȡ�ͱ���ֵ---0x08000000-0x0801FFFF-------------------------------------/	
	//��pid
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)&balance_UP_KP,2);
	STMFLASH_Read(FLASH_SAVE_ADDR + 32,(u16*)&balance_UP_KD,2);
	STMFLASH_Read(FLASH_SAVE_ADDR + 32*2,(u16*)&velocity_KP,2);
	STMFLASH_Read(FLASH_SAVE_ADDR + 32*3,(u16*)&velocity_KI,2);

//	//д��pid
//  STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&balance_UP_KP,2);
//	STMFLASH_Write(FLASH_SAVE_ADDR + 32,(u16*)&balance_UP_KD,2);
//	STMFLASH_Write(FLASH_SAVE_ADDR + 32*1,(u16*)&velocity_KP,2);
//	STMFLASH_Write(FLASH_SAVE_ADDR + 32*2,(u16*)&velocity_KI,2);
///*****************************************************************************/

///---------------------Minibalance��λ�����ݷ���-------------------------------------------------------/		
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
			while(program==0||(key_bsp != 2 && key != 5 && PS2_KEY != 13)) //------------------һ���˵�	
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
//					OLED_Clear();									 //=====OLED����
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
					if(program_last != program)  //�����ı���ˢ��
					OLED_Refresh();	
					program_last = program;
					delay_ms(50);              //�����ʱ����ӣ�ps2�ֱ���Ҫ
				}                                                           //------------------һ���˵�	
		
		OLED_Clear();    //------------------ѡ������ʾ
		OLED_ShowNum(0,0,program,1,12,0);
    OLED_ShowString(12, 0, "Program", 12,1); 
		OLED_Refresh();	
		delay_ms(1000);
		OLED_Clear();		//------------------ѡ������ʾ
		
    switch (program) //++++++++++++++++++++++++++++++++++++++++++++һ���˵��µĿ��ƴ���
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
					while(key != 3 && PS2_KEY != 15){       //--------------------------------�����˵�
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
//					OLED_Clear();									 //=====OLED����
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
						}                                              //--------------------------------�����˵�
						if(program1_last != program1)  //�����ı���ˢ��
						OLED_Refresh();	
						program1_last = program1;
						delay_ms(50);              //�����ʱ����ӣ�ps2�ֱ���Ҫ
					}
					switch(program1){              //**************************************�����˵����ƴ���
						case 1:    //��ʾfft����
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
							OLED_Show_Wave(wave_temp,2500);          //������ʾ������maxΪ������ʾ�����ֵ
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
							//���Ƴ���
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
					}				                  //**************************************�����˵����ƴ���
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
							if(PS2_KEY == 1 || key == 5)shuzi_change = ~shuzi_change;    //SELECT ��3�� �������ָ���
						  if(PS2_KEY == 13 || key == 3)shuzi_change = ~shuzi_change;   //GREEN ��1�� ���в˵�����
						
							if(key == 2 || PS2_KEY == 5){         //����
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
							
							if(key == 8 || PS2_KEY == 7){        //����
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
							
							if(key == 6 || PS2_KEY == 6){       //����
								if(shuzi_change == 0){
									x_caidan++;
								}
							}
							if(key == 4 || PS2_KEY == 8){      //����
								if(shuzi_change == 0){
									x_caidan--;
								}
							}
							
							if(key == 3 || PS2_KEY == 15){      //�˳�	
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
									if(key == 2 || PS2_KEY == 5){      //��
										Is_Save = 1;
									}
									if(key == 8 || PS2_KEY == 7){      //����
										Is_Save = 0;
									}
									if(key == 5 || PS2_KEY == 1 || PS2_KEY == 13){      //ȷ��
										if(Is_Save = 1){
												//д��pid
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
									if(Is_Save != Is_Save_last)OLED_Refresh();	//�������ı�ʱ��ˢ��
									Is_Save_last = Is_Save;
								}
							break;
							}
							
							//��ֵ���
							if(x_caidan > X_caidan - 1)x_caidan = 0;
							if(x_caidan < 0)x_caidan = X_caidan - 1;
							if(y_caidan > Y_caidan - 1)y_caidan = 0;
							if(y_caidan < 0)y_caidan = Y_caidan - 1;
							memset(mode,1,sizeof(mode));    //����ȫ����0
							mode[y_caidan][x_caidan] = 0;   //ѡ��Ԫ����0
							//pid��ֵ
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
							 //����ʱ�䳤��
							count_ms = 0;
							time_count = 1;		
							//��Ҫ����ʱ��Ĵ�����ڴ˴�	
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
				}                              //++++++++++++++++++++++++++++++++++++++++++++һ���˵��µĿ��ƴ���
		}			
}



