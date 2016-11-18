
#ifndef __MOTOR_H_
#define __MOTOR_H_


#include "include.h"



extern uint16 motor_P;
extern uint16 motor_I;
extern uint16 motor_D;

uint8 motor_speed(uint8 straight_pulse, uint8 bend_pulse, int8 error);
void pwm_init(void);
void motor_PID(int desired_value, int actual_value);


#endif  


