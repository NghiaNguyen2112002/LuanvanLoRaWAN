#include <Arduino.h>
#include <LoRa.h>
#include "my_wifi.h"
#include "my_base64.h"
#include "relay.h"
#include "sensor_read.h"

#define EXCECUTING_CYCLE               50
#define NODE_ID                         0

#define PIN_SS            GPIO_NUM_15 
#define PIN_RST           -1
#define PIN_DIO0          GPIO_NUM_37

// #define SCK               GPIO_NUM_18
// #define MISO              GPIO_NUM_19
// #define MOSI              GPIO_NUM_13
// #define SS                GPIO_NUM_15

#define TX1               16
#define RX1               17

char str[100];
uint8_t loraBuff[50];
String serialStr;
uint16_t length;
String subMsgTopic;

float temp, humi;
uint8_t rlState, rlState1, rlState2, rlState3, rlState4;


static uint64_t timeNow;
static uint16_t timeNowCnt;


static uint16_t timeReadSensor;


//==================LOW LEVEL FUNCTION==================//
#if 1
uint8_t CalcCRC8(uint8_t* pos, uint16_t bitLength) {
    uint16_t crc, remaining;
    uint16_t data;
    uint8_t bitPos, bytePos;
    uint16_t i, j;

    crc = 0x018D;           // x^8+x^7+x^3+x^2+1
    bitPos = 0x80;
    bytePos = 0;    
    data = 0;
    remaining = 0;

    for (i = 0; i < bitLength; i++) {
        remaining <<= 1;
        if (data & bitPos) {
            remaining |= 1;
        }

        if (remaining & 0x0100) {
            remaining ^= crc;
        }

        bitPos >>= 1;
        if (bitPos == 0) {
            bitPos = 0x80;
            bytePos++;

            data = *(pos + bytePos);
        }
    }

    return remaining & 0x00FF;
}



/* This function return the length of encoded string */
uint8_t ProcEncode(uint8_t* str, uint8_t id, float temp, float humi, uint8_t rlState){
    /* Format: 
    *      length(8b) |  ID (8b) | tempHI(8b) | tempLO(8b) | humiHI(8b) | humiLO(8b) | rlState(8b) | CRC (8b)
    *           0         1            2             3           4             5          6           7
    */


    uint16_t dmy;
	  uint16_t i;

    i = 1;
    str[i++] = id;
    
    dmy = (uint16_t)temp;
    str[i++] = dmy;
    str[i++] = (uint16_t)((temp - dmy) * 100.0);;

    dmy = (uint16_t)humi;
    str[i++] = dmy;
    str[i++] = (uint16_t)((humi - dmy) * 100.0);;

    str[i++] = rlState;
    
    str[i] = 0;

    str[0] = i+1;

    dmy = CalcCRC8(str, (i+1)*8);

    str[i] = dmy;       //CRC 8

    return i+1;      //str size  = i
}

/* This function return whether the received string is valid */
uint8_t ProcDecode(uint8_t* str, uint8_t* id, float* temp,  float* humi, uint8_t* rlState){
    if(CalcCRC8(str, str[0]*8) != 0){
        return 0;   //invalid
    }

    /* This string valid */
    *id = str[1];
    *temp = str[2]*1.0 + str[3] / 100.0;;
    *humi = str[4]*1.0 + str[5] / 100.0;;
    *rlState = str[6];

    return str[0];

}


#endif


void setup() {
  // put your setup code here, to run once:  
  Serial.setRxBufferSize(2048);
  Serial.begin(115200);

  SR_Init(RX1, TX1, 9600);

  LoRa.setPins(PIN_SS, PIN_RST, PIN_DIO0);

  if( !LoRa.begin(440E6) ){

    while(!LoRa.begin(440E6)){
      Serial.println("Starting Lora failed!");
      delay(2000);
    }

  }

  LoRa.setSyncWord(0x78);

  LoRa.idle();

  Serial.println("Begin");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() >= timeNow + EXCECUTING_CYCLE){
    timeNowCnt = (millis() - timeNow) / EXCECUTING_CYCLE;
    timeNow = millis();


    if(timeReadSensor > timeNowCnt){
      timeReadSensor -= timeNowCnt;
    }
    else {
      timeReadSensor = 0;
    }

    if(timeReadSensor == 0){
      timeReadSensor = 10000 / EXCECUTING_CYCLE;

      SR_GetTempHumi(&temp, &humi);
      rlState = RL_GetStateOri();

      Serial.println("Send Lora");
      LoRa.beginPacket();
      LoRa.write((uint8_t*)loraBuff,  ProcEncode((uint8_t*)loraBuff, NODE_ID, temp, humi, rlState) );
      
      
      LoRa.endPacket();

   

    }

    int packetSize = LoRa.parsePacket();
    if(packetSize){

      Serial.println("Received'");
    
      // read packet
      for (int i = 0; i < packetSize; i++) {
        loraBuff[i] = (char)LoRa.read();
        Serial.print(loraBuff[i], HEX); Serial.print(" ");
      }

    }
  



    if(Serial.available()){
      serialStr = Serial.readString();
      
    }
  }


  delay(50);
}

