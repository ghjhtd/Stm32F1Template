#include "control.h"	

float Mechanical_angle=0; 


u8 UltrasonicWave_Voltage_Counter=0;

/* ��ʱ��1�жϷ������ */
void TIM1_UP_IRQHandler( void )                         //��ʱ�������delay_ms����������������systick��ʱ�������¶�ʱʧ�ܣ���ò�Ҫ��
{
	if ( TIM_GetITStatus( TIM1, TIM_IT_Update ) != RESET )                  /* ���ָ����TIM�жϷ������:TIM �ж�Դ */
	{
		TIM_ClearITPendingBit( TIM1, TIM_IT_Update );                   /* ���TIMx���жϴ�����λ:TIM �ж�Դ */
		time++;

		if(time % 50 == 0){  //5ms��ʱ����
//			pitch = Read_Kalman(0);
//			roll = Read_Kalman(1);
//			yaw = Read_Kalman(2);
			LED0 = ~LED0;
//////			while(Read_DMP(&pitch,&roll,&yaw) != 0);
			Read_DMP(&pitch,&roll,&yaw);                          //��ʱ3ms���ң���10������ݺ���ʱ55ms���ң���ȡ�ٶ�̫��ʱ�ᷢ��FIFO���-����while��ȡ�����Ż����ȡ��ʱֻ��1.8ms����
//																														//��3.6ms�Ķ�ʱʱ�պ�5ms��ʱ
////			roll = Read_Kalman(1);
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								 //===�õ�����������
//			enconder_left = Read_Encoder(3);
//			enconder_right = Read_Encoder(4);
//			Balance_Pwm =balance_UP(roll,Mechanical_angle,gyrox);   //===ƽ�⻷PID����	
//			Velocity_Pwm=velocity(enconder_left,enconder_right);       //===�ٶȻ�PID����	 
//			if(1==Flag_Left||1==Flag_Right)    
//			Turn_Pwm =turn(enconder_left,enconder_right,gyroz);        //===ת��PID����
//			else Turn_Pwm=0.5*gyroz;
//			Moto1=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //===�������ֵ������PWM
//			Moto2=Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //===�������ֵ������PWM
//			Xianfu_Pwm();  																					 //===PWM�޷�
//			Turn_Off(roll,12.0);																 //===���Ƕ��Լ���ѹ�Ƿ�����
//			Set_Pwm(Moto1,Moto2);     			//===��ֵ��PWM�Ĵ���  	
		}
	
		if(time_count == 1){ //��һ��ʱ�䳤��
			count_ms++;
		}
	}
}

//void EXTI9_5_IRQHandler(void) 
//{    
//	if(PBin(5)==0)		
//	{		
//		EXTI->PR=1<<5;                                           //===���LINE5�ϵ��жϱ�־λ   
//		mpu_dmp_get_data(&pitch,&roll,&yaw);										 //===�õ�ŷ���ǣ���̬�ǣ�������
//		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								 //===�õ�����������
//		Encoder_Left=-Read_Encoder(2);                           //===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
//		Encoder_Right=Read_Encoder(4);                           //===��ȡ��������ֵ
//		UltrasonicWave_Voltage_Counter++;
//		if(UltrasonicWave_Voltage_Counter==10)									 //===100ms��ȡһ�γ������������Լ���ѹ
//		{
//			UltrasonicWave_Voltage_Counter=0;
//			Voltage=Get_battery_volt();		                         //===��ȡ��ص�ѹ		
//			UltrasonicWave_StartMeasure();												 //===��ȡ��������ֵ
//		}
//		Balance_Pwm =balance_UP(pitch,Mechanical_angle,gyroy);   //===ƽ�⻷PID����	
//		Velocity_Pwm=velocity(Encoder_Left,Encoder_Right);       //===�ٶȻ�PID����	 
//  	if(1==Flag_Left||1==Flag_Right)    
//		Turn_Pwm =turn(Encoder_Left,Encoder_Right,gyroz);        //===ת��PID����
//		else Turn_Pwm=0.5*gyroz;
//		Moto1=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //===�������ֵ������PWM
//		Moto2=Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //===�������ֵ������PWM
//	  Xianfu_Pwm();  																					 //===PWM�޷�
//		Turn_Off(pitch,Voltage);																 //===���Ƕ��Լ���ѹ�Ƿ�����
//		Set_Pwm(Moto1,Moto2);                                    //===��ֵ��PWM�Ĵ���  
//	}       	
//} 

/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ���еƽ��Ƕȣ���е��ֵ�������ٶ�
����  ֵ��ֱ������PWM
��    �ߣ��������
**************************************************************************/
int balance_UP(float Angle,float Mechanical_balance,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-Mechanical_balance;    							 //===���ƽ��ĽǶ���ֵ�ͻ�е���
	 balance=balance_UP_KP*Bias+balance_UP_KD*Gyro;  //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

/**************************************************************************
�������ܣ��ٶ�PI����
��ڲ����������������ֵ
����  ֵ���ٶȿ���PWM
��    �ߣ��������
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
		if(1==Flag_Qian)				
		{
			flag_UltrasonicWave=0;//����ǰ���Ƿ����ϰ���,����ң�����ȼ���ߡ�����־λ��0
			Movement=-150;
		}
    else if(1==Flag_Hou)//���յ�����APPң������		
		{
			flag_UltrasonicWave=0;//����ǰ���Ƿ����ϰ���,����ң�����ȼ���ߡ�����־λ��0
			Movement=150;//�趨�ٶ�
		}
		/*���������ľ������10cmʱ,��10cm�������ϰ���,����������־λ�ò��Ҹ�����ֵʹ�����,�������˸��򵥵�P��������*/
		//else if(UltrasonicWave_Distance<10&&UltrasonicWave_Distance>3)
		//{
		//	flag_UltrasonicWave=1;
		//	Movement=UltrasonicWave_Distance*50;		
		//}
		else//û�н��ܵ��κε�����
		{	
			flag_UltrasonicWave=0;
			Movement=0;
		}
   //=============�ٶ�PI������=======================//	
		Encoder_Least =(encoder_left+encoder_right)-0;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
		if(Encoder_Integral<-10000)		Encoder_Integral=-10000;            //===�����޷�	
		Velocity=Encoder*velocity_KP+Encoder_Integral*velocity_KI;        //===�ٶȿ���	
	  if(pitch<-40||pitch>40) 			Encoder_Integral=0;     						//===����رպ��������
		all = Encoder_Integral*velocity_KI;
	  return Velocity;
}
/**************************************************************************
�������ܣ�ת��PD����
��ڲ����������������ֵ��Z����ٶ�
����  ֵ��ת�����PWM
��    �ߣ��������
**************************************************************************/
int turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	  float Turn_Amplitude=100,Kp=20,Kd=0;     
	  //=============ң��������ת����=======================//
	  //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
  	if(1==Flag_Left||1==Flag_Right)                      
		{
			if(++Turn_Count==1)
			Encoder_temp=myabs(encoder_left+encoder_right);      
			Turn_Convert=55/Encoder_temp;
			if(Turn_Convert<0.6)Turn_Convert=0.6;
			if(Turn_Convert>3)Turn_Convert=3;
		}	
	  else
		{
			Turn_Convert=0.9;
			Turn_Count=0;
			Encoder_temp=0;
		}			
		if(1==Flag_Left)	           Turn_Target+=Turn_Convert;
		else if(1==Flag_Right)	     Turn_Target-=Turn_Convert; 
		else Turn_Target=0;
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת��	�ٶ��޷�
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
		else Kd=0;   //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
  	//=============ת��PD������=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}

/**************************************************************************
�������ܣ����PD����
��ڲ������Ƕ�
����  ֵ����ǿ���PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int balance(float Angle)
{  
   float Bias;                       //���ƫ��
	 static float Last_Bias,D_Bias;    //PID��ر���
	 int balance;                      //PWM����ֵ 
	 Bias=Angle-ZHONGZHI;              //���ƽ��ĽǶ���ֵ �ͻ�е���
	 D_Bias=Bias-Last_Bias;            //���ƫ���΢�� ����΢�ֿ���
	 balance=-Balance_KP*Bias-D_Bias*Balance_KD;   //===������ǿ��Ƶĵ��PWM  PD����
   Last_Bias=Bias;                   //������һ�ε�ƫ��
	 return balance;
}

/**************************************************************************
�������ܣ�λ��PD���� 
��ڲ�����������
����  ֵ��λ�ÿ���PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int Position(int Encoder)
{  
   static float Position_PWM,Last_Position,Position_Bias,Position_Differential;
	 static float Position_Least;
  	Position_Least =Encoder-Position_Zero;             //===
    Position_Bias *=0.8;		   
    Position_Bias += Position_Least*0.2;	             //===һ�׵�ͨ�˲���  
	  Position_Differential=Position_Bias-Last_Position;
	  Last_Position=Position_Bias;
		Position_PWM=Position_Bias*Position_KP+Position_Differential*Position_KD; //===�ٶȿ���	
	  return Position_PWM;
}

void uputcU3(u8 ch)
{
	UsartSendByte(USART2,ch);
}

void whenInt(u8 cmd, u32 int_value)
{
//	if(cmd == 1){
//		which_pid = int_value;
//		printf("which_pid:%d\r\n",which_pid);
//	}
//	if(cmd == 2){
//		which_pid_number = int_value;
//		printf("which_pid_number:%d\r\n",which_pid_number);
//	}
}


void whenFloat(u8 cmd, float float_value)
{
//	if(cmd == 3){
//		if(which_pid == 0){
//			switch(which_pid_number){
//				case 0:
//					Position_KP=float_value;
//				printf("Position_KP:%f\r\n",Position_KP);
//					break;
//				case 1:
//					Position_KI=float_value;
//				printf("Position_KI:%f\r\n",Position_KI);
//					break;
//				case 2:
//					Position_KD=float_value;
//				printf("Position_KD:%f\r\n",Position_KD);
//					break;
//			}
//		}
//		if(which_pid == 1){
//			switch(which_pid_number){
//				case 0:
//					Position_Kp=float_value;
//				printf("Position_Kp:%f\r\n",Position_Kp);
//					break;
//				case 1:
//					Position_Ki=float_value;
//				printf("Position_Ki:%f\r\n",Position_Ki);
//					break;
//				case 2:
//					Position_Kd=float_value;
//				printf("Position_Kd:%f\r\n",Position_Kd);
//					break;
//			}
//		}
//		if(which_pid == 2){
//			switch(which_pid_number){
//				case 0:
//					
//					break;
//				case 1:
//					
//					break;
//				case 2:
//					
//					break;
//			}
//		}
//		
//	}
//	if(cmd == 7){ Basic_Amplitude = float_value;}
}


void whenString(u8 cmd, u8 *str_value, u8 length)
{
	u8 i=0;
	printf("Str - %d: <", cmd);
	for(;i<length;i++) printf("%c", str_value[i]);
	printf(">\r\n");
}

void whenInts(u8 cmd, u32 *ints_value, u8 length)
{
	u8 i= 0;
	printf("Ints - %d: [", cmd);
	for (;i<length;i++){
	printf("%d, ", ints_value[i]);
	}
	printf("]\r\n");
}

void whenFloats(u8 cmd, float *floats_value, u8 length)
{
	u8 i= 0;
	printf("Floats - %d: [", cmd);
	for (;i<length;i++){
	printf("%f, ", floats_value[i]);
	}
	printf("]\r\n");
}

void whenCompound(u8 cmd, u32 *ints_value, float *floats_value, u8 length) //���
{
	u8 i= 0;
	printf("Compound - %d: [", cmd);
	for (;i<length;i++){
	printf("%d, ", ints_value[i]);
	printf("%f, ", floats_value[i]);
	}
	printf("]\r\n");
}

void whenError(char* chars)
{
	printf("[Error]:%s\r\n", chars);
}

void whenLost(uint8_t *buffers, uint16_t length){
	printf("[Lost]:%s\r\n", buffers);
}


