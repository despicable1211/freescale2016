/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */
#include  "include.h"

uint8  *center;                                       //指向中线数组
int i;  

void loop(void)
{
    camera_get_img();                                 //摄像头获取图像                   
    img_extract(img, imgbuff, CAMERA_SIZE);           //解压为灰度图像
    center = findCenterLine(img);                     //中线提取
    for(i = 59; i >= center[60]; i--)                 //把提取的中线放回图像显示
    {
        img[i][center[i]] = 0x00;            
    } 
    //if(gpio_get(PTA16)||sendflag)sendimg(img, CAMERA_W * CAMERA_H);  //图像发送到上位机
    if(gpio_get(PTA17))                                 //把数据写入flash
    {
        flasherase();                                  //擦除flash
        flashwrite();                                  //写入flash
      }
    get_error();                                       //获取图像偏差     
    if(gpio_get(PTA19))Key_control();                  //按键控制选择
    else Key_control1();
    Display();                                         //oled显示
}

void  main(void)
{   
    init_all();
 
    while(1)
    {
       loop();
    }
}



