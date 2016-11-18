
#include "Motor.h"


#define MOTOR_HZ      (20*1000)                          //���Ƶ��
#define STEER_HZ      300                                //���Ƶ��
#define STEER_MIDDLE  4500                               //�������pwm

uint16 motor_P = 0;                                        //���pֵ
uint16 motor_I = 0;                                        //���iֵ
uint16 motor_D = 0;                                        //���dֵ
int    motor_pwm;   
int    motor_error[3];                                   //�洢�����ƫ��ֵ
 
/**********************************************
�������ܣ���ʼ����������
��   ������
���� ֵ����
*************************************************/
void pwm_init(void)
{
    
    ftm_pwm_init(FTM2, FTM_CH0,STEER_HZ,STEER_MIDDLE);   //��ʼ�� ���
    
    ftm_pwm_init(FTM0, FTM_CH4,MOTOR_HZ,0);             //��ʼ�� ��� PWM
    ftm_pwm_init(FTM0, FTM_CH5,MOTOR_HZ,0);             //��ʼ�� ��� PWM    
}

/**********************************************
�������ܣ��������ٶȿ���
��   ��1��straight_pulse ֱ���ٶ�
��  ��2��bend_pulse ����ٶ�
��  ��3��error ����ͷ��ȡƫ��ֵ
���� ֵ��out_pulseʵ���������
*************************************************/
uint8 motor_speed(uint8 straight_pulse, uint8 bend_pulse, int8 error)
{
    
    uint8 out_pulse = 0;
    
    if(error<7) out_pulse = straight_pulse;
    else out_pulse = bend_pulse;
    
    return out_pulse;
}

/**********************************************
�������ܣ����PID����
��   ��1��desired_value ����ֵ
��  ��2��actual_value ʵ��ֵ
���� ֵ����
*************************************************/

void motor_PID(int desired_value, int actual_value)
{
    
     
     int sum_P = 0;
     int sum_I = 0;
     int sum_D = 0;                                     
     char i ;   
    
     if(actual_value>250) actual_value = 250;                    //��̫�ߵ�����ȥ�����˲�
     for(i=2;i>0;i--)                                           //��������
        motor_error[i] = motor_error[i-1];
                                                 
     motor_error[0] = desired_value - actual_value;
     sum_P = motor_P * (motor_error[0] - motor_error[1]);
     sum_I = motor_I * motor_error[0];
     sum_D = motor_D * (motor_error[0]-2*motor_error[1]+motor_error[2]);
     motor_pwm += (sum_P + sum_I + sum_D); 
     

     
    if (actual_value <= 15 )                                   //ײǽͣ��
        motor_pwm = 0;
    
    if (motor_pwm>3500)  motor_pwm = 3500;                     //�����������

    
    
      
    if(motor_pwm>=0)
     {
        ftm_pwm_duty(FTM0, FTM_CH4,motor_pwm);                //����10000   
        ftm_pwm_duty(FTM0, FTM_CH5,0);
     } 

    else 
    {
        ftm_pwm_duty(FTM0, FTM_CH4,0);                            
        ftm_pwm_duty(FTM0, FTM_CH5,0);  
     }  
  }






