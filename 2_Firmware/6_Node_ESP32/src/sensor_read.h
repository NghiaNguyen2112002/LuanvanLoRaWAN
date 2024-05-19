#ifndef _SENSOR_READ_H_
#define _SENSOR_READ_H_

#include <Arduino.h>

#define INDEX_TEMP                  0
#define INDEX_HUMI                  1

// #define MODE_TEST                   


void SR_Init(uint8_t RX, uint8_t TX, uint32_t baudrate);

float SR_GetTemp(void);
float SR_GetHumi(void);
uint8_t SR_GetHumiTemp(float* temp, float* humi);



#endif // !_SENSOR_READ_H_