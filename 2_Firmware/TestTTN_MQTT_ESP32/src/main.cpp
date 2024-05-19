#include <Arduino.h>

#include "my_wifi.h"
#include "Lorawan.h"
#include "sensor_read.h"


const char* staName = "PonPetShop";
const char* staPass = "21122023";

// const char* staName = "MANGDAY KTX H1-518 4G";
// const char* staPass = "20202024";

const char* apName = "ESP32 Gateway";
const char* apPass = "123456789";

#define LORAWAN_BAUDRATE            115200
// #define RX_1                        G19     //17
// #define TX_1                        G22     //5

// #define RX_2                        G33     //4
// #define TX_2                        G23     //16

#define RX_1                        17
#define TX_1                        5

#define RX_2                        4
#define TX_2                        16

// #define PIN_BTN                     G39
// #define PIN_RGB                     G27

#define PIN_BTN                     5
#define PIN_RGB                     18
String payload;
String strSer;
String loraMess;

unsigned long previousMillis = 0; 
const long interval = 5000;
 
int time_now;

uint16_t timeCnt;

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

  WF_Init((char*)staName, (char*)staPass, (char*)apName, (char*)apPass, 1);
  // WF_Init(NULL, NULL, (char*)apName, (char*)apPass, 1);


  
  // pinMode(PIN_RGB, OUTPUT);

  // for(int i = 0; i < 255; i += 10){
  //   analogWrite(PIN_RGB, i);
  //   delay(100);
  // }

  Serial.println("Init Sensor");
  SR_Init(RX_2, TX_2, 9600);

  Serial.println("Init LoraWAN");

  LRW_Init_OTAA(RX_1, TX_1, LORAWAN_BAUDRATE);
  
  delay(10000);
  
  while(!Serial1.available());

  if(Serial1.available()){
    payload = Serial1.readString();

    Serial.print("LORA: "); Serial.println(payload);
  }
 
  Serial.println("/************ BEGIN ************/");

  
}



void loop() {
  char str[200];

  while(millis() - time_now < 1000);

  time_now = millis();

  timeCnt++;

  if(stateLed){
    stateLed = 0;
    // digitalWrite(PIN_RGB, HIGH);
    
  }
  else {
    stateLed = 1;
    // digitalWrite(PIN_RGB, LOW);
  }
  // stateLed = !stateLed;
  // Serial.print("LED: ");  Serial.println(stateLed);

  if((timeCnt % 100) == 0){
    
    SR_GetTempHumi(&temp, &humi);
    SR_GetSoil_TempHumi(&sTemp, &sHumi);
    SR_GetSoil_PH(&sPH);
    SR_GetSoil_NPK(&sNito, &sPhotpho, &sKali);
    SR_GetSoil_Conductivity(&sConductivity);

    // temp = SR_GetTemp();
    // humi = SR_GetHumi();
    // CO = SR_GetCO();
    // CO2 = SR_GetCO2();
    // SO2 = SR_GetSO2();
    // NO2 = SR_GetNO2();
    // PM25 = SR_GetPM2_5();
    // O3 = SR_GetO3();
    // PM10 = SR_GetPM10();



    // sprintf(str, "{\"temp\":%.02f,\"humi\":%.02f,\"CO\":%d,\"CO2\":%d,\"SO2\":%d,\"NO2\":%d,\"PM25\":%d,\"O3\":%d,\"PM10\":%d}",
    //     temp, humi, CO, CO2, SO2, NO2, PM25, O3, PM10
    // );  
    sprintf(str, "{\"temp\":%.02f,\"humi\":%.02f,\"sTemp\":%.02f,\"sHumi\":%.02f,\"sPH\":%.02f,\"sN\":%d,\"sP\":%d,\"sK\":%d,\"sCon\":%d}",
    
    temp, humi, sTemp, sHumi, sPH, sNito, sPhotpho, sKali, sConductivity );  
    Serial.println("DATA: "); Serial.println(str);

    LRW_SendMsg(1, 15, str);
  }

  // delay(10000);
  // if(Serial.available()){
  //   strSer = Serial.readString();
    
  //   Serial.print("ESPNow sending: "); Serial.println(strSer);
  //   WF_SendESPNOW((uint8_t*)strSer.c_str(), strSer.length());
  // }


  if(Serial1.available()){
    payload = Serial1.readString();

    Serial.print("LORA: "); Serial.println(payload);
    
    if(payload.indexOf("RECV") != -1){
      LRW_SendMsg(1, 15, str);

    }   
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

}