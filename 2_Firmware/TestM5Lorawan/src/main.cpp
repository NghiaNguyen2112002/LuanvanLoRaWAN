#include <Arduino.h>
#include <Wire.h>
#include <DHT_U.h>

#include "my_wifi.h"
#include "Lorawan.h"
#include "sensor_read.h"
#include "relay.h"

#define USE_DHT


const char* staName = "Cloud Coffee";
const char* staPass = "23456789";

// const char* staName = "MANGDAY KTX H1-518 4G";
// const char* staPass = "20202024";

const char* apName = "ESP32 Gateway";
const char* apPass = "123456789";

char str[200];

#ifdef USE_DHT
  #define DHTTYPE    DHT11     // DHT 11
  #define PIN_DHT11   G32
  DHT dht(PIN_DHT11, DHTTYPE);
#endif // USE_DHT



#define RELAY_SCL                   G32
#define RELAY_SDA                   G26

#define LORAWAN_BAUDRATE            115200
#define RX_1                        G19
#define TX_1                        G22

#define RX_2                        G33
#define TX_2                        G23

#define RX_3                        G21
#define TX_3                        G25

#define PIN_BTN                     G39
#define PIN_RGB                     G27


#define MAINLOOP_FRE                50

String payload;
String strSer;
String loraMess;

unsigned long previousMillis = 0; 
const long interval = 5000;
 
int time_now;

uint32_t timeCnt, timeSent;

uint8_t stateLed = 0;

float temp = 0, humi = 0;
uint16_t CO = 0, CO2 = 0, SO2 = 0, NO2 = 0, PM25 = 0, PM10 = 0, O3 = 0;


float sTemp = 0, sHumi = 0, sPH = 0;
uint16_t sNito = 0, sPhotpho = 0, sKali = 0, sConductivity = 0;

void setup() {
  uint8_t cnt;



  Serial.begin(115200);

  cnt = 10;
  while(cnt-- > 0){
    Serial.println("Start Init Module!");
    delay(100);
  }


  // while(Serial.available() == 0){
  //   Serial.println("Enter something");  
  //   delay(500);
  // }
  // strSer = Serial.readString();

  // WF_Init((char*)staName, (char*)staPass, (char*)apName, (char*)apPass, 1);
  // WF_Init(NULL, NULL, (char*)apName, (char*)apPass, 1);
  
  pinMode(PIN_RGB, OUTPUT);

  for(int i = 0; i < 255; i += 10){
    analogWrite(PIN_RGB, i);
    delay(10);
  }

  Serial.println("Init Relay");
  RL_Init(RELAY_SDA, RELAY_SCL);

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 2; j++){
      RL_Toggle(i);
      delay(200);
    }

  }

  Serial.println("Init Sensor");
  SR_Init(RX_2, TX_2, 9600);

  Serial.println("Init LoraWAN");

  LRW_Init_OTAA(RX_3, TX_3, LORAWAN_BAUDRATE);

  // delay(10000);
  
  while(!LRW_StartJoin()){
    Serial.print("Join Fail:"); Serial.println(cnt++);
  }

  if(Serial1.available()){
    payload = Serial1.readString();

    Serial.print("LORA: "); Serial.println(payload);
  }
 

#ifdef USE_DHT
  dht.begin();

#endif // USE_DHT

  Serial.println("/************ BEGIN ************/");

  
}



void loop() {
  while(millis() - time_now < MAINLOOP_FRE);

  // if(millis() - time_now > MAINLOOP_FRE){
    int dmy = (millis() - time_now) / MAINLOOP_FRE;
    time_now = millis();


    timeCnt++;

    if(timeSent > dmy){
      timeSent -= dmy;
    }
    else {
      timeSent = 0;
    }

    int cnt = 0;

    if(_RevCnt == 0){
      Serial.println("Reconnecting Gateway!");
      while(!LRW_StartJoin()){
      Serial.print("Join Fail:"); Serial.println(cnt++);

      }

      _RevCnt = 2;
    }


    if(timeSent == 0){
      timeSent = 600000 / MAINLOOP_FRE;
    
      RL_SetState(0, 1);

      delay(10000);

      // while( (SR_GetTempHumi(&temp, &humi) == 0)){
      //   delay(2000);
      // }

      if((SR_GetTempHumi(&temp, &humi) == 0)){
        delay(2000);
      }


      SR_GetTempHumi(&temp, &humi);
      SR_GetSoil_TempHumi(&sTemp, &sHumi);
      SR_GetSoil_PH(&sPH);
      SR_GetSoil_NPK(&sNito, &sPhotpho, &sKali);
      SR_GetSoil_Conductivity(&sConductivity);

      RL_SetState(0, 0);

#ifdef USE_DHT
      temp = dht.readTemperature();
      humi = dht.readHumidity();
#endif // USE_DHT

      sprintf(str, "{\"air\":{\"temp\":%.02f,\"humi\":%.02f},\"soil\":{\"sTemp\":%.02f,\"sHumi\":%.02f,\"sPH\":%.02f,\"sN\":%d,\"sP\":%d,\"sK\":%d,\"sCon\":%d},\"relay\":{\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}}",
        temp, humi, sTemp, sHumi, sPH, sNito, sPhotpho, sKali, sConductivity, RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3) 
      );  

      // {"air":{"temp":0.00,"humi":0.00},"soil":{"sTemp":0.00,"sHumi":0.00,"sPH":0.00,"sN":0,"sP":0,"sK":0,"sCon":0},"relay":{"RL1":0,"RL2":0,"RL3":0,"RL4":0}}

      // sprintf(str, "{\"temp\":%.02f,\"humi\":%.02f,\"sTemp\":%.02f,\"sHumi\":%.02f,\"sPH\":%.02f,\"sN\":%d,\"sP\":%d,\"sK\":%d,\"sCon\":%d,\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}",                                        
      
      // temp, humi, sTemp, sHumi, sPH, sNito, sPhotpho, sKali, sConductivity, RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3) );  
      Serial.println("DATA: "); Serial.println(str);

      // Serial.println(str);
      LRW_SendMsg(1, 15, str);
    
    
    }      

  // }

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
      if(payload.indexOf("A1") != -1){
        Serial.println("Button 1 pressed!");
        RL_Toggle(0);
        
        sprintf(str, "{\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}",                                        
              RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3) ); 
        LRW_SendMsg(1, 15, str);
      }
      else if(payload.indexOf("A2") != -1){
        Serial.println("Button 2 pressed!");
        RL_Toggle(1);
        
        sprintf(str, "{\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}",                                        
              RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3) ); 
        LRW_SendMsg(1, 15, str);
      }
      else if(payload.indexOf("A3") != -1){
        Serial.println("Button 3 pressed!");
        RL_Toggle(2);

        sprintf(str, "{\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}",                                        
              RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3) ); 
        LRW_SendMsg(1, 15, str);      
      }
      else if(payload.indexOf("A4") != -1){
        Serial.println("Button 4 pressed!");
        RL_Toggle(3);
        
        sprintf(str, "{\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}",                                        
              RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3) ); 
        LRW_SendMsg(1, 15, str);      
        
      }
    }
    else if(payload.indexOf("ERR+SENT") != -1){
      _RevCnt == 0;
    }
  }







}