#ifndef _BATTERY_H_
#define _BATTERY_H_


#include <Arduino.h>
#include "pin.h"

#include <driver/adc.h>

#define CALIB_VOL2          (12740)
#define CALIB_ADC2          (2800)

#define CALIB_VOL1          (5000)     
#define CALIB_ADC1          (990)       

#define PIN_LEVEL_LOW           (6200)
#define PIN_LEVEL_FULL          (8300)


void BAT_Init(uint8_t pinBat);
uint16_t BAT_GetBat(void);

#endif // !_BATTERY_H_