#include "stm32f10x.h"
#include "Track.h"
#include "Motor.h"

// 定义传感器GPIO配置数组
static const TrackSensorGPIO_TypeDef trackSensorGPIO[TRACK_SENSOR_NUM] = {
    {GPIOB, GPIO_Pin_10},  // TRACK_SECONDARY_LEFT
    {GPIOB, GPIO_Pin_11},  // TRACK_PRIME_LEFT
    {GPIOB, GPIO_Pin_12},  // TRACK_MEDIUM
    {GPIOB, GPIO_Pin_13},  // TRACK_PRIME_RIGHT
    {GPIOB, GPIO_Pin_14}   // TRACK_SECONDARY_RIGHT
};

// 使用静态变量存储传感器状态
static uint8_t track_sensors[TRACK_SENSOR_NUM];

void Track_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure = {
        .GPIO_Mode = GPIO_Mode_IN_FLOATING,
        .GPIO_Speed = GPIO_Speed_50MHz
    };
    
    for (int i = 0; i < TRACK_SENSOR_NUM; i++) {
        GPIO_InitStructure.GPIO_Pin = trackSensorGPIO[i].GPIO_Pin;
        GPIO_Init(trackSensorGPIO[i].GPIOx, &GPIO_InitStructure);
    }
}

void Track_RefreshSensors(void) {
    for (int i = 0; i < TRACK_SENSOR_NUM; i++) {
        track_sensors[i] = GPIO_ReadInputDataBit(trackSensorGPIO[i].GPIOx, trackSensorGPIO[i].GPIO_Pin);
    }
}

int Track_GetError(void) {
    static int last_error = 0;  // 保存上一次误差值
    static int no_sensor_count = 0;  // 新增：连续无传感器计数
    static const int sensor_weights[TRACK_SENSOR_NUM] = {
        -35,  // TRACK_SECONDARY_LEFT
        -20,  // TRACK_PRIME_LEFT
        0,    // TRACK_MEDIUM
        20,   // TRACK_PRIME_RIGHT
        35    // TRACK_SECONDARY_RIGHT
    };

    int total_weight = 0;
    int active_sensors = 0;
    
    for(int i = 0; i < TRACK_SENSOR_NUM; i++) {
        if(track_sensors[i]) {
            total_weight += sensor_weights[i];
            active_sensors++;
        }
    }
    
    if(active_sensors == 0) {
        no_sensor_count++;
        if(no_sensor_count < 3) {
            return 0;  // 连续3次内无传感器返回0
        }
        return last_error;  // 连续3次无传感器返回历史值
    } else {
        no_sensor_count = 0;  // 重置计数器
    }
    
    if(active_sensors > 1) {
        total_weight /= active_sensors;
    }
    
    last_error = total_weight;  // 更新保存的误差值
    return total_weight;
}

/**
 * @brief 获取轨道传感器状态数组
 * @return const uint8_t* 指向传感器状态数组的指针
 */
const uint8_t* Track_GetSensorStates(void) {
   
    return track_sensors;
}

/**
 * @brief 获取当前循迹偏差值
 * @return int 循迹偏差值(-100~+100)
 */
int Track_GetCurrentError(void) {
    return Track_GetError();
}
