#ifndef __PID_H
#define __PID_H

#include <stdint.h>

typedef struct {
    float Kp;       // 比例系数
    float Ki;       // 积分系数
    float Kd;       // 微分系数
    float error0;   // 当前误差
    float error1;   // 上次误差
    float errorInt; // 积分误差
} PIDController;

// 函数声明
void PIDparamwrite(char array[100]);
float PID_Calculate(float target, float actual);
void PID_Reset(void);
void PID_Init(void);
void PID_SetParameters(float Kp, float Ki, float Kd);
#endif
