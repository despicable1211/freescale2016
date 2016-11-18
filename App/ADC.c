
#include "ADC.h"

void adc_config(void)
{
    adc_init(ADC0_DM1);                     //ADC初始化
    gpio_init(PTB1,GPO,0);                  //初始化蜂鸣器引脚
}




void adc_diaplay(void)
{
    uint16 var=0;
    uint8  ge,shi,bai;
    uint8  display[20];
    
    var = adc_once   (ADC0_DM1, ADC_12bit);     //电压采集
    var = (var*990)/4095; 
    ge  = (var%100)%10;
    shi = (var%100)/10;
    bai = var/100;
    sprintf(display,"%d.%d%d",bai,shi,ge);
    LED_P6x8Str(104,0,display);
    
    
    if(var<=730)                                //当电压低于 7.30V 时报警
    {
        gpio_turn(PTB1);
    }
    else gpio_set(PTB1,0);
  
}
  









