#include "relay.h"


static uint8_t rlState;

static void WriteReg(uint8_t reg, uint8_t value){
    Wire.beginTransmission(RELAY_ADDR);

    Wire.write(reg);
    Wire.write(value);

    int error = Wire.endTransmission(true);
    Serial.printf("endTransmission: %u\n", error);
}


void RL_Init(uint8_t sda, uint8_t scl){
    rlState = 0;

    Wire.begin(sda, scl, 100000);
    WriteReg(RELAY_REG_MODE, 0x01);
    WriteReg(RELAY_REG_CTRL, rlState);

    // for(int i = 0; i < 10; i++){
    //     delay(500);
    //     rlState = ~rlState;

    //     Serial.print("State: "); Serial.println(rlState);
    // }

}


void RL_SetState(uint8_t index, uint8_t state){
    if(state == 0){
        rlState &= ~(1 << index);
    }
    else {
        rlState |= (1 << index);
       
    }

    WriteReg(RELAY_REG_CTRL, rlState);

}


uint8_t RL_GetState(uint8_t index){
    return (rlState & (1 << index)) > 0;
}


void RL_Toggle(uint8_t index){
    if(rlState & (1 << index)){ // current state 1
        RL_SetState(index, 0);
    }
    else {
        RL_SetState(index, 1);
    }
}


uint8_t RL_GetStateOri(void){
    return (rlState);
}