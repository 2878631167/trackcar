#ifndef __TRACKING_H
#define __TRACKING_H

#include "stm32f10x.h"

// 循迹模式定义
typedef enum {
    TRACKING_MODE_OFF = 0,
    TRACKING_MODE_LINE,    // 循线模式
    TRACKING_MODE_EDGE,    // 沿边模式
    TRACKING_MODE_CUSTOM   // 自定义模式
} TrackingMode_TypeDef;

// 循迹状态定义
typedef enum {
    TRACKING_STATE_IDLE = 0,
    TRACKING_STATE_RUNNING,
    TRACKING_STATE_LOST,
    TRACKING_STATE_COMPLETED,
    TRACKING_STATE_ERROR
} TrackingState_TypeDef;

// 传感器配置结构体
typedef struct {
    uint8_t sensorCount;   // 传感器数量
    float sensorSpacing;   // 传感器间距(mm)
    float detectionRange;  // 检测范围(mm)
} SensorConfig_TypeDef;

// 函数声明
void Tracking_Init(SensorConfig_TypeDef config);
void Tracking_Run(void);
void Tracking_Stop(void);
void Tracking_SetMode(TrackingMode_TypeDef mode);
void Tracking_SetCustomTarget(float target);
TrackingMode_TypeDef Tracking_GetMode(void);
TrackingState_TypeDef Tracking_GetState(void);
void Tracking_SetBaseSpeed(uint8_t speed);
void Tracking_SetPIDParams(float Kp, float Ki, float Kd);
void Tracking_CalibrateSensors(void);
float Tracking_GetLastError(void);

#endif
