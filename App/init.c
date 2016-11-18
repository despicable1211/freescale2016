
#include  "include.h"

/***************************
��������init_all
�������ܣ���ʼ������
��������
���أ���
***************************/
void init_all(void)
{
    ov7725_init();                                      //����ͷ��ʼ��
    //adc_config();                                     //��Դ��ѹ�ɼ�
    OLED_Init();                                        //��ʾ����ʼ��     
    pwm_init();                                         //�������PWM��ʼ�� 
    key_init1();                                        //������ʼ��
    gpio_init (PTA19,GPI,0);                            //���������ʼ�� 
    gpio_init (PTA17,GPI,0);
    flash_init();                                       //flash��ʼ��
    ftm_quad_init(FTM1);                                //���������ʼ��
    NVIC_init();                                        //�ж����ó�ʼ��
    flashread();                                        //������flash����   
    LED_Fill(0X00);                                     //OLED���� 
}