#include "baterry.h"

uint8_t pinBaterry;


// 77 = 3.6
int16_t CalCalibVol(uint16_t value){
    return (CALIB_VOL2 - CALIB_VOL1) * (value - CALIB_ADC1) / (CALIB_ADC2 - CALIB_ADC1) + CALIB_VOL1;
}

void BAT_Init(uint8_t pinBat){
    pinBaterry = pinBat;
    pinMode(pinBat, INPUT);

}

uint16_t BAT_GetBat(void){
    uint16_t adcValue;
    int16_t vol;
    uint8_t i;
    
    adcValue = 0;

    i = 3;
    while(i--){
        adcValue += analogRead(pinBaterry);
        delay(50);
    }

    adcValue /= 3;
    
    // Serial.print("Milivol: "); Serial.println(analogReadMilliVolts(pinBaterry));
    // Serial.print("Reso: "); Serial.println(analogReadResolution(12));


    vol = CalCalibVol(adcValue);    
    
    // Serial.print("ADC: "); Serial.println(adcValue);
    Serial.print("Vol: "); Serial.println(vol);

    if(vol < PIN_LEVEL_LOW) return 0;
    if(vol > PIN_LEVEL_FULL) return 100;


    // vol = map(vol, PIN_LEVEL_LOW, PIN_LEVEL_FULL, 0, 100);
    
    return (vol-PIN_LEVEL_LOW) * 100 / (PIN_LEVEL_FULL-PIN_LEVEL_LOW);
}