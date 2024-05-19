#include <Arduino.h>
#include <esp_task_wdt.h>
#include "base64.h"

#include "Lorawan.h"
#include "pin.h"
#include "relay.h"
#include "sensor_read.h"
#include "baterry.h"

#define WDT_TIMEOUT                 65

#define LORAWAN_BAUDRATE            115200

#define TIME_READ_SENSOR            (10*60000)
#define MAINLOOP_FRE                50


#define ENABLE_SLEEP    


static unsigned long previousMillis = 0; 
static const long interval = 5000;
 
static int time_now;
static uint32_t timeCnt, timeSent;

static String strSer;
static uint8_t flagUplinkRL, flagEnableUplinkRL;
static char buffUplink[200];

static float temp, humi;
static uint8_t baterry;

static uint8_t flagSleep;

void EnterSleep(void);

void setup() {
  uint8_t cnt;

  Serial1.begin(LORAWAN_BAUDRATE, SERIAL_8N1, PIN_LORA_TX, PIN_LORA_RX);
  Serial.begin(LORAWAN_BAUDRATE);


  Serial.println("Start Init Module!");

  pinMode(PIN_BT, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  BAT_Init(PIN_ADC_BAT);
  pinMode(PIN_END_LDO_5V, OUTPUT);
  RL_Init();
  SR_Init(PIN_RS485_TX, PIN_RS485_RX, 9600);
  
  // delay(1000);
  cnt = 0;
  digitalWrite(PIN_END_LDO_5V, 0);
  while(cnt--){
    Serial.print("BAT ADC: "); Serial.println(analogRead(PIN_ADC_BAT));
    Serial.print("BAT VOL: "); Serial.println(BAT_GetBat());   
    digitalWrite(PIN_LED, cnt & 0x01);

    delay(1000);
  }

  while(LRW_IsDeviceConnected() != 1);
  Serial.println("LORAWAN Device connected");
  
  cnt = 0;
  while(cnt--){
    Serial.println("Start Init Module!");
    delay(500);
    RL_Toggle(0);
    RL_Toggle(1);
    RL_Toggle(2);
    RL_Toggle(3);
  }

  RL_SetState(0, 0);
  RL_SetState(1, 0);
  RL_SetState(2, 0);
  RL_SetState(3, 0);

  // while(0){
  //   SR_GetHumiTemp(&temp, &humi);  
  //   Serial.print("Temp: "); Serial.println(temp);
  //   Serial.print("Humi: "); Serial.println(humi);
  //   Serial.print("Battery: "); Serial.println(BAT_GetBat());
  //   delay(2000);
  // }


  // LRW_Init(PIN_LORA_TX, PIN_LORA_RX, LORAWAN_BAUDRATE);


  
  LRW_StartJoin();
  
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch

  flagEnableUplinkRL = 1;
  flagSleep = 0;

  Serial.println("/************ BEGIN ************/");

}

void loop() {
  while(millis() - time_now < MAINLOOP_FRE);
  esp_task_wdt_reset();

  int dmy = (millis() - time_now) / MAINLOOP_FRE;
  time_now = millis();

  if(timeSent > dmy){
    timeSent -= dmy;
  }
  else {
    timeSent = 0;
  }

  if(digitalRead(PIN_BT) == 0){
    uint32_t dmy;

    dmy = millis();
    while((digitalRead(PIN_BT) == 0) && (millis() - dmy) < 3000);

    if((millis() - dmy) > 2000){
      flagEnableUplinkRL = 0;
    }
    else {
      timeSent = 0;
    }

  }

  if(timeSent == 0){
    timeSent = TIME_READ_SENSOR / MAINLOOP_FRE;
    
    // RL_SetState(0, 1);
    
    while(Serial1.available()){
      Serial1.read();
    }

    uint8_t cnt = 3;

    while((cnt) && (SR_GetHumiTemp(&temp, &humi) == 0)){
      cnt--;
      delay(2000);
    }

    RL_SetState(0, 0);
    
    if(cnt != 0){
      sprintf(buffUplink, "{\"air\":{\"temp\":%.02f,\"humi\":%.02f},\"relay\":{\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}, \"bat\":%d}",
        temp, humi, RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3), BAT_GetBat()
      );  
    }   
    else {
      sprintf(buffUplink, "{\"relay\":{\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}, \"bat\":%d}",
        RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3), (int)BAT_GetBat()
      );  
    }


    Serial.print("Temp: "); Serial.println(temp);
    Serial.print("Humi: "); Serial.println(humi);
    Serial.print("Battery: "); Serial.println(BAT_GetBat());

    LRW_SendMsg(1, 15, buffUplink);
  }

  if(Serial1.available()){
    strSer = Serial1.readString();

    Serial.print("LORA: "); Serial.println(strSer);

    if(strSer.indexOf("OK+RECV:") != -1){

      flagSleep = 1;

      
      LRW_SendMsg(1, 15, buffUplink);
    }

    if(strSer.indexOf("OK+RECV:") != -1 && strSer.indexOf("02,00,00") == -1) {
      strSer = strSer.substring(strSer.indexOf("OK+RECV:") + 17);
      
      if(strSer.length() > 9){
        
        Serial.print("Received: "); Serial.println(strSer);
        uint8_t state1, state2, state3, state4;

        state1 = strSer[1] - '0';
        state2 = strSer[3] - '0';
        state3 = strSer[5] - '0';
        state4 = strSer[7] - '0';
        flagUplinkRL = strSer[9] - '0';

        RL_SetState(0, state1);
        RL_SetState(1, state2);
        RL_SetState(2, state3);
        RL_SetState(3, state4);

        Serial.println("Update Relay!");
        Serial.print("Relay 1: "); Serial.print(state1 != 0 ? "ON":"OFF");
        Serial.print("; Relay 2: "); Serial.print(state2 != 0 ? "ON":"OFF");
        Serial.print("; Relay 3: "); Serial.print(state3 != 0 ? "ON":"OFF");
        Serial.print("; Relay 4: "); Serial.println(state4 != 0 ? "ON":"OFF");

      }
      

      // SendCMD("AT+DRX?\r\n");
      // WaitMsg(100);
      // if(strSer.indexOf("A1") != -1){
      //   flagUplinkRL = 1;
      //   Serial.println("Relay 1 Off!");
      //   RL_SetState(0, 0);
      // }
      // else if(strSer.indexOf("B1") != -1){
      //   flagUplinkRL = 1;
      //   Serial.println("Relay 1 On!");
      //   RL_SetState(0, 1);
      // }

      // else if(strSer.indexOf("A2") != -1){
      //   flagUplinkRL = 1;
      //   Serial.println("Relay 2 Off!");
      //   RL_SetState(1, 0);
      // }
      // else if(strSer.indexOf("A3") != -1){
      //   flagUplinkRL = 1;
      //   Serial.println("Relay 3 Off!");
      //   RL_SetState(2, 0);
      // }
      // else if(strSer.indexOf("A4") != -1){
      //   flagUplinkRL = 1;
      //   Serial.println("Relay 4 Off!");
      //   RL_SetState(3, 0);
      // }
      // else if(strSer.indexOf("B2") != -1){
      //   flagUplinkRL = 1;
      //   Serial.println("Relay 2 On!");
      //   RL_SetState(1, 1);
      // }
      // else if(strSer.indexOf("B3") != -1){
      //   flagUplinkRL = 1;
      //   Serial.println("Relay 3 On!");
      //   RL_SetState(2, 1);
      // }
      // else if(strSer.indexOf("B4") != -1){
      //   flagUplinkRL = 1;
      //   Serial.println("Relay 4 On!");
      //   RL_SetState(3, 1);
      // }

    }
    else if(strSer.indexOf("ERR+SEN") != -1){
      // LRW_StartJoin();
      flagSleep = 1;

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

  if(flagUplinkRL && flagEnableUplinkRL){
    flagUplinkRL = 0;
    sprintf(buffUplink, "{\"relay\":{\"RL1\":%d,\"RL2\":%d,\"RL3\":%d,\"RL4\":%d}}", RL_GetState(0), RL_GetState(1), RL_GetState(2), RL_GetState(3));
    LRW_SendMsg(1, 15, buffUplink);

    digitalWrite(PIN_LED, 0);
  }
  else {
    digitalWrite(PIN_LED, 1);

  }


#ifdef ENABLE_SLEEP
  
  if(flagSleep){
      EnterSleep();
  }
#endif


}



void EnterSleep(void){

  Serial.println("Enter sleep mode!");

  delay(5000);

  Serial.end();
  Serial1.end();
  Serial2.end();


  pinMode(PIN_LORA_RX, INPUT);
  pinMode(PIN_LORA_TX, INPUT);

  pinMode(PIN_RS485_RX, INPUT);
  pinMode(PIN_RS485_RX, INPUT);

  pinMode(PIN_RL1, INPUT);
  pinMode(PIN_RL2, INPUT);
  pinMode(PIN_RL3, INPUT);
  pinMode(PIN_RL4, INPUT);

  pinMode(PIN_LED, INPUT);
  pinMode(PIN_END_LDO_5V, INPUT);
  esp_sleep_enable_ext0_wakeup(PIN_BT, 0);
  esp_sleep_enable_timer_wakeup(TIME_READ_SENSOR * 1000);   

  esp_deep_sleep_start();
  

}