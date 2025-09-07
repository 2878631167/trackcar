#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "stm32f10x.h"

// 定义Trig和Echo引脚
#define TRIG_GPIO_PORT GPIOA
#define TRIG_GPIO_PIN GPIO_Pin_6
#define ECHO_GPIO_PORT GPIOA
#define ECHO_GPIO_PIN GPIO_Pin_7

// 函数声明

void Ultrasonic_Init(void); // 初始化HC-SR04
double Ultrasonic_GetDistance(void); // 获取距离

#endif /* __ULTRASONIC_H */
