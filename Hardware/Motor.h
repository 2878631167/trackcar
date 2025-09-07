#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
#include <stdint.h>

// 添加电机控制宏
#define MOTOR_SET(pin) GPIO_SetBits(pin.GPIOx, pin.GPIO_Pin)
#define MOTOR_RESET(pin) GPIO_ResetBits(pin.GPIOx, pin.GPIO_Pin)
// 添加电机模式枚举
typedef enum {
    MOTOR_MODE_AUTO,
    MOTOR_MODE_MANUAL
} MotorMode_TypeDef;

// 添加电机状态枚举
typedef enum {
    MOTOR_STATE_STOPPED,
    MOTOR_STATE_FORWARD,
    MOTOR_STATE_BACKWARD,
    MOTOR_STATE_TURNING_LEFT,
    MOTOR_STATE_TURNING_RIGHT
} MotorState_TypeDef;

// L298N配置结构体
typedef struct {
    struct {
        GPIO_TypeDef* GPIOx;
        uint16_t GPIO_Pin;
    } IN1, IN2, IN3, IN4;
} L298N_Config_TypeDef;

void Motor_Init(void);
void Motor_Stop(void);

void MotorLeft_SetPWM(int16_t PWM);
void MotorRight_SetPWM(int16_t PWM);

void Motor_SetSpeed(uint8_t motor, int speed);


#endif
