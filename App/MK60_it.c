/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      山外K60 平台中断服务函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "MK60_it.h"

uint16 pulse_count=0;                                   //编码器回传脉冲
uint16 set_max_pulse=110;                              //设置电机最大脉冲
uint16 set_min_pulse=80;                               //设置电机最小脉冲
uint16 pulse_out = 0;                                 //电机当前所用脉冲

uint8 motor_start_flag=0;                             //电机开始标志
uint8 motor_stop_flag=0;                              //电机停止标志
int16 motor_start_key=0;                              //按键起跑标志

/*!==================================================================
 *  @brief      中断配置函数
 *  @since      v5.0
 *  备 注：     ....
 ==================================================================*/
void NVIC_init(void)
{
    NVIC_SetPriorityGrouping(4);                        //设置优先级分组,4bit 抢占优先级,没有亚优先级
    NVIC_SetPriority(PORTA_IRQn,0);                     //配置优先级
    NVIC_SetPriority(UART3_RX_TX_IRQn,1);               //配置优先级
    NVIC_SetPriority(DMA0_IRQn,2);                      //配置优先级
    NVIC_SetPriority(PIT0_IRQn,3);                      //配置优先级
    
    set_vector_handler(UART3_RX_TX_VECTORn,uart3_handler);// 设置中断服务函数
    uart_rx_irq_en (UART3);                               //开串口接收中断
    
    pit_init_ms(PIT0,10);                                 //初始化PIT0，定时时间为： 500ms
    set_vector_handler(PIT0_VECTORn ,PIT_IRQHandler);     //设置PIT0的中断服务函数为 PIT_IRQHandler
    enable_irq (PIT0_IRQn);                               //使能PIT0中断
}


/*!==================================================================
 *  @brief      PIT0中断服务函数
 *  @since      v5.0
 *  备 注：     .....
 ==================================================================*/
void PIT_IRQHandler(void)
{
      
       PIT_Flag_Clear(PIT0);                                                            //清中断标志位
       
       pulse_count =  ftm_quad_get(FTM1);                                               //保存脉冲计数器计算值
       ftm_quad_clean(FTM1);                                                            //清空脉冲计数器计算值
       steer_control( servo_PID());                                                     //舵机打角计算
       
       
       pulse_out = motor_speed(set_max_pulse,set_min_pulse,abs_error);                  //变速控制
       if (motor_stop_flag == 1 ) pulse_out=0;                                          //蓝牙控制停车
       motor_PID(pulse_out,pulse_count);                                                //电机pid控制
}

/*!=================================================================
 *  @brief      UART3中断服务函数
 *  @since      v5.0
 *  备 注：     .....
 ==================================================================*/
void uart3_handler(void)
{
    char ch;

    if(uart_query    (UART3) == 1)                      //接收数据寄存器满
    {
        //用户需要处理接收数据
        uart_getchar   (UART3, &ch);                    //无限等待接受1个字节
        //uart_putchar   (UART3 , ch);                  //发送字符串
        if(ch==11) motor_stop_flag = 1;                 //电机停车标志
        else if(ch==10) motor_stop_flag =0;             

    }
}