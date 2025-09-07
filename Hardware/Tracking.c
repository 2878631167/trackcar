#include "stm32f10x.h"
#include "pid.h"
#include "Motor.h"
#include "Sensor.h"
#include "Tracking.h"
#include <math.h>

// 循迹控制参数
static PIDController trackingPID = {1.0f, 0.0f, 0.0f};
static uint8_t baseSpeed = 50;
static TrackingMode_TypeDef trackingMode = TRACKING_MODE_OFF;
static TrackingState_TypeDef trackingState = TRACKING_STATE_IDLE;
static float customTarget = 0.0f;
static SensorConfig_TypeDef sensorConfig;
static float lastError = 0.0f;

// 初始化循迹功能
void Tracking_Init(SensorConfig_TypeDef config) {
    sensorConfig = config;
    Sensor_Init();
    PID_Reset(&trackingPID);
    trackingState = TRACKING_STATE_IDLE;
}

// 主循迹函数
void Tracking_Run(void) {
    if(trackingMode == TRACKING_MODE_OFF || Motor_GetMode() != MOTOR_MODE_AUTO) {
        trackingState = TRACKING_STATE_IDLE;
        return;
    }

    // 获取传感器数据
    float sensorValue = GetSensorValue();
    lastError = sensorValue;
    
    // 根据模式确定目标值
    float targetValue = 0.0f;
    switch(trackingMode) {
        case TRACKING_MODE_LINE:
            targetValue = 0.0f; // 居中
            break;
        case TRACKING_MODE_EDGE:
            targetValue = 0.5f; // 沿边
            break;
        case TRACKING_MODE_CUSTOM:
            targetValue = customTarget;
            break;
        default:
            break;
    }

    // 计算PID输出
    float output = PID_Calculate(&trackingPID, targetValue, sensorValue);

    // 应用电机控制(带限幅)
    output = fmaxf(-baseSpeed, fminf(output, baseSpeed));
    MotorLeft_SetPWM(baseSpeed + output);
    MotorRight_SetPWM(baseSpeed - output);
    
    // 更新状态
    if(fabs(sensorValue) > 1.0f) {
        trackingState = TRACKING_STATE_LOST;
    } else if(fabs(sensorValue) < 0.05f) {
        trackingState = TRACKING_STATE_RUNNING;
    }
}

// 新增功能实现
void Tracking_Stop(void) {
    MotorLeft_SetPWM(0);
    MotorRight_SetPWM(0);
    trackingState = TRACKING_STATE_IDLE;
}

void Tracking_SetCustomTarget(float target) {
    customTarget = target;
}

void Tracking_CalibrateSensors(void) {
    // 实现传感器校准逻辑
    Sensor_Calibrate();
}

float Tracking_GetLastError(void) {
    return lastError;
}

// 设置循迹模式
void Tracking_SetMode(TrackingMode_TypeDef mode) {
    trackingMode = mode;
    if(mode == TRACKING_MODE_OFF) {
        trackingState = TRACKING_STATE_IDLE;
    }
}

// 获取当前循迹模式
TrackingMode_TypeDef Tracking_GetMode(void) {
    return trackingMode;
}

// 获取当前循迹状态
TrackingState_TypeDef Tracking_GetState(void) {
    return trackingState;
}

// 设置基础速度
void Tracking_SetBaseSpeed(uint8_t speed) {
    baseSpeed = speed;
}

// 设置PID参数
void Tracking_SetPIDParams(float Kp, float Ki, float Kd) {
    trackingPID.Kp = Kp;
    trackingPID.Ki = Ki;
    trackingPID.Kd = Kd;
    PID_Reset(&trackingPID);
}