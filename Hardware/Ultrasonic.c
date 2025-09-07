#include "Ultrasonic.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "Delay.h" // 需要实现一个微秒级延时函数

// 全局变量
volatile uint32_t echo_start_time = 0; // Echo信号开始时间
volatile uint32_t echo_end_time = 0;   // Echo信号结束时间
volatile uint8_t echo_flag = 0;        // Echo信号标志


// 初始化HC-SR04
void Ultrasonic_Init(void) {
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		    
    
    
		

    // 配置Trig引脚为推挽输出
		GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = TRIG_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TRIG_GPIO_PORT, &GPIO_InitStructure);

    // 配置Echo引脚为浮空输入
    GPIO_InitStructure.GPIO_Pin = ECHO_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ECHO_GPIO_PORT, &GPIO_InitStructure);

    // 配置外部中断
    
    
		EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
    EXTI_Init(&EXTI_InitStructure);

    // 配置NVIC
		NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置定时器2用于计时
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 1MHz计数频率
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM2, ENABLE);
}

// 获取距离
double Ultrasonic_GetDistance(void) {
    uint32_t duration = 0;
    double distance = 0;
    uint32_t timeout = 100000; // 超时计数器

    // 发送Trig信号
    GPIO_SetBits(TRIG_GPIO_PORT, TRIG_GPIO_PIN);
    Delay_us(15);
    GPIO_ResetBits(TRIG_GPIO_PORT, TRIG_GPIO_PIN);

    // 等待Echo信号开始
    while (echo_flag == 0 && timeout--);
    if (timeout == 0) return -1; // 超时返回错误值

    echo_flag = 0;

    // 计算时间差
    duration = echo_end_time - echo_start_time;

    // 计算距离（单位：厘米）
    distance = (double)duration / 58.0;
    return distance;
}

// 外部中断处理函数
void EXTI9_5_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
        if (GPIO_ReadInputDataBit(ECHO_GPIO_PORT, ECHO_GPIO_PIN) == SET) {
            // Echo信号上升沿
            echo_start_time = TIM_GetCounter(TIM2);
        } else {
            // Echo信号下降沿
            echo_end_time = TIM_GetCounter(TIM2);
            echo_flag = 1;
        }
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
}
