/******************************************************************************

          版权所有 (C), 2011-2012, 信意电子科技(http://xydz123.taobao.com/)

 ******************************************************************************
  文 件 名   : oled.h
  版 本 号   : v1.0
  作    者   : Guokaiyi
  生成日期   : 2012-11-12
  最近修改   : 
  功能描述   : oled.c 的头文件
  修改历史   :
  1.日    期   : 2012-11-12
    作    者   : Guokaiyi
    修改内容   : 创建文件

******************************************************************************/

#ifndef __OLED_H
#define __OLED_H

#include "include.h"
#include "stdint.h"
#include  <stdio.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <ctype.h>

/*----------------------------------------------------------------------------*
 * 宏定义                                                                     *
 *----------------------------------------------------------------------------*/
//#define LED_SCL       PORTA_PA2 
//#define LED_SDA       PORTA_PA3
//#define LED_RST       PORTA_PA4
//#define LED_DC        PORTA_PA5 
//#define LED_PORT      DDRA

#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0
                                                        
#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128

#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据

/*----------------------------------------------------------------------------*
 * 外部函数原型                                                               *
 *----------------------------------------------------------------------------*/
 void OLED_CfgGpio(void);
 void OLED_Init(void);
 void LED_SetPos(uint8_t ucIdxX, uint8_t ucIdxY);
 void LED_Fill(uint8_t ucData);                     //写0x00清屏
 
 
 //显示字符串
 void LED_P6x8Char(uint8_t ucIdxX,uint8_t ucIdxY,uint8_t ucData);
 void LED_P6x8Str(uint8_t ucIdxX,uint8_t ucIdxY,uint8_t ucDataStr[]);
 void LED_P8x16Str(uint8_t ucIdxX,uint8_t ucIdxY,uint8_t ucDataStr[]);
 void LED_P14x16Str(uint8_t ucIdxX,uint8_t ucIdxY,uint8_t ucDataStr[]);
 void LED_P16x16Str(uint8_t ucIdxX,uint8_t ucIdxY,uint8_t ucDataStr[]);       //汉字显示，要取模
 void LED_PXx16MixStr(uint8_t ucIdxX, uint8_t ucIdxY, uint8_t ucDataStr[]);
 
                                          
 
 //把相应的变量数值显示出来
 void LED_PrintChar(uint8_t ucIdxX, uint8_t ucIdxY, char cData);    
 void LED_PrintShort(uint8_t ucIdxX, uint8_t ucIdxY, int sData);    
 void LED_PrintImage(uint8_t *pucTable, uint32_t usRowNum, uint32_t usColumnNum);
 void SYSTEM_TIME_Delay(uint32_t usTime);
 void LED_P8x16Char(uint8_t ucIdxX, uint8_t ucIdxY, uint8_t ucData);
 void LED_8x16PrintShort(uint8_t ucIdxX, uint8_t ucIdxY, unsigned int sData);   
 void LED_8X16PrintChar(uint8_t ucIdxX, uint8_t ucIdxY, char cData);
 void LED_paodao(unsigned char L, unsigned char R );
 //void LED_PrintShort(uint8_t ucIdxX, uint8_t ucIdxY, int sData);
void Dis_Float(char ucIdxX,char ucIdxY,double real,char N) ;
void LED_8X16PrintChar_out(uint8_t ucIdxX, uint8_t ucIdxY,int cData);
void LED_6X8PrintChar_out(uint8_t ucIdxX, uint8_t ucIdxY,char cData);
void LED_8x16PrintShort_out(uint8_t ucIdxX, uint8_t ucIdxY, int sData);
void LED_DisNum(uint16_t x, uint16_t y, uint32_t num);
void Dis_Float_1(char ucIdxX,char ucIdxY,double real,char N) ;
void Dis_Float_2(char ucIdxX,char ucIdxY,double real,char N) ;
 void LED_paodao_2(void);
#endif   
