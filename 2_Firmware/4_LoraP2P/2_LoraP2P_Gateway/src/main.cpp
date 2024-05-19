#include <Arduino.h>
#include <LoRa.h>
#include <string>
#include "string.h"
#include "my_wifi.h"
#include "my_base64.h"


#define EXCECUTING_CYCLE               50
#define NODE_ID                         0


#define PIN_SS            D8
#define PIN_RST           -1
#define PIN_DIO0          D1

#define WIFI_DEFAULT_NAME           ("MANG DAY KTX H1-519")
#define WIFI_DEFAULT_PASS           ("123456789a@")

// #define MQTT_SERVER "eu1.cloud.thethings.network"
// #define MQTT_PORT 1883
// #define MQTT_USER "test-atom-eu1@ttn"
// #define MQTT_PASS "NNSXS.PAMWVMNQ6WJZ5LBZYUI77YIUGCLWK6HPO2LC6MQ.KVTF3JWV5KMTIRBKAIIANKXQXRWM5DXFVPHJS5ZPXFK75OQRQGCQ"

// #define MQTT_TOPIC_UPLINK       "v3/test-atom-eu1@ttn/devices/class-c-abp/up"
// #define MQTT_TOPIC_DOWNLINK     "v3/test-atom-eu1@ttn/devices/class-c-abp/down/push"
// #define CLIENT_ID  "class-c-abp"

#define MQTT_SERVER "mqttserver.tk"
#define MQTT_PORT 1883
#define MQTT_USER "innovation"
#define MQTT_PASS "Innovation_RgPQAZoA5N"
#define MQTT_PUB_TOPIC  "/innovation/airmonitoring/LoRaWAN"
#define CLIENT_ID  "NhomLoraWAN-P2P"


char str[100];
uint8_t loraBuff[50];
String serialStr;
uint16_t length;
String subMsgTopic;

float temp, humi;
uint8_t id, rlState, rlState1, rlState2, rlState3, rlState4;


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


void ProcessDownlink(String str){
  uint16_t i, j;
  int16_t indexFrm;
  String processStr;
  
  // "frm_payload":"EQ==","correlation_ids"

  indexFrm = str.indexOf("frm_payload");
  if(indexFrm != -1){
    i = str.indexOf(":\"", indexFrm);
    i = i+2;
    j = str.indexOf("\"", i);

    processStr = str.substring(i, j);

    Serial.print("Payload: "); Serial.println(processStr);

    processStr = BASE64_Decode(processStr);

    
    Serial.print("Base64 Decoded Payload: ");
    for(i = 0; i < processStr.length(); i++){
      Serial.print("0x"); Serial.print(processStr[i], HEX); Serial.print(" ");
    } 
    Serial.println();
  }
}


#endif


void setup() {
  // put your setup code here, to run once:
  Serial.setRxBufferSize(2048);
  Serial.begin(115200);


  LoRa.setPins(PIN_SS, PIN_RST, PIN_DIO0);

  if( !LoRa.begin(440E6) ){

    while(!LoRa.begin(440E6)){
      Serial.println("Starting Lora failed!");
      delay(2000);
    }

  }

  LoRa.setSyncWord(0x78);

  LoRa.receive();

  WF_Init(WIFI_DEFAULT_NAME, WIFI_DEFAULT_PASS, NULL, NULL, 0);

  while(WF_IsConnected() == 0){
    Serial.print(".");
    delay(500);
  }

  WF_MQTT_Connect(MQTT_SERVER, MQTT_PORT, CLIENT_ID, MQTT_USER, MQTT_PASS);


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


  
    int packetSize = LoRa.parsePacket();
    if(packetSize){

      Serial.println("Received'");
    
      // read packet
      for (int i = 0; i < packetSize; i++) {
        loraBuff[i] = (char)LoRa.read();
        Serial.print(loraBuff[i], HEX); Serial.print(" ");
      }

      ProcDecode(loraBuff, &id, &temp, &humi, &rlState);

      char str[100];
      uint8_t size;
      rlState1 = (rlState & (1 << 0)) > 0;
      rlState2 = (rlState & (1 << 1)) > 0;
      rlState3 = (rlState & (1 << 2)) > 0;
      rlState4 = (rlState & (1 << 3)) > 0;

      size = sprintf(str, "{\"id\": %d, \"temp\": %.02f, \"humi\": %.02f, \"RL1\": %d, \"RL2\": %d, \"RL3\": %d, \"RL4\": %d}",
        id, temp, humi, rlState1, rlState2, rlState3, rlState4
      );

      WF_MQTT_Publish(MQTT_PUB_TOPIC, str, size);

    }




    if(WF_IsConnected() == 0){
      Serial.println("Connect Wifi!");
      WF_Init(WIFI_DEFAULT_NAME, WIFI_DEFAULT_PASS, NULL, NULL, 0);
      
      while(WF_IsConnected() == 0){
        Serial.println(".");
        delay(500);
      }
    }

    
    if(WF_MQTT_IsConnected() == 0){
      Serial.println("Connect Server!");
     
      WF_MQTT_Connect(MQTT_SERVER, MQTT_PORT, CLIENT_ID, MQTT_USER, MQTT_PASS);
    
      // WF_MQTT_Subscribe(MQTT_TOPIC_DOWNLINK);
      // WF_MQTT_Subscribe("#");
      
    }

    if(WF_MQTT_GetMsg(&subMsgTopic)){
      // ProcessDownlink(subMsgTopic);


      /* Send cmd to Node */


    }


    WF_MQTT_ClientLoop();
      
    if(Serial.available()){
      serialStr = Serial.readString();
      Serial.println(serialStr);  
    }
  }
  


  

  // if(Serial.available()){
  //   serialStr = Serial.readString();
  //   if(serialStr.indexOf("UP") != -1){
  //     WF_MQTT_Publish(MQTT_TOPIC_UPLINK, (const char*) serialStr.c_str() + 2, 0);
  //   }
  //   else if(serialStr.indexOf("DOWN") != -1){
  //     WF_MQTT_Publish(MQTT_TOPIC_DOWNLINK, (const char*) serialStr.c_str() + 4, 0);
  //   }
  //   else {
  //     WF_MQTT_Publish(MQTT_TOPIC_UPLINK, (const char*) serialStr.c_str(), 0);
  //   }
  // }

}

