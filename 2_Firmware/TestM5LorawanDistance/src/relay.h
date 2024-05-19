#ifndef _RELAY_H_
#define _RELAY_H_

#include <Arduino.h>
#include <Wire.h>

#define RELAY_ADDR                  0x26
#define RELAY_REG_MODE              0x10
#define RELAY_REG_CTRL              0x11



void RL_Init(uint8_t sda, uint8_t scl);
void RL_SetState(uint8_t index, uint8_t state);
uint8_t RL_GetState(uint8_t index);
void RL_Toggle(uint8_t index);


#endif

