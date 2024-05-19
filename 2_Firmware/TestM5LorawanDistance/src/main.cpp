#include <Arduino.h>
#include <Wire.h>
#include <DHT_U.h>

#include "my_wifi.h"
#include "Lorawan.h"
#include "sensor_read.h"
#include "relay.h"

#include <FastLED.h>

#define USE_DHT


const char* staName = "Cloud Coffee";
const char* staPass = "23456789";

// const char* staName = "MANGDAY KTX H1-518 4G";
// const char* staPass = "20202024";

const char* apName = "ESP32 Gateway";
const char* apPass = "123456789";

char str[200];



#define NUM_PACKET_SENT_PERPRSSED           20

#define RELAY_SCL                   G32
#define RELAY_SDA                   G26

#define LORAWAN_BAUDRATE            115200
#define RX_1                        G19
#define TX_1                        G22

#define RX_2                        G33
#define TX_2                        G23

#define RX_3                        G21
#define TX_3                        G25

#define PIN_BUTTON                    G39
#define PIN_LED_RGB                   G27

#define PIN_LED_IR                    G12


#define MAINLOOP_FRE                50

String payload;
String strSer;
String loraMess;

char pubMsg[100];
String strHTTP[200];
uint16_t strHttpCnt;

unsigned long previousMillis = 0; 
const long interval = 5000;
 
int time_now;

uint32_t timeCnt, timeSent;

uint8_t stateLed = 0;

uint16_t packetCnt;

CRGB leds[1];

void setup() {
  uint8_t cnt;



  Serial.begin(115200);

  cnt = 10;
  while(cnt-- > 0){
    Serial.println("Start Init Module!");
    delay(100);
  }

  FastLED.addLeds<SK6812, PIN_LED_RGB, GRB>(leds, 1);

  leds[0] = CRGB ( 0xFF, 0, 0);
  FastLED.show();
  

  // WF_Init((char*)staName, (char*)staPass, (char*)apName, (char*)apPass, 1);

  // while(WF_IsConnected() == 0){
  //   Serial.print(".");
  //   delay(500);
  // }


  // WF_NTP_Init();
  

  LRW_Init_OTAA(RX_3, TX_3, LORAWAN_BAUDRATE);

  // delay(10000);
  
  while(!LRW_StartJoin()){
    Serial.print("Join Fail:"); Serial.println(cnt++);
  }

  if(Serial1.available()){
    payload = Serial1.readString();

    Serial.print("LORA: "); Serial.println(payload);
  }


  Serial.println("/************ BEGIN ************/");

  
}



void SendPacket(uint16_t numberPkts){
  uint16_t cnt;
  char str[100];
  
  Serial.println("Start Send");

  leds[0] = CRGB ( 0xFF, 0, 0);
  FastLED.show();
  
  sprintf(str, "__STARTSTARTSTART__\n");
  LRW_SendMsg(1, 15, str); 
  delay(2000);


  for(cnt = 0; cnt < numberPkts; cnt++){


    sprintf(str, "{\"ID\": %d, \"time\": \"%s\"}\n", packetCnt++, WF_NTP_GetDateTime());
    LRW_SendMsg(1, 15, str); 

    leds[0] = CRGB (0xFF, 0, 0);
    FastLED.show();
    delay(5000);

    leds[0] = CRGB (0, 0, 0);
    FastLED.show();
    delay(100);

  }

  delay(2000);
  sprintf(str, "__ENDENDEND__\n");
  LRW_SendMsg(1, 15, str); 


  Serial.println("End Send");

  leds[0] = CRGB (0, 0xFF, 0);
  FastLED.show();
}


void loop() {
    if(_RevCnt == 0){
      Serial.println("Reconnecting Gateway!");
      while(!LRW_StartJoin()){


      }

      _RevCnt = 2;
    }


  if(Serial.available()){
    payload = Serial.readString();
    //     Serial.print("Received: "); Serial.println(payload);

    if(payload.indexOf("AT") != -1){
      Serial.print("CMD: "); SendCMD(payload);
    } 
    else {
      Serial.print("MSG: "); Serial.println(LRW_SendMsg(1, 15, payload));
    }
    
  }

  if(Serial1.available()){
    payload = Serial1.readString();

    Serial.print("LORA: "); Serial.println(payload);
    
    if(payload.indexOf("OK+RECV") != -1){
      _RevCnt = 2;
    }
    else if(payload.indexOf("ERR+SENT") != -1){
      _RevCnt == 0;
    }
  }

}