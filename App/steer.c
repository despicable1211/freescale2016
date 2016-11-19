
#include "Steer.h"
#include "math.h"

uint16 servo_pwm;
uint16 servo_P = 0;              //舵机P值
uint16 servo_I = 0;
uint16 servo_D = 50;             //舵机D值


unsigned int abs_error;         //打角偏差绝对值
char last_error[3];              //存储打角偏差
char P[22]={ 4,4,4,4,4,   4,5,5,5,5, 
               6,6,6,6,7,   7,8,8,9,9,
               10,10};

unsigned char alsolute_value(char value);

/**********************************************
函数功能：舵机打角控制
参   数：PWM_steer
返回 值：无
*************************************************/
void steer_control(uint16 pwm_steer)
{
    ftm_pwm_duty(FTM2, FTM_CH0,pwm_steer);              // 重新设置pwm占空比 
}

/**********************************************
函数功能：舵机pid控制
参   数：无
返回 值：Servo_PWM
*************************************************/
uint16 servo_PID(void)
  {  
      char n;      
      int sum_kp,sum_ki,sum_kd;                                    
      

      for(n=2;n>0;n--)                                   //用数组保存偏差
          {
            last_error[n] = last_error[n-1];
            }
              last_error[0] = error;
      sum_kp = last_error[0];
      sum_ki = last_error[0]-last_error[1];
      sum_kd = last_error[0]-2*last_error[1]+last_error[2];

      abs_error = alsolute_value(error);                   //打角偏差取绝对值
      if(abs_error<=21)   servo_P = P[abs_error];         //根据偏差取P值
      else if(abs_error>21&&abs_error<=40) servo_P = 18;   
      else if(abs_error>40&&abs_error<50) servo_P = 22;
      else if(abs_error>=50) servo_P=25;
      
      sum_kp = error - last_error[1];                         //误差的变化率；
    
      if(sum_kp>= 14 || sum_kp<= -14)                 //若偏差率过大，保持上一次数据
         sum_kp = error -last_error[2];     
      if(error >= -2 && error <= 2) 
        servo_pwm = 4500;                               //限制偏差过小打角
      else 
        servo_pwm = (uint16)(4500 + servo_P * error + servo_I*sum_ki + servo_D * sum_kd);  //打角量计算
        

     /********************限制打角范围************************/
      if(servo_pwm>=5600)       
          servo_pwm = 5600;
      else if(servo_pwm<=3400)     
          servo_pwm = 3400;
      else 
        servo_pwm = servo_pwm;
     
     return servo_pwm;     
  }



/**********************************************
函数功能：取绝对值
参   数：value
返回 值：temp
*************************************************/

unsigned char alsolute_value( char value) 
{
     unsigned char temp=0;
    if(value<0) temp = -value;
    else temp = value;
    return (unsigned char)temp;
  }

