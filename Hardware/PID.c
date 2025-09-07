#include "stm32f10x.h"  
#include "pid.h"
#include <string.h>
#include <stdlib.h>

static PIDController pid = {0.5f, 0.0f, 1.0f}; // 默认PID参数

// 更新PID参数(通过串口调用)
void PIDparamwrite(char array[100]) {
    char *token = strtok(array, " "); 
    pid.Kp = strtof(token, NULL); 
    
    token = strtok(NULL, " ");
    pid.Ki = strtof(token, NULL);
    
    token = strtok(NULL, " ");
    pid.Kd = strtof(token, NULL);
}
void PID_Init(void) {

    PID_Reset();  // 重置控制器状态
}

void PID_SetParameters(float Kp, float Ki, float Kd) {
    pid.Kp = Kp;
    pid.Ki = Ki;
    pid.Kd = Kd;
    PID_Reset();  // 重置控制器状态
}
// 核心PID计算函数
float PID_Calculate(float target, float actual) {
    pid.error1 = pid.error0;
    pid.error0 = target - actual;
    
    // 积分项处理
    if (pid.Ki != 0) {
        pid.errorInt += pid.error0;
    } else {
        pid.errorInt = 0;
    }
    
    // PID计算
    // float output = pid.Kp * pid.error0 + 
    //                pid.Ki * pid.errorInt + 
    //                pid.Kd * (pid.error0 - pid.error1);
    // PD计算
    float output = pid.Kp * pid.error0 + 
                   pid.Kd * (pid.error0 - pid.error1);
      
    return output;
}

// 重置PID控制器
void PID_Reset(void) {
    pid.error0 = 0;
    pid.error1 = 0;
    pid.errorInt = 0;
}
