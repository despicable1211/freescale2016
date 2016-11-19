
#include "Steer.h"
#include "math.h"

uint16 servo_pwm;
uint16 servo_P = 0;              //���Pֵ
uint16 servo_I = 0;
uint16 servo_D = 50;             //���Dֵ


unsigned int abs_error;         //���ƫ�����ֵ
char last_error[3];              //�洢���ƫ��
char P[22]={ 4,4,4,4,4,   4,5,5,5,5, 
               6,6,6,6,7,   7,8,8,9,9,
               10,10};

unsigned char alsolute_value(char value);

/**********************************************
�������ܣ������ǿ���
��   ����PWM_steer
���� ֵ����
*************************************************/
void steer_control(uint16 pwm_steer)
{
    ftm_pwm_duty(FTM2, FTM_CH0,pwm_steer);              // ��������pwmռ�ձ� 
}

/**********************************************
�������ܣ����pid����
��   ������
���� ֵ��Servo_PWM
*************************************************/
uint16 servo_PID(void)
  {  
      char n;      
      int sum_kp,sum_ki,sum_kd;                                    
      

      for(n=2;n>0;n--)                                   //�����鱣��ƫ��
          {
            last_error[n] = last_error[n-1];
            }
              last_error[0] = error;
      sum_kp = last_error[0];
      sum_ki = last_error[0]-last_error[1];
      sum_kd = last_error[0]-2*last_error[1]+last_error[2];

      abs_error = alsolute_value(error);                   //���ƫ��ȡ����ֵ
      if(abs_error<=21)   servo_P = P[abs_error];         //����ƫ��ȡPֵ
      else if(abs_error>21&&abs_error<=40) servo_P = 18;   
      else if(abs_error>40&&abs_error<50) servo_P = 22;
      else if(abs_error>=50) servo_P=25;
      
      sum_kp = error - last_error[1];                         //���ı仯�ʣ�
    
      if(sum_kp>= 14 || sum_kp<= -14)                 //��ƫ���ʹ��󣬱�����һ������
         sum_kp = error -last_error[2];     
      if(error >= -2 && error <= 2) 
        servo_pwm = 4500;                               //����ƫ���С���
      else 
        servo_pwm = (uint16)(4500 + servo_P * error + servo_I*sum_ki + servo_D * sum_kd);  //���������
        

     /********************���ƴ�Ƿ�Χ************************/
      if(servo_pwm>=5600)       
          servo_pwm = 5600;
      else if(servo_pwm<=3400)     
          servo_pwm = 3400;
      else 
        servo_pwm = servo_pwm;
     
     return servo_pwm;     
  }



/**********************************************
�������ܣ�ȡ����ֵ
��   ����value
���� ֵ��temp
*************************************************/

unsigned char alsolute_value( char value) 
{
     unsigned char temp=0;
    if(value<0) temp = -value;
    else temp = value;
    return (unsigned char)temp;
  }

