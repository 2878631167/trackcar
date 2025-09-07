#include "stm32f10x.h"                  // Device header

/* PWM 左轮占空比 */
int16_t Duty_Left = 0;
/* PWM 右轮占空比 */
int16_t Duty_Right = 0;
/* 舵机 PWM 占空比 */
int16_t Duty_Servo = 0;

/**
 * @brief  初始化 TIM2 的 PWM 输出，PA0/PA1
 * @note   配置 GPIO、定时器时基和 PWM 模式
 */
void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; /* PIN_PA 00 01 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;      /* ARR */
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 2;    /* PSC */
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); /* 未配置的寄存器给初值，防止出现未知错误 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;     /* CCR */
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief  初始化 TIM3 的 PWM 输出，PA6 用于舵机
 * @note   舵机 PWM 频率一般为 50Hz（周期20ms），占空比1ms~2ms
 */
void Servo_PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; /* PA6 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;      /* 20ms周期，单位1us */
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;      /* 1us计数，72MHz/72=1MHz */
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500; /* 默认1.5ms中位 */
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief  设置 TIM2 通道1 的 PWM 占空比
 * @param  Compare: 占空比值
 */
void PWM_SetCompare1(uint16_t Compare)
{
	Duty_Left = Compare;
	TIM_SetCompare1(TIM2, Compare);
	
}

/**
 * @brief  设置 TIM2 通道2 的 PWM 占空比
 * @param  Compare2: 占空比值
 */
void PWM_SetCompare2(uint16_t Compare2)
{
	Duty_Right = Compare2;
	TIM_SetCompare2(TIM2, Compare2);
}

/**
 * @brief  设置舵机 PWM 占空比（TIM3 通道1）
 * @param  Compare: 占空比值（单位：us，范围一般为 500~2500）
 */
void Servo_SetCompare(uint16_t Compare)
{
    Duty_Servo = Compare;
    TIM_SetCompare1(TIM3, Compare);
}
