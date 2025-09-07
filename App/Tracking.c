#include "stm32f10x.h"
#include "Track.h"
#include "Motor.h"
#include "PID.h"

// 电机方向定义
#define MOTOR_LEFT  0
#define MOTOR_RIGHT 1
#define MAX_ADJUST 40 // 最大调整量
int baseSpeed = 57; //基础速度

// 添加全局变量存储电机速度
int left_motor_speed = 0;
int right_motor_speed = 0;

void Tracking_Init(void)
{
    Track_Init();
    Motor_Init();
}

// 在Tracking.c中
void Tracking_Run(void) {
    int error = Track_GetError();
    
    // 更新PID控制器
    float output = PID_Calculate(0, error);
    
    // 增加调整量限制
    if (output > MAX_ADJUST) {
        output = MAX_ADJUST;
    } else if (output < -MAX_ADJUST) {
        output = -MAX_ADJUST;
    }
    
    // 更新电机速度并设置PWM
    left_motor_speed = baseSpeed - output;
    right_motor_speed = baseSpeed + output;
    
    MotorLeft_SetPWM(left_motor_speed);
    MotorRight_SetPWM(right_motor_speed);
}