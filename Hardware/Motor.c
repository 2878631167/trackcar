#include "stm32f10x.h"
#include "Motor.h"
#include "PWM.h"
#include <stdint.h>

// 电机控制模式变量

MotorMode_TypeDef Motor_Mode = MOTOR_MODE_AUTO; // 添加变量定义，默认自动模式
MotorState_TypeDef Motor_State = MOTOR_STATE_STOPPED; // 添加状态变量初始化

// L298N驱动引脚配置 (可根据实际硬件修改)
static const L298N_Config_TypeDef l298nConfig = {
    .IN1 = {GPIOB, GPIO_Pin_5},
    .IN2 = {GPIOB, GPIO_Pin_6},
    .IN3 = {GPIOB, GPIO_Pin_0},
    .IN4 = {GPIOB, GPIO_Pin_1}
};

void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure = {
        .GPIO_Mode = GPIO_Mode_Out_PP,
        .GPIO_Speed = GPIO_Speed_50MHz
    };
    
    // 初始化所有电机控制引脚
    GPIO_InitStructure.GPIO_Pin = l298nConfig.IN1.GPIO_Pin | l298nConfig.IN2.GPIO_Pin |
                                  l298nConfig.IN3.GPIO_Pin | l298nConfig.IN4.GPIO_Pin;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    PWM_Init();
}


void Motor_Stop(void) {
    MotorLeft_SetPWM(0);
    MotorRight_SetPWM(0);
    Motor_State = MOTOR_STATE_STOPPED; // 更新状态
}




void MotorLeft_SetPWM(int16_t PWM)
{
        
        if (PWM >= 0) {
            MOTOR_SET(l298nConfig.IN1);
            MOTOR_RESET(l298nConfig.IN2);
			
            PWM_SetCompare1(PWM);
        }
        else {            
			MOTOR_RESET(l298nConfig.IN1);
            MOTOR_SET(l298nConfig.IN2);
            PWM_SetCompare1(-PWM);
        }

}

void MotorRight_SetPWM(int16_t PWM)
{   
        
    
        if (PWM >= 0) {
			MOTOR_SET(l298nConfig.IN3);
            MOTOR_RESET(l298nConfig.IN4);
            PWM_SetCompare2(PWM);
        }
        else {
			MOTOR_RESET(l298nConfig.IN3);
            MOTOR_SET(l298nConfig.IN4);
            PWM_SetCompare2(-PWM);
        }

}

