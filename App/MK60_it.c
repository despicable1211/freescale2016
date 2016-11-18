/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      ɽ��K60 ƽ̨�жϷ�����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "MK60_it.h"

uint16 pulse_count=0;                                   //�������ش�����
uint16 set_max_pulse=110;                              //���õ���������
uint16 set_min_pulse=80;                               //���õ����С����
uint16 pulse_out = 0;                                 //�����ǰ��������

uint8 motor_start_flag=0;                             //�����ʼ��־
uint8 motor_stop_flag=0;                              //���ֹͣ��־
int16 motor_start_key=0;                              //�������ܱ�־

/*!==================================================================
 *  @brief      �ж����ú���
 *  @since      v5.0
 *  �� ע��     ....
 ==================================================================*/
void NVIC_init(void)
{
    NVIC_SetPriorityGrouping(4);                        //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
    NVIC_SetPriority(PORTA_IRQn,0);                     //�������ȼ�
    NVIC_SetPriority(UART3_RX_TX_IRQn,1);               //�������ȼ�
    NVIC_SetPriority(DMA0_IRQn,2);                      //�������ȼ�
    NVIC_SetPriority(PIT0_IRQn,3);                      //�������ȼ�
    
    set_vector_handler(UART3_RX_TX_VECTORn,uart3_handler);// �����жϷ�����
    uart_rx_irq_en (UART3);                               //�����ڽ����ж�
    
    pit_init_ms(PIT0,10);                                 //��ʼ��PIT0����ʱʱ��Ϊ�� 500ms
    set_vector_handler(PIT0_VECTORn ,PIT_IRQHandler);     //����PIT0���жϷ�����Ϊ PIT_IRQHandler
    enable_irq (PIT0_IRQn);                               //ʹ��PIT0�ж�
}


/*!==================================================================
 *  @brief      PIT0�жϷ�����
 *  @since      v5.0
 *  �� ע��     .....
 ==================================================================*/
void PIT_IRQHandler(void)
{
      
       PIT_Flag_Clear(PIT0);                                                            //���жϱ�־λ
       
       pulse_count =  ftm_quad_get(FTM1);                                               //�����������������ֵ
       ftm_quad_clean(FTM1);                                                            //����������������ֵ
       steer_control( servo_PID());                                                     //�����Ǽ���
       
       
       pulse_out = motor_speed(set_max_pulse,set_min_pulse,abs_error);                  //���ٿ���
       if (motor_stop_flag == 1 ) pulse_out=0;                                          //��������ͣ��
       motor_PID(pulse_out,pulse_count);                                                //���pid����
}

/*!=================================================================
 *  @brief      UART3�жϷ�����
 *  @since      v5.0
 *  �� ע��     .....
 ==================================================================*/
void uart3_handler(void)
{
    char ch;

    if(uart_query    (UART3) == 1)                      //�������ݼĴ�����
    {
        //�û���Ҫ�����������
        uart_getchar   (UART3, &ch);                    //���޵ȴ�����1���ֽ�
        //uart_putchar   (UART3 , ch);                  //�����ַ���
        if(ch==11) motor_stop_flag = 1;                 //���ͣ����־
        else if(ch==10) motor_stop_flag =0;             

    }
}