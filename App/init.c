
#include  "include.h"

/***************************
函数名：init_all
函数功能：初始化设置
参数：无
返回：无
***************************/
void init_all(void)
{
    ov7725_init();                                      //摄像头初始化
    //adc_config();                                     //电源电压采集
    OLED_Init();                                        //显示屏初始化     
    pwm_init();                                         //电舵机输出PWM初始化 
    key_init1();                                        //按键初始化
    gpio_init (PTA19,GPI,0);                            //输入输出初始化 
    gpio_init (PTA17,GPI,0);
    flash_init();                                       //flash初始化
    ftm_quad_init(FTM1);                                //正交解码初始化
    NVIC_init();                                        //中断配置初始化
    flashread();                                        //开机读flash参数   
    LED_Fill(0X00);                                     //OLED清屏 
}