#include "dht11.h"
#include "Delay.h"  // 需用户自行实现微秒级延时函数（如基于SysTick）
#include "Measur.h"
//uint8_t temp = 0, humi = 0;  // 初始化为默认值
// GPIO模式切换函数
static void DHT11_GPIO_Mode(GPIOMode_TypeDef mode) {
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin   = DHT11_GPIO_PIN;
    gpio.GPIO_Mode  = mode;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &gpio);
}

// DHT11初始化
void DHT11_Init(void) {
    RCC_APB2PeriphClockCmd(DHT11_RCC, ENABLE); // 开启GPIO时钟
    DHT11_GPIO_Mode(GPIO_Mode_Out_PP);         // 推挽输出模式
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN); // 拉高总线
}

// 读取温湿度数据（返回0成功，1超时，2校验失败）
uint8_t DHT11_Read_Data(void) {
    uint8_t data[5] = {0};
    
    // 发送起始信号
    DHT11_GPIO_Mode(GPIO_Mode_Out_PP);         // 输出模式
    GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN); // 拉低总线
    Delay_ms(18);                              // 至少18ms[1,4](@ref)
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN); // 释放总线
    Delay_us(30);                               // 主机拉高20-40us[4,7](@ref)

    // 切换为输入模式（带上拉）
    DHT11_GPIO_Mode(GPIO_Mode_IN_FLOATING);    // 浮空输入[7](@ref)

    // 检测传感器响应（83us低电平 + 87us高电平）
    if (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) != 0) return 1;
    Delay_us(83);
    if (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) != 1) return 1;
    Delay_us(87);

    // 读取40位数据
    for (uint8_t i = 0; i < 40; i++) {
        while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN)); // 等待高电平
        Delay_us(35); // 判断高低电平的阈值时间[4,8](@ref)
        if (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN)) {
            data[i/8] |= (1 << (7 - i%8));     // 高位在前存储
            while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN)); // 等待低电平
        }
    }

    // 校验数据（校验和=前4字节之和）
    if (data[4] != (data[0] + data[1] + data[2] + data[3])) return 2;

    // 返回结果（DHT11小数部分固定为0）
    g_humidity = data[0];
    g_temperature = data[2];
    return 0;
}
