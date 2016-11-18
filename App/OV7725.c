#include "include.h" 
 
uint8 imgbuff[CAMERA_SIZE];                                 //定义存储接收图像的数组
uint8 img[CAMERA_H][CAMERA_W];


void ov7725_init(void)
{
    camera_init(imgbuff);                                   //摄像头初始化
    
    //配置中断服务函数
    set_vector_handler(PORTB_VECTORn , PORTB_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
}

/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTB_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTB_ISFR);
    flag = PORTB_ISFR;
    PORTB_ISFR  = ~0;                                   //清中断标志位

    n = 22;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 22;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif


}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


//发送图像到上位机显示
//不同的上位机，不同的命令，这里使用 yy_摄像头串口调试 软件
//如果使用其他上位机，则需要修改代码
void sendimg(void *imgaddr, uint32 imgsize)
{
#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //发送后命令
}