#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "Motor.h"
#include "pid.h"
#include <string.h>

#define BUFFER_SIZE 100

static uint8_t rxBuffer[BUFFER_SIZE];
static uint16_t rxIndex = 0;

// 串口初始化
void SerialCom_Init(uint32_t baudRate) {
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置USART1 Tx (PA9)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置USART1 Rx (PA10)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

// 处理接收到的命令
void ProcessCommand(char* command) {
    if(strncmp(command, "PID ", 4) == 0) {
        PIDparamwrite(command + 4);
    } else {
        MotorCmdGet(command); // 使用已有的电机命令处理函数
    }
}

// USART1中断处理函数
void USART1_IRQHandler(void) {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t ch = USART_ReceiveData(USART1);

        if(ch == '\n' || ch == '\r') {
            if(rxIndex > 0) {
                rxBuffer[rxIndex] = '\0';
                ProcessCommand((char*)rxBuffer);
                rxIndex = 0;
            }
        } else if(rxIndex < BUFFER_SIZE - 1) {
            rxBuffer[rxIndex++] = ch;
        }
    }
}