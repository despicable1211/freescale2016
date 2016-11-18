#ifndef __STEER_H__
#define __STEER_H__


#include "include.h"

extern uint16 servo_pwm;
extern uint16 servo_P;
extern uint16 servo_D;
extern unsigned int abs_error;
void steer_control(uint16 pwm_steer);
uint16 servo_PID(void);



#endif 
