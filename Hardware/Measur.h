#ifndef _Measur_H
#define _Measur_H

extern uint8_t g_dht11_data_ready;
extern uint8_t g_temperature, g_humidity;
extern uint8_t Alam_temperature , Alam_humidity;
void MeasurApp_Process(void);
void MeasurInit(void);
#endif
