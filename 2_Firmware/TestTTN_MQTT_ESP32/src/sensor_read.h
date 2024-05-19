#ifndef _SENSOR_READ_H_
#define _SENSOR_READ_H_

#include <Arduino.h>

#define INDEX_TEMP                  0
#define INDEX_HUMI                  1
#define INDEX_CO                    2
#define INDEX_CO2                   3
#define INDEX_SO2                   4
#define INDEX_NO2                   5
#define INDEX_PM2_5                 6
#define INDEX_O3                    7
#define INDEX_PM10                  8

// #define MODE_TEST                   


void SR_Init(uint8_t RX, uint8_t TX, uint32_t baudrate);
void SR_GetData(uint8_t index);

float SR_GetTemp(void);
float SR_GetHumi(void);
uint16_t SR_GetCO(void);
uint16_t SR_GetCO2(void);
uint16_t SR_GetSO2(void);
uint16_t SR_GetNO2(void);
uint16_t SR_GetPM2_5(void);
uint16_t SR_GetO3(void);
uint16_t SR_GetPM10(void);

void SR_GetTempHumi(float* temp, float* humi);

void SR_GetSoil_PH(float* pH);
void SR_GetSoil_TempHumi(float* sTemp, float* sHumi);
void SR_GetSoil_NPK(uint16_t* nito, uint16_t* phoptpho, uint16_t* kali);
void SR_GetSoil_Conductivity(uint16_t* conductivity);

#endif // !_SENSOR_READ_H_