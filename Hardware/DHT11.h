#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

// 定义DHT11连接的GPIO端口和引脚（示例使用PA9，用户可修改）
#define DHT11_GPIO_PORT GPIOB
#define DHT11_GPIO_PIN  GPIO_Pin_6
#define DHT11_RCC       RCC_APB2Periph_GPIOB
//extern uint8_t temp, humi;
// 函数声明
void DHT11_Init(void);
uint8_t DHT11_Read_Data(void);

#endif
