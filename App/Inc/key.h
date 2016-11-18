
#ifndef __KEY_H
#define __KEY_H	 



#include "include.h"






#define KEY_ON	0
#define KEY_OFF	1

extern int8 test_error;
void key_init1(void);            //°´¼ü³õÊ¼»¯
uint8_t Key_Scan(PTXn_e ptxn);
void Key_control(void);
void Key_control1(void);
void Display(void);
uint8_t Key_value(void);
void flashwrite();
void flashread();
void flasherase();



#endif


