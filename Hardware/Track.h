#ifndef __TRACK_H
#define __TRACK_H

#include "stm32f10x.h"

#define TRACK_SENSOR_NUM 5

typedef enum {
    TRACK_SECONDARY_LEFT = 0,
    TRACK_PRIME_LEFT,
    TRACK_MEDIUM,
    TRACK_PRIME_RIGHT,
    TRACK_SECONDARY_RIGHT
} TrackSensor_TypeDef;

typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
} TrackSensorGPIO_TypeDef;

void Track_Init(void);
void Track_RefreshSensors(void);
int Track_GetError(void);
const uint8_t* Track_GetSensorStates(void);
#endif /* __TRACK_H */

