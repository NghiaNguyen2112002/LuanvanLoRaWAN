#include "LedRGB.h"


static uint8_t pinLed;




static void delay_ns(uint32_t cnt){
    uint32_t i, j;  
    uint32_t cycCnt;

    cycCnt = NS_TO_CYCLES(cnt);


    for(i = 0; i < cycCnt; i++);
    

}

static void WriteReset(void){
    digitalWrite(pinLed, 0);
    delayMicroseconds(DURATION_RESET_US);
}


static void WriteStart(void){
    digitalWrite(pinLed, 1);
    delayMicroseconds(DURATION_RESET_US);
}


static void WriteBit(uint8_t bit){
    if(bit != 0){
        Serial.print(1);
        digitalWrite(pinLed, 1);
        delay_ns(DURATION_1H_NS);     
        digitalWrite(pinLed, 0);
        delay_ns(DURATION_1L_NS);        
    }
    else {
        
        Serial.print(0);
        digitalWrite(pinLed, 1);
        delay_ns(DURATION_0H_NS);     
        digitalWrite(pinLed, 0);
        delay_ns(DURATION_0L_NS);

    }
}




void SK6812_Init(uint8_t ledPin){
    pinLed = ledPin;

    pinMode(pinLed, OUTPUT);

}



void SK6812_Display(uint8_t red, uint8_t green, uint8_t blue){
    uint32_t bitPos;
    uint8_t i;

    // WriteReset();

WriteStart();

// WriteReset();

    for(i = 0; i < 8; i++){
        WriteBit(green & 0x01);
        green >>= 1;
    }
    for(i = 0; i < 8; i++){
        WriteBit(red & 0x01);
        red >>= 1;
    } 
    for(i = 0; i < 8; i++){
        WriteBit(blue & 0x01);
        blue >>= 1;
    }





    WriteReset();

    // digitalWrite(pinLed, 1);

    Serial.println();

}

