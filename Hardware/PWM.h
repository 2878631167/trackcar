#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);
//void Steering_EnginePWM(uint16_t Compare);
extern uint16_t Duty_Left ;
extern uint16_t Duty_Right ;
#endif
