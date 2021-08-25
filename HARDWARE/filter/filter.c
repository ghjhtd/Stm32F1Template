#include "filter.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
float K1 =0.02; 
float angle1, angle_dot1; 	
float Q_angle=0.001;// 过程噪声的协方差
float Q_gyro=0.003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle=0.5;// 测量噪声的协方差 既测量偏差
float dt=0.005;//                 
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

float K2 =0.02; 
float angle2, angle_dot2; 	
float Q_angle2=0.001;// 过程噪声的协方差
float Q_gyro2=0.003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle2=0.5;// 测量噪声的协方差 既测量偏差
float dt2=0.005;//                 
char  C_02 = 1;
float Q_bias2, Angle_err2;
float PCt_02, PCt_12, E2;
float K_02, K_12, t_02, t_12;
float Pdot2[4] ={0,0,0,0};
float PP2[2][2] = { { 1, 0 },{ 0, 1 } };

float K3 =0.02; 
float angle3, angle_dot3; 	
float Q_angle3=0.001;// 过程噪声的协方差
float Q_gyro3=0.003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle3=0.5;// 测量噪声的协方差 既测量偏差
float dt3=0.005;//                 
char  C_03 = 1;
float Q_bias3, Angle_err3;
float PCt_03, PCt_13, E3;
float K_03, K_13, t_03, t_13;
float Pdot3[4] ={0,0,0,0};
float PP3[2][2] = { { 1, 0 },{ 0, 1 } };
/**************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Kalman_Filter1(float Accel,float Gyro)		
{
	angle1+=(Gyro - Q_bias) * dt; //先验估计
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - angle1;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle1	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	angle_dot1   = Gyro - Q_bias;	 //输出值(后验估计)的微分=角速度
}

/**************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Kalman_Filter2(float Accel,float Gyro)		
{
	angle2+=(Gyro - Q_bias2) * dt2; //先验估计
	Pdot2[0]=Q_angle2 - PP2[0][1] - PP2[1][0]; // Pk-先验估计误差协方差的微分

	Pdot2[1]=-PP2[1][1];
	Pdot2[2]=-PP2[1][1];
	Pdot2[3]=Q_gyro2;
	PP2[0][0] += Pdot2[0] * dt2;   // Pk-先验估计误差协方差微分的积分
	PP2[0][1] += Pdot2[1] * dt2;   // =先验估计误差协方差
	PP2[1][0] += Pdot2[2] * dt2;
	PP2[1][1] += Pdot2[3] * dt2;
		
	Angle_err2 = Accel - angle2;	//zk-先验估计
	
	PCt_02 = C_02 * PP2[0][0];
	PCt_12 = C_02 * PP2[1][0];
	
	E2 = R_angle2 + C_02 * PCt_02;
	
	K_02 = PCt_02 / E2;
	K_12 = PCt_12 / E2;
	   
	t_02 = PCt_02;
	t_12 = C_02 * PP2[0][1];

	PP2[0][0] -= K_02 * t_02;		 //后验估计误差协方差
	PP2[0][1] -= K_02 * t_12;
	PP2[1][0] -= K_12 * t_02;
	PP2[1][1] -= K_12 * t_12;
		
	angle2	+= K_02 * Angle_err2;	 //后验估计
	Q_bias2	+= K_12 * Angle_err2;	 //后验估计
	angle_dot2   = Gyro - Q_bias2;	 //输出值(后验估计)的微分=角速度
}

/**************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Kalman_Filter3(float Accel,float Gyro)		
{
	angle3+=(Gyro - Q_bias3) * dt3; //先验估计
	Pdot3[0]=Q_angle3 - PP3[0][1] - PP3[1][0]; // Pk-先验估计误差协方差的微分

	Pdot3[1]=-PP3[1][1];
	Pdot3[2]=-PP3[1][1];
	Pdot3[3]=Q_gyro3;
	PP3[0][0] += Pdot3[0] * dt3;   // Pk-先验估计误差协方差微分的积分
	PP3[0][1] += Pdot3[1] * dt3;   // =先验估计误差协方差
	PP3[1][0] += Pdot3[2] * dt3;
	PP3[1][1] += Pdot3[3] * dt3;
		
	Angle_err3 = Accel - angle3;	//zk-先验估计
	
	PCt_03 = C_03 * PP3[0][0];
	PCt_13 = C_03 * PP3[1][0];
	
	E3 = R_angle3 + C_03 * PCt_03;
	
	K_03 = PCt_03 / E3;
	K_13 = PCt_13 / E3;
	 
	t_03 = PCt_03;
	t_13 = C_03 * PP3[0][1];

	PP3[0][0] -= K_03 * t_03;		 //后验估计误差协方差
	PP3[0][1] -= K_03 * t_13;
	PP3[1][0] -= K_13 * t_03;
	PP3[1][1] -= K_13 * t_13;
		
	angle3	+= K_03 * Angle_err3;	 //后验估计
	Q_bias3	+= K_13 * Angle_err3;	 //后验估计
	angle_dot3   = Gyro - Q_bias3;	 //输出值(后验估计)的微分=角速度
}

/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Yijielvbo(float angle_m, float gyro_m)
{
   angle1 = K1 * angle_m+ (1-K1) * (angle1 + gyro_m * 0.005);
}
