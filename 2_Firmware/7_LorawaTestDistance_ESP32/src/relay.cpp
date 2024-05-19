#include "relay.h"


static uint8_t rlPin[NO_RELAY] = {PIN_RL1, PIN_RL2, PIN_RL3, PIN_RL4};
static uint8_t rlState[NO_RELAY];

void RL_Init(void){
    uint8_t i;

    for(i = 0; i < NO_RELAY; i++){
        rlState[i] = 0;
        pinMode(rlPin[i], OUTPUT);
    }


    // for(int i = 0; i < 10; i++){
    //     delay(500);
    //     rlState = ~rlState;

    //     Serial.print("State: "); Serial.println(rlState);
    // }

}


void RL_SetState(uint8_t index, uint8_t state){
    if(index >= NO_RELAY) return;
    
    rlState[index] = state;
    digitalWrite(rlPin[index], state);
}


uint8_t RL_GetState(uint8_t index){
    if(index >= NO_RELAY) return 0;

    return rlState[index];
}


void RL_Toggle(uint8_t index){
    if(index >= NO_RELAY) return;

    if(rlState[index]){ // current state 1
        RL_SetState(index, 0);
    }
    else {
        RL_SetState(index, 1);
    }
}