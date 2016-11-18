
#include "Motor.h"


#define MOTOR_HZ      (20*1000)                          //电机频率
#define STEER_HZ      300                                //舵机频率
#define STEER_MIDDLE  4500                               //舵机打中pwm

uint16 motor_P = 0;                                        //电机p值
uint16 motor_I = 0;                                        //电机i值
uint16 motor_D = 0;                                        //电机d值
int    motor_pwm;   
int    motor_error[3];                                   //存储电机的偏差值
 
/**********************************************
函数功能：初始化电机、舵机
参   数：无
返回 值：无
*************************************************/
void pwm_init(void)
{
    
    ftm_pwm_init(FTM2, FTM_CH0,STEER_HZ,STEER_MIDDLE);   //初始化 舵机
    
    ftm_pwm_init(FTM0, FTM_CH4,MOTOR_HZ,0);             //初始化 电机 PWM
    ftm_pwm_init(FTM0, FTM_CH5,MOTOR_HZ,0);             //初始化 电机 PWM    
}

/**********************************************
函数功能：电机输出速度控制
参   数1：straight_pulse 直道速度
参  数2：bend_pulse 弯道速度
参  数3：error 摄像头获取偏差值
返回 值：out_pulse实际输出脉冲
*************************************************/
uint8 motor_speed(uint8 straight_pulse, uint8 bend_pulse, int8 error)
{
    
    uint8 out_pulse = 0;
    
    if(error<7) out_pulse = straight_pulse;
    else out_pulse = bend_pulse;
    
    return out_pulse;
}

/**********************************************
函数功能：电机PID控制
参   数1：desired_value 期望值
参  数2：actual_value 实际值
返回 值：无
*************************************************/

void motor_PID(int desired_value, int actual_value)
{
    
     
     int sum_P = 0;
     int sum_I = 0;
     int sum_D = 0;                                     
     char i ;   
    
     if(actual_value>250) actual_value = 250;                    //把太高的数据去掉，滤波
     for(i=2;i>0;i--)                                           //存蓄数据
        motor_error[i] = motor_error[i-1];
                                                 
     motor_error[0] = desired_value - actual_value;
     sum_P = motor_P * (motor_error[0] - motor_error[1]);
     sum_I = motor_I * motor_error[0];
     sum_D = motor_D * (motor_error[0]-2*motor_error[1]+motor_error[2]);
     motor_pwm += (sum_P + sum_I + sum_D); 
     

     
    if (actual_value <= 15 )                                   //撞墙停车
        motor_pwm = 0;
    
    if (motor_pwm>3500)  motor_pwm = 3500;                     //限制最大脉冲

    
    
      
    if(motor_pwm>=0)
     {
        ftm_pwm_duty(FTM0, FTM_CH4,motor_pwm);                //精度10000   
        ftm_pwm_duty(FTM0, FTM_CH5,0);
     } 

    else 
    {
        ftm_pwm_duty(FTM0, FTM_CH4,0);                            
        ftm_pwm_duty(FTM0, FTM_CH5,0);  
     }  
  }






