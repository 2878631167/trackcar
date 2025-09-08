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

/**
 * @brief  电机初始化函数，配置L298N相关GPIO和PWM。
 * @note   需在主程序初始化阶段调用。
 * @param  无
 * @retval 无
 */
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

/**
 * @brief  停止所有电机运行。
 * @note   设置PWM为0并更新电机状态。
 * @param  无
 * @retval 无
 */
void Motor_Stop(void) {
    MotorLeft_SetPWM(0);
    MotorRight_SetPWM(0);
    Motor_State = MOTOR_STATE_STOPPED; // 更新状态
}

/**
 * @brief  设置左电机PWM及方向。
 * @param  PWM 左电机PWM值，正为正转，负为反转。
 * @retval 无
 */
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

/**
 * @brief  设置右电机PWM及方向。
 * @param  PWM 右电机PWM值，正为正转，负为反转。
 * @retval 无
 */
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

/**
 * @brief  电机滑行（COAST）状态，使电机自由转动。
 * @note   通过将所有控制引脚拉低实现滑行。
 * @param  无
 * @retval 无
 */
void Motor_Coast(void) {
    MOTOR_RESET(l298nConfig.IN1);
    MOTOR_RESET(l298nConfig.IN2);
    MOTOR_RESET(l298nConfig.IN3);
    MOTOR_RESET(l298nConfig.IN4);
    MotorLeft_SetPWM(0);
    MotorRight_SetPWM(0);
    Motor_State = MOTOR_STATE_COAST; // 需在 MotorState_TypeDef 中定义
}

/**
 * @brief  电机刹车（BRAKE）状态，使电机快速停止。
 * @note   通过将所有控制引脚拉高实现刹车。
 * @param  无
 * @retval 无
 */
void Motor_Brake(void) {
    MOTOR_SET(l298nConfig.IN1);
    MOTOR_SET(l298nConfig.IN2);
    MOTOR_SET(l298nConfig.IN3);
    MOTOR_SET(l298nConfig.IN4);
    MotorLeft_SetPWM(0);
    MotorRight_SetPWM(0);
    Motor_State = MOTOR_STATE_BRAKE; // 需在 MotorState_TypeDef 中定义
}

/**
 * @brief  左电机滑行（COAST），自由转动。
 * @note   左电机控制引脚全拉低。
 */
void MotorLeft_Coast(void) {
    MOTOR_RESET(l298nConfig.IN1);
    MOTOR_RESET(l298nConfig.IN2);
    MotorLeft_SetPWM(0);
}

/**
 * @brief  右电机滑行（COAST），自由转动。
 * @note   右电机控制引脚全拉低。
 */
void MotorRight_Coast(void) {
    MOTOR_RESET(l298nConfig.IN3);
    MOTOR_RESET(l298nConfig.IN4);
    MotorRight_SetPWM(0);
}

/**
 * @brief  左电机刹车（BRAKE），快速停止。
 * @note   左电机控制引脚全拉高。
 */
void MotorLeft_Brake(void) {
    MOTOR_SET(l298nConfig.IN1);
    MOTOR_SET(l298nConfig.IN2);
    MotorLeft_SetPWM(0);
}

/**
 * @brief  右电机刹车（BRAKE），快速停止。
 * @note   右电机控制引脚全拉高。
 */
void MotorRight_Brake(void) {
    MOTOR_SET(l298nConfig.IN3);
    MOTOR_SET(l298nConfig.IN4);
    MotorRight_SetPWM(0);
}

