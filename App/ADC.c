
#include "ADC.h"

void adc_config(void)
{
    adc_init(ADC0_DM1);                     //ADC��ʼ��
    gpio_init(PTB1,GPO,0);                  //��ʼ������������
}




void adc_diaplay(void)
{
    uint16 var=0;
    uint8  ge,shi,bai;
    uint8  display[20];
    
    var = adc_once   (ADC0_DM1, ADC_12bit);     //��ѹ�ɼ�
    var = (var*990)/4095; 
    ge  = (var%100)%10;
    shi = (var%100)/10;
    bai = var/100;
    sprintf(display,"%d.%d%d",bai,shi,ge);
    LED_P6x8Str(104,0,display);
    
    
    if(var<=730)                                //����ѹ���� 7.30V ʱ����
    {
        gpio_turn(PTB1);
    }
    else gpio_set(PTB1,0);
  
}
  









