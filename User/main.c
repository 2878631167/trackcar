#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "dht11.h"
#include "Timer.h"

#include "Ultrasonic.h"
#include "Motor.h"
#include "Track.h"
#include "PWM.h"
#include "PID.h"
#include "Tracking.h"  // 添加Tracking模块头文件
#include <stdbool.h>  // 支持bool类型

// 添加外部变量声明
extern int left_motor_speed;
extern int right_motor_speed;

#define HISTORY_SIZE 5
static uint8_t sensor_history[HISTORY_SIZE][TRACK_SENSOR_NUM];
static int history_index = 0;
static bool track_lost = false;

int main(void)
{
    Serial_Init();
    Motor_Init();
    Track_Init();
    LED_Init();
    OLED_Init();
    Timer3_Init();
    
    OLED_Clear();
    OLED_ShowString(0, 0, "Position:", OLED_6X8);
    OLED_ShowString(60, 48, "R:", OLED_6X8);  // 右电机标签
    OLED_ShowString(0, 48, "L:", OLED_6X8);  // 左电机标签

    OLED_Update();
    
//    MotorLeft_SetPWM(-80);
//    MotorRight_SetPWM(60);
    
while (1) 
{
	
	Tracking_Run();
    // // 获取当前传感器状态
    // const uint8_t *sensors = Track_GetSensorStates();

    // // 显示当前5路传感器状态
    // OLED_Printf(60, 0, OLED_6X8, "%d %d %d %d %d", 
    //            sensors[0], sensors[1], sensors[2],
    //            sensors[3], sensors[4]);
    // OLED_Update();

    // // 检测是否轨道丢失（所有传感器为0）
    // track_lost = true;
    // for(int i = 0; i < TRACK_SENSOR_NUM; i++) {
    //     if(sensors[i]) {
    //         track_lost = false;
    //         break;
    //     }
    // }
    
    // // 只在轨道未丢失时更新历史记录
    // if(!track_lost) {
    //     // 保存当前状态到历史记录
    //     for(int i = 0; i < TRACK_SENSOR_NUM; i++) {
    //         sensor_history[history_index][i] = sensors[i];
    //     }
        
    //     // 更新历史索引（循环缓冲）
    //     history_index = (history_index + 1) % HISTORY_SIZE;
    // }
    
    // // 显示最近5次传感器状态历史
    // for(int i = 0; i < HISTORY_SIZE; i++) {
    //     int y_pos = 8 + i*8;  // 每行间隔8像素
    //     OLED_Printf(0, y_pos, OLED_6X8, "H%d: %d%d%d%d%d", i+1,
    //                sensor_history[i][0], sensor_history[i][1],
    //                sensor_history[i][2], sensor_history[i][3],
    //                sensor_history[i][4]);
    // }
    
    // // 显示电机速度
    // OLED_ShowNum(12, 48, left_motor_speed, 3, OLED_6X8);  // 显示左电机速度值
    
    // OLED_ShowNum(72, 48, right_motor_speed, 3, OLED_6X8);  // 显示右电机速度值
    
    // OLED_Update();
}
}

void TIM3_IRQHandler(void)
{	
    static uint16_t Count;
    
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
        Count++;
        
        if (Count >= 1) {
            Count = 0;
			Track_RefreshSensors();
            
        }	
        
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}
