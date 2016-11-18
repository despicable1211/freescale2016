//#ifndef __PROCESS_H__
//#define __PROCESS_H__


#include "include.h"


extern uint8 Chang;
extern int8 error[59]; 
extern unsigned char THRESHOLD;
extern unsigned char  L_point1;
extern unsigned char  R_point1;
extern unsigned char  L_point2 ;
extern unsigned char  R_point2;  
extern unsigned char  one_midline;
extern unsigned char  oneline_flag; //单线道路标记
extern unsigned char  ZhiJiao;
extern uint8  ZhiJiao_flag;//直角标志
extern int time;
extern uint8 img[CAMERA_H][CAMERA_W];
extern signed char row_use;
extern unsigned char obstacle_flag;

void obstacle_detect(uint8 heigh);

//void Oneline_2(char h);
//void Make01_2(char a,char b,char c,char d,char e );
void MidLine(char begin_H,char end_H );
void ZhiJiaoFlay(char x,char t) ;
void Do_Zhijiao(char h);


//#endif /* __KL26_7620_H__  */
