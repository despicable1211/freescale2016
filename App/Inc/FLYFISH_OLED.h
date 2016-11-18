#ifndef     _FLYFISH_OLED_H
#define     _FLYFISH_OLED_H


#include    "common.h"
#include    "MK60_port.h"
#include    "MK60_gpio.h"


//----------------4线SPI--OLED端口定义---------------
#define OLED_CS_Clr()       PTD10_OUT = 0//CS
#define OLED_CS_Set()       PTD10_OUT = 1

#define OLED_RST_Clr()      PTD11_OUT = 0//RST
#define OLED_RST_Set()      PTD11_OUT = 1

#define OLED_DC_Clr()       PTD12_OUT = 0//DC
#define OLED_DC_Set()       PTD12_OUT = 1

#define OLED_SCLK_Clr()     PTD13_OUT = 0//SCLK
#define OLED_SCLK_Set()     PTD13_OUT = 1

#define OLED_SDIN_Clr()     PTD14_OUT = 0//SDIN
#define OLED_SDIN_Set()     PTD14_OUT = 1


#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据


void OLED_Init(void);
void OLED_Write(u8 Data,u8 Cmd);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void OLED_Fill(void);
u32 OLED_POW(u8 m,u8 n);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,int32 num,u8 len,u8 size,u8 mode);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);
void OLED_ShowHanzi(u8 x,u8 y,u8 num,u8 size,u8 mode);
void OLED_ShowSentence(u8 x,u8 y,u8 sta,u8 end,u8 size);
void OLED_DrawBMP(u8 x,u8 y,u8 length,u8 height,u8 mode);
void OLED_ON(void);
void OLED_OFF(void);

#endif  //FLYFISH_OLED
