#include "control.h"	

float Mechanical_angle=0; 


u8 UltrasonicWave_Voltage_Counter=0;

/* 定时器1中断服务程序 */
void TIM1_UP_IRQHandler( void )                         //定时器里调用delay_ms函数会打断主函数的systick定时器，导致定时失败，最好不要用
{
	if ( TIM_GetITStatus( TIM1, TIM_IT_Update ) != RESET )                  /* 检查指定的TIM中断发生与否:TIM 中断源 */
	{
		TIM_ClearITPendingBit( TIM1, TIM_IT_Update );                   /* 清除TIMx的中断待处理位:TIM 中断源 */
		time++;

		if(time % 50 == 0){  //5ms延时触发
//			pitch = Read_Kalman(0);
//			roll = Read_Kalman(1);
//			yaw = Read_Kalman(2);
			LED0 = ~LED0;
//////			while(Read_DMP(&pitch,&roll,&yaw) != 0);
			Read_DMP(&pitch,&roll,&yaw);                          //耗时3ms左右，隔10多个数据后会耗时55ms左右，读取速度太慢时会发生FIFO溢出-可用while读取，经优化后读取耗时只需1.8ms左右
//																														//当3.6ms的定时时刚好5ms定时
////			roll = Read_Kalman(1);
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								 //===得到陀螺仪数据
//			enconder_left = Read_Encoder(3);
//			enconder_right = Read_Encoder(4);
//			Balance_Pwm =balance_UP(roll,Mechanical_angle,gyrox);   //===平衡环PID控制	
//			Velocity_Pwm=velocity(enconder_left,enconder_right);       //===速度环PID控制	 
//			if(1==Flag_Left||1==Flag_Right)    
//			Turn_Pwm =turn(enconder_left,enconder_right,gyroz);        //===转向环PID控制
//			else Turn_Pwm=0.5*gyroz;
//			Moto1=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //===计算左轮电机最终PWM
//			Moto2=Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //===计算右轮电机最终PWM
//			Xianfu_Pwm();  																					 //===PWM限幅
//			Turn_Off(roll,12.0);																 //===检查角度以及电压是否正常
//			Set_Pwm(Moto1,Moto2);     			//===赋值给PWM寄存器  	
		}
	
		if(time_count == 1){ //测一段时间长短
			count_ms++;
		}
	}
}

//void EXTI9_5_IRQHandler(void) 
//{    
//	if(PBin(5)==0)		
//	{		
//		EXTI->PR=1<<5;                                           //===清除LINE5上的中断标志位   
//		mpu_dmp_get_data(&pitch,&roll,&yaw);										 //===得到欧拉角（姿态角）的数据
//		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								 //===得到陀螺仪数据
//		Encoder_Left=-Read_Encoder(2);                           //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
//		Encoder_Right=Read_Encoder(4);                           //===读取编码器的值
//		UltrasonicWave_Voltage_Counter++;
//		if(UltrasonicWave_Voltage_Counter==10)									 //===100ms读取一次超声波的数据以及电压
//		{
//			UltrasonicWave_Voltage_Counter=0;
//			Voltage=Get_battery_volt();		                         //===读取电池电压		
//			UltrasonicWave_StartMeasure();												 //===读取超声波的值
//		}
//		Balance_Pwm =balance_UP(pitch,Mechanical_angle,gyroy);   //===平衡环PID控制	
//		Velocity_Pwm=velocity(Encoder_Left,Encoder_Right);       //===速度环PID控制	 
//  	if(1==Flag_Left||1==Flag_Right)    
//		Turn_Pwm =turn(Encoder_Left,Encoder_Right,gyroz);        //===转向环PID控制
//		else Turn_Pwm=0.5*gyroz;
//		Moto1=Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //===计算左轮电机最终PWM
//		Moto2=Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //===计算右轮电机最终PWM
//	  Xianfu_Pwm();  																					 //===PWM限幅
//		Turn_Off(pitch,Voltage);																 //===检查角度以及电压是否正常
//		Set_Pwm(Moto1,Moto2);                                    //===赋值给PWM寄存器  
//	}       	
//} 

/**************************************************************************
函数功能：直立PD控制
入口参数：角度、机械平衡角度（机械中值）、角速度
返回  值：直立控制PWM
作    者：大鱼电子
**************************************************************************/
int balance_UP(float Angle,float Mechanical_balance,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-Mechanical_balance;    							 //===求出平衡的角度中值和机械相关
	 balance=balance_UP_KP*Bias+balance_UP_KD*Gyro;  //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}

/**************************************************************************
函数功能：速度PI控制
入口参数：电机编码器的值
返回  值：速度控制PWM
作    者：大鱼电子
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
		if(1==Flag_Qian)				
		{
			flag_UltrasonicWave=0;//无论前面是否有障碍物,蓝牙遥控优先级最高。将标志位置0
			Movement=-150;
		}
    else if(1==Flag_Hou)//接收到蓝牙APP遥控数据		
		{
			flag_UltrasonicWave=0;//无论前面是否有障碍物,蓝牙遥控优先级最高。将标志位置0
			Movement=150;//设定速度
		}
		/*当超声波的距离低于10cm时,即10cm处存在障碍物,将超声波标志位置并且赋积分值使其后退,这里做了个简单的P比例计算*/
		//else if(UltrasonicWave_Distance<10&&UltrasonicWave_Distance>3)
		//{
		//	flag_UltrasonicWave=1;
		//	Movement=UltrasonicWave_Distance*50;		
		//}
		else//没有接受到任何的数据
		{	
			flag_UltrasonicWave=0;
			Movement=0;
		}
   //=============速度PI控制器=======================//	
		Encoder_Least =(encoder_left+encoder_right)-0;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.8;		                                                //===一阶低通滤波器       
		Encoder += Encoder_Least*0.2;	                                    //===一阶低通滤波器    
		Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===接收遥控器数据，控制前进后退
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===积分限幅
		if(Encoder_Integral<-10000)		Encoder_Integral=-10000;            //===积分限幅	
		Velocity=Encoder*velocity_KP+Encoder_Integral*velocity_KI;        //===速度控制	
	  if(pitch<-40||pitch>40) 			Encoder_Integral=0;     						//===电机关闭后清除积分
		all = Encoder_Integral*velocity_KI;
	  return Velocity;
}
/**************************************************************************
函数功能：转向PD控制
入口参数：电机编码器的值、Z轴角速度
返回  值：转向控制PWM
作    者：大鱼电子
**************************************************************************/
int turn(int encoder_left,int encoder_right,float gyro)//转向控制
{
	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	  float Turn_Amplitude=100,Kp=20,Kd=0;     
	  //=============遥控左右旋转部分=======================//
	  //这一部分主要是根据旋转前的速度调整速度的起始速度，增加小车的适应性
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
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===转向	速度限幅
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
		else Kd=0;   //转向的时候取消陀螺仪的纠正 有点模糊PID的思想
  	//=============转向PD控制器=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===结合Z轴陀螺仪进行PD控制
	  return Turn;
}

/**************************************************************************
函数功能：倾角PD控制
入口参数：角度
返回  值：倾角控制PWM
作    者：平衡小车之家
**************************************************************************/
int balance(float Angle)
{  
   float Bias;                       //倾角偏差
	 static float Last_Bias,D_Bias;    //PID相关变量
	 int balance;                      //PWM返回值 
	 Bias=Angle-ZHONGZHI;              //求出平衡的角度中值 和机械相关
	 D_Bias=Bias-Last_Bias;            //求出偏差的微分 进行微分控制
	 balance=-Balance_KP*Bias-D_Bias*Balance_KD;   //===计算倾角控制的电机PWM  PD控制
   Last_Bias=Bias;                   //保持上一次的偏差
	 return balance;
}

/**************************************************************************
函数功能：位置PD控制 
入口参数：编码器
返回  值：位置控制PWM
作    者：平衡小车之家
**************************************************************************/
int Position(int Encoder)
{  
   static float Position_PWM,Last_Position,Position_Bias,Position_Differential;
	 static float Position_Least;
  	Position_Least =Encoder-Position_Zero;             //===
    Position_Bias *=0.8;		   
    Position_Bias += Position_Least*0.2;	             //===一阶低通滤波器  
	  Position_Differential=Position_Bias-Last_Position;
	  Last_Position=Position_Bias;
		Position_PWM=Position_Bias*Position_KP+Position_Differential*Position_KD; //===速度控制	
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

void whenCompound(u8 cmd, u32 *ints_value, float *floats_value, u8 length) //混合
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


