#ifndef __FILTER_H
#define __FILTER_H
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
extern float angle1, angle_dot1; 	
extern float angle2, angle_dot2; 
extern float angle3, angle_dot3; 
void Kalman_Filter1(float Accel,float Gyro);		
void Kalman_Filter2(float Accel,float Gyro);		
void Kalman_Filter3(float Accel,float Gyro);		
void Yijielvbo(float angle_m, float gyro_m);
#endif
