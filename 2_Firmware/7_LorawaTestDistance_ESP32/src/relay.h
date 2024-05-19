#ifndef _RELAY_H_
#define _RELAY_H_

#include <Arduino.h>
#include "pin.h"

#define NO_RELAY                4


void RL_Init(void);
void RL_SetState(uint8_t index, uint8_t state);
uint8_t RL_GetState(uint8_t index);
void RL_Toggle(uint8_t index);


#endif

