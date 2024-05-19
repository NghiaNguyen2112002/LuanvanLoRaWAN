#include <Arduino.h>
#include <esp_task_wdt.h>
#include "my_wifi.h"

// #include <FastLED.h>

#include "Lorawan.h"
#include "pin.h"
#include "relay.h"


#define WDT_TIMEOUT                 65

#define LORAWAN_BAUDRATE            115200

#define TIME_READ_SENSOR            120000
#define MAINLOOP_FRE                50

static unsigned long previousMillis = 0; 
static const long interval = 5000;
 
static int time_now;
static uint32_t timeCnt, timeSent;

static String strSer;
static uint8_t flagUplinkRL;
static char buffUplink[200];


const char* staName = "Holmes211";
const char* staPass = "0906631096";


void SendPacket(uint16_t numberPkts){
  uint16_t cnt;
  char str[100];

  Serial.println("Start Send");


  delay(1000);
  
  RL_SetState(3, 1);
  for(int packCnt = 0; packCnt < numberPkts; packCnt++){
    
    Serial.print("Counter: "); Serial.println(packCnt);

    sprintf(str, "{\"ID\": %d, \"TimeSend\": \"%s\"}", packCnt, WF_NTP_GetDateTime().c_str());
    LRW_SendMsg(1, 15, str);


    strSer = "";

    while(strSer.indexOf("OK+RECV") == -1){
      if(Serial1.available()){
        strSer = Serial1.readString();
        Serial.println(strSer);
      }
      
      if(digitalRead(PIN_BT) == 0){
        while (digitalRead(PIN_BT) == 0){
          delay(100);
        }
          LRW_StartJoin();

        break;
      }
    }

    delay(100);
    
  }



  RL_SetState(3, 0);
  Serial.println("End Send");

}


void setup() {
  
  uint8_t cnt;

  Serial1.begin(LORAWAN_BAUDRATE, SERIAL_8N1, PIN_LORA_TX, PIN_LORA_RX);
  Serial.begin(LORAWAN_BAUDRATE);

  
  
  pinMode(PIN_BT, INPUT);
  // pinMode(PIN_LED_RGB, OUTPUT);
  
  RL_Init();

  delay(1000);

  RL_SetState(0, 1);
  WF_Init((char*)staName, (char*)staPass, (char*)NULL, (char*)NULL, 1);

  while(WF_IsConnected() == 0){
    Serial.print(".");
    delay(500);
  }

  
  RL_SetState(1, 1);
  WF_NTP_Init();

  Serial.println("Connect LoRaWAN");

  while(LRW_IsDeviceConnected() != 1);
  Serial.println("LORAWAN Device connected");
  



  // LRW_Init(PIN_LORA_RX, PIN_LORA_TX, LORAWAN_BAUDRATE);

  LRW_StartJoin();
  
  RL_SetState(2, 1);
  timeSent = TIME_READ_SENSOR / MAINLOOP_FRE;
  
  Serial.println("/************ BEGIN ************/");

}

void loop() {

  while(millis() - time_now < MAINLOOP_FRE);

  int dmy = (millis() - time_now) / MAINLOOP_FRE;
  time_now = millis();

  if(digitalRead(PIN_BT) == 0){
    while (digitalRead(PIN_BT) == 0){
      delay(100);
    }
    timeSent = 0;
  }

  if(timeSent == 0){
    timeSent = TIME_READ_SENSOR / MAINLOOP_FRE;
    
    SendPacket(10);
  }

  if(Serial1.available()){
    strSer = Serial1.readString();

    Serial.print("LORA: "); Serial.println(strSer);

    if(strSer.indexOf("OK+RECV") != -1){

      
    }
    else if(strSer.indexOf("ERR+SENT") != -1){
      LRW_StartJoin();
    }
  }

  if(Serial.available()){
    strSer = Serial.readString();
    Serial.print("Received: "); Serial.println(strSer);

    if(strSer.indexOf("AT") != -1){
      SendCMD(strSer);
    }
    else {
      Serial.print("MSG: "); Serial.println(LRW_SendMsg(1, 15, strSer));
    }
    
  }

}

