#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "string.h"
#include "dht11.h"
#include "Timer.h"
#include "Serial.h"
volatile uint8_t g_update_display_flag = 0;
volatile uint8_t g_dht11_data_ready = 0;
uint8_t g_temperature = 0, g_humidity = 0;
uint8_t Alam_temperature = 20, Alam_humidity = 30;
void MeasurInit(void)
{
	
	LED_Init();
	OLED_Init();
	DHT11_Init();
	Timer1_Init();
	Serial_Init();
	OLED_ShowString(0, 0, "temp:",OLED_8X16);
	OLED_ShowString(0, 16, "humi:",OLED_8X16);
	OLED_ShowString(0, 32, "Alam_temp:",OLED_8X16);
	OLED_ShowString(0, 64, "Alam_humi:",OLED_8X16);
	OLED_Update();
}

void AlamGet(void)
{
	if(Serial_RxFlag == 1)
	{
		Alam_temperature = Serial_RxPacket[0];
		Alam_humidity = Serial_RxPacket[1];
		OLED_ShowNum(88, 32, Serial_RxPacket[0],2,OLED_8X16);
		OLED_ShowNum(88, 64, Serial_RxPacket[1],2,OLED_8X16);
		OLED_Update();
		Serial_RxFlag = 0;
	}
}
 //处理DHT11数据读取
static void DHT11_Handler(void) 
	{
    if (DHT11_Read_Data() == 0) 
			{
//				g_update_display_flag = 1;  // 触发显示更新
			}
        
       
		if(g_humidity>  Alam_humidity| g_temperature> Alam_temperature)
			{
				LED2_ON();
			}
		else
			{
				LED2_OFF();
			}
  }

static void OLED_Updatefun(void) {
		OLED_ShowNum(64, 0, g_temperature,2,OLED_8X16);
		OLED_ShowNum(64, 16, g_humidity,2,OLED_8X16);
		OLED_Update();
}

//void MeasurApp_Process(void) {
//				AlamGet();
//    if (g_dht11_data_ready) {
//        g_dht11_data_ready = 0;
//        DHT11_Handler();
//				
//				
//				Serial_SendString("temperature：");
//				Serial_SendNumber(g_temperature, 2);
//				Serial_SendString("humidity：");
//				Serial_SendNumber(g_humidity, 2);
//    }
//    
//    if (g_update_display_flag) {
//        g_update_display_flag = 0;
//        OLED_Updatefun();
//    }
//}

