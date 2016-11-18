/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */
#include  "include.h"

uint8  *center;                                       //ָ����������
int i;  

void loop(void)
{
    camera_get_img();                                 //����ͷ��ȡͼ��                   
    img_extract(img, imgbuff, CAMERA_SIZE);           //��ѹΪ�Ҷ�ͼ��
    center = findCenterLine(img);                     //������ȡ
    for(i = 59; i >= center[60]; i--)                 //����ȡ�����߷Ż�ͼ����ʾ
    {
        img[i][center[i]] = 0x00;            
    } 
    //if(gpio_get(PTA16)||sendflag)sendimg(img, CAMERA_W * CAMERA_H);  //ͼ���͵���λ��
    if(gpio_get(PTA17))                                 //������д��flash
    {
        flasherase();                                  //����flash
        flashwrite();                                  //д��flash
      }
    get_error();                                       //��ȡͼ��ƫ��     
    if(gpio_get(PTA19))Key_control();                  //��������ѡ��
    else Key_control1();
    Display();                                         //oled��ʾ
}

void  main(void)
{   
    init_all();
 
    while(1)
    {
       loop();
    }
}



