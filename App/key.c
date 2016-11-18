
#include "key.h" 

char Page_choose=0;                     //ҳ��ѡ��                      
char Param_choose=0;                    //����ѡ��/

extern uint16 motor_start_key;
extern uint16 set_max_pulse;
extern uint16 set_min_pulse;
extern uint16 pulse_count;
extern uint8  motor_stop_flag;
extern signed int   motor_pwm;

uint16 *Variable[][6]={{&servo_P,&servo_D ,&motor_P ,&motor_D ,&set_max_pulse,&set_min_pulse  },{&motor_start_key}};//,{&ROL_P,&ROL_I,&ROL_D,&PIT_P,&PIT_I,&PIT_D},&YAW_P,&YAW_D



/**********************************************
�������ܣ���ʼ������
��   ������
���� ֵ����
*************************************************/
void key_init1(void)
{
 	 gpio_init (PTC8,  GPI,0);
         gpio_init (PTC9,  GPI,0); 
         gpio_init (PTC10, GPI,0); 
         gpio_init (PTC11, GPI,0);

}

/**
  * @brief   ����Ƿ��а�������
  * @param   ����Ķ˿ںͶ˿�λ
  *		@arg GPIOx: x�����ǣ�A...G�� 
  *		@arg GPIO_PIN ������GPIO_PIN_x��x������1...16��
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */

uint8_t Key_Scan(PTXn_e ptxn)
{	
	/*����Ƿ��а������� */
	if(gpio_get(ptxn) == KEY_ON ) 
	{	   
		/*��ʱ����*/
		DELAY_MS(10);

                
		if(gpio_get(ptxn) == KEY_ON )  
		{	 
			/*�ȴ������ͷ� */
			while(gpio_get(ptxn) == KEY_ON);   
			return 	KEY_ON;	 
		}
		else
			return KEY_OFF;
	}
	else
		return KEY_OFF;
}


uint8_t Key_value(void)
{
	char Key_munber=0;
	if (Key_Scan(PTC8)==0) Key_munber=1;
	if (Key_Scan(PTC9)==0) Key_munber=2;
	if (Key_Scan(PTC10)==0) Key_munber=3;
	if (Key_Scan(PTC11)==0) Key_munber=4;
	return Key_munber;
}
/******************************/
//��������:��������
//��������
//���أ���
/*******************************/
void Key_control1(void)
 {
  	char Key=0;
	Key = Key_value();        
	switch(Key)
		{
			case 1:Page_choose++;                                   //ҳ��ѡ��
				if(Page_choose>2) Page_choose=0; 
				LED_Fill(0x00); 
				break;
			case 2: Param_choose++;                                 //����ѡ��
				if(Param_choose > 5) Param_choose = 0;
				LED_Fill(0x00);  
				break;
			case 3: *Variable[Page_choose][Param_choose] +=100;
				break;
			case 4: *Variable[Page_choose][Param_choose] -=100;
				break;
		}
	
}
/**********************************************
�������ܣ���������
��   ������
���� ֵ����
*************************************************/
void Key_control(void)
{
	char Key=0;
	Key = Key_value();        
	switch(Key)
		{
			case 1:Page_choose++;
				if(Page_choose>2) Page_choose=0; 
				LED_Fill(0x00); 
				break;
			case 2: Param_choose++;
				if(Param_choose > 5) Param_choose = 0;
				LED_Fill(0x00);  
				break;
			case 3: *Variable[Page_choose][Param_choose] +=1; 
				break;
			case 4: *Variable[Page_choose][Param_choose] -=1;
                                flashwrite();
				break;
		}
	
}
/**********************************************
�������ܣ�oled��ʾ
��   ������
���� ֵ����
*************************************************/
void Display(void)
{
	Key_control();
	switch(Page_choose)
	{
		case 0:  LED_PrintShort(8,0,servo_P);
                         LED_PrintShort(64,0,servo_D);
                        
                         LED_PrintShort(8,2,motor_P);
                         LED_PrintShort(64,2,motor_D);
                        
                         LED_PrintShort(8, 4,set_max_pulse);
                         LED_PrintShort(64,4,set_min_pulse);
                         
                         LED_P6x8Str(30, 5, "error:");
                         LED_PrintShort(68,5,error);
                         
                         LED_P6x8Str(30,7,"pulse:");
                         LED_PrintShort(64,7,pulse_count);
                         switch (Param_choose)
                          {
                            case 0: LED_P6x8Str(0, 0, "&");
                                    break;
                            case 1: LED_P6x8Str(60, 0, "&");
                                    break;
                            case 2: LED_P6x8Str(0, 2, "&");
                                    break;
                            case 3: LED_P6x8Str(60, 2, "&");
                                    break;
                            case 4: LED_P6x8Str(0, 4, "&");
                                    break;
                            case 5: LED_P6x8Str(60, 4, "&");
                                    break;                        
                          }     
			 break;
		
		case 1: LED_paodao_2();
                        LED_P6x8Str(86,0,"MT_pmw:");
                        LED_PrintShort(86,1,motor_pwm);
                        LED_P6x8Str(86,3,"ST_pmw:");
                        LED_PrintShort(86,4,servo_pwm);
                        LED_P6x8Str(86, 6, "error:");
                        LED_PrintShort(86,7,error);     
			break;			                
	}	 
}
/***************************
��������flashwrite
�������ܣ�д���Ե�ֻ��flash
��������
���أ���
***************************/
void flashwrite()
{
  //flash_write(252, 0, Servo_P );       //��Servo_Pд������254��ƫ�Ƶ�ַ0
  flash_write(251, 0, servo_D );
  flash_write(251, 1*16, motor_P );
  flash_write(251, 2*16, motor_D );
  flash_write(251, 3*16, set_max_pulse );
  flash_write(251, 4*16, set_min_pulse );
}
/******************************
��������flashread
���ܣ���flash����
��������
���أ���
******************************/
void flashread()
{
  //Servo_P=flash_read(254,0,uint8);            //��flash254��������ַƫ��0����������uint8
  servo_D=flash_read(251,0,uint16);
  motor_P=flash_read(251,1*16,uint16);
  motor_D=flash_read(251,2*16,uint16);
  set_max_pulse=flash_read(251,3*16,uint16);
  set_min_pulse=flash_read(251,4*16,uint16);
}
/**********************************************
����  ����flasherase
�������ܣ�����flash
��   ������
���� ֵ����
*************************************************/
void flasherase()
{
  flash_erase_sector(251);
}