#include <Arduino.h>


#include "my_wifi.h"
#include "my_base64.h"
#include "E220_lora_p2p.h"

#include "LedRGB.h"

#include <FastLED.h>


// #define SENSOR_NODE 
#define GATEWAY

#ifdef GATEWAY
    #define LORA_OWN_CHANNEL                  0x00
    #define LORA_TARTGET_CHANNEL              0x00

    #define LORA_OWN_ADDR                   0x0000
    #define LORA_TARGET_ADDR                0x0000
    
    // const char* staName = "Holmes211";
    // const char* staPass = "0906631096";
    // const char* staName = "iPhone của 10";
    // const char* staPass = "15112002";
#endif // GATEWAY



#ifdef SENSOR_NODE
    #define LORA_OWN_CHANNEL                  0x00
    #define LORA_TARTGET_CHANNEL              0x00

    #define LORA_OWN_ADDR                   0x0000
    #define LORA_TARGET_ADDR                0x0000

        
    // const char* staName = "Holmes211";
    // const char* staPass = "0906631096";

    // const char* staName = "iPhone của 10";
    // const char* staPass = "15112002";
#endif // SENSOR_NODE
    


#define START_CHAR              0xFE
#define STOP_CHAR               0xFC

#define NUM_PACKET_SENT_PERPRSSED           100

const char* staName = "Ken House Coffee";
const char* staPass = "12345678";

// const char* staName = "PonPetShop";
// const char* staPass = "21122023";

const char* apName = NULL;
const char* apPass = NULL;
// const char* apName = "ESP32 Gateway";
// const char* apPass = "123456789";

#define PIN_LORA_RX                   G26
#define PIN_LORA_TX                   G32

#define PIN_BUTTON                    G39
#define PIN_LED_RGB                   G27

#define PIN_LED_IR                    G12


uint8_t colorLed;

LoRaConfigItem_t config;
RecvFrame_t data;

String str;
int i, j;

unsigned long previousMillis = 0; 
const long interval = 5000;
 
int time_now;

uint32_t timeCnt, timeSent;

char pubMsg[100];
String strHTTP[200];
uint16_t strHttpCnt;

uint16_t packetCnt;

uint8_t flagPktCplt;

uint32_t ledRGBColor;

void SendPacket(uint16_t numberPkts);
void GatewayLoop(void);
void NodeLoop(void);


CRGB leds[1];

void delay_ns(uint32_t cnt){
    uint32_t i, j;  
    uint32_t cycCnt;

    cycCnt = NS_TO_CYCLES(cnt);
    Serial.println(cycCnt);

    uint32_t dmy;
    dmy = millis();

    for(i = 0; i < cycCnt; i++){
      j++;
    }

    Serial.print("Delay"); Serial.println((int) (millis() - dmy));

}


void setup() {
  uint8_t cnt;

  FastLED.addLeds<SK6812, PIN_LED_RGB, GRB>(leds, 1);

  leds[0] = CRGB ( 0xFF, 0, 0);
  FastLED.show();
  
  
  // for (int i = 0; i <= 19; i++) {
  //   leds[i] = CRGB ( 0xFF, 0xFF, 0xFF);
  //   FastLED.show();
  //   delay(40);
  // }
  // for (int i = 19; i >= 0; i--) {
  //   leds[i] = CRGB ( 0xFF, 0xFF, 0xFF);
  //   FastLED.show();
  //   delay(40);
  // }


  // put your setup code here, to run once:
  delay(3000);
  Serial.begin(115200);

    config.baud_rate                = BAUD_9600;
    config.air_data_rate            = BW125K_SF9;
    config.subpacket_size           = SUBPACKET_200_BYTE;
    config.rssi_ambient_noise_flag  = RSSI_AMBIENT_NOISE_ENABLE;
    config.transmitting_power       = TX_POWER_13dBm;
    config.rssi_byte_flag           = RSSI_BYTE_ENABLE;
    config.transmission_method_type = UART_P2P_MODE;
    config.lbt_flag                 = LBT_DISABLE;
    config.wor_cycle                = WOR_2000MS;
    config.encryption_key           = 0x1234;

    config.own_address              = LORA_OWN_ADDR;
    config.own_channel              = LORA_OWN_CHANNEL;

    config.target_address           = LORA_TARGET_ADDR;
    config.target_channel           = LORA_TARTGET_CHANNEL;



  packetCnt = 0;

  cnt = 10;
  while(cnt-- > 0){
    Serial.print(cnt); Serial.println(" Start Init Module!");
    delay(100);
  }
  
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED_RGB, OUTPUT);
  pinMode(PIN_LED_IR, OUTPUT);

  digitalWrite(PIN_LED_IR, 1);
  delay(2000);
  digitalWrite(PIN_LED_IR, 0);
  delay(2000);


  SK6812_Init(PIN_LED_RGB);


  E220_Init(PIN_LORA_RX, PIN_LORA_TX, 9600);
  delay(100);

  WF_Init((char*)staName, (char*)staPass, (char*)apName, (char*)apPass, 1);

  while(WF_IsConnected() == 0){
    Serial.print(".");
    delay(500);
  }


  WF_NTP_Init();


  // WF_MQTT_Connect(MQTT_SERVER, MQTT_PORT, CLIENT_ID, MQTT_USER, MQTT_PASS);
  // WF_MQTT_Subscribe("#");


  
  if(E220_Setting(&config) == 0){
    Serial.println("E220 Init failed!");
  }

  // Serial.println(CLIENT_ID);

  // delay_ns(2000000000);

  // delay_ns(100000);
  // delay_ns(500000);

  //     SK6812_Display(0xFF, 0, 0);

  // while(1){
  //   delay(1000);
  // }
  // delay(2000);
  
  // SK6812_Display(0, 0xFF, 0);
  // delay(2000);

  // SK6812_Display(0, 0, 0xFF);
  // delay(2000);


  Serial.println("/************ BEGIN ************/");



  
}

void loop() {




  // put your main code here, to run repeatedly:
  #ifdef GATEWAY
    GatewayLoop();
  #endif // GATEWAY

  #ifndef GATEWAY
    NodeLoop();
  #endif // GATEWAY
}



void SendPacket(uint16_t numberPkts){
  uint16_t cnt;
  char str[100];
  
  Serial.println("Start Send");

  leds[0] = CRGB ( 0xFF, 0, 0);
  FastLED.show();
  

  E220_SendFrame(&config, (uint8_t*)str, sprintf(str, "__STARTSTARTSTART__\n"));
  delay(2000);


  for(cnt = 0; cnt < numberPkts; cnt++){


    E220_SendFrame(&config, (uint8_t*)str, sprintf(str, "{\"ID\": %d, \"time\": \"%s\"}\n", packetCnt++, WF_NTP_GetDateTime()));
    
    leds[0] = CRGB (0xFF, 0, 0);
    FastLED.show();
    delay(100);

    leds[0] = CRGB (0, 0, 0);
    FastLED.show();
    delay(100);

  }

  delay(2000);
  E220_SendFrame(&config, (uint8_t*)str, sprintf(str, "__ENDENDEND__\n"));

  Serial.println("End Send");

  leds[0] = CRGB (0, 0xFF, 0);
  FastLED.show();
}


void GatewayLoop(void){
  if(digitalRead(PIN_BUTTON) == 0){
    uint32_t cntMs;

    cntMs = millis();
    while(digitalRead(PIN_BUTTON) == 0);

    if(millis() - cntMs > 2000){
      /* Hold 2s */
      packetCnt = 0;
      strHttpCnt = 0;
    }
    else {
      /* Pressed */
      SendPacket(NUM_PACKET_SENT_PERPRSSED);
    }


  }


  if(Serial.available()){
    str = Serial.readString();

    // {"downlinks": [{"f_port": 15,"frm_payload": "vu8=","priority": "NORMAL"}]}
    WF_MQTT_Publish(MQTT_TOPIC_UPLINK, pubMsg, sprintf(pubMsg, "{\"downlinks\": [{\"f_port\": 15,\"frm_payload\": \"%s\",\"priority\": \"NORMAL\"}]}", str.c_str()));
    WF_HttpPost(HTTP_URL, str);
    Serial.print("HTTP Code: "); Serial.println((int)WF_HttpPost(HTTP_URL, str));

    Serial.print("Send: ");     Serial.println(str);
    E220_SendFrame(&config, (uint8_t*)str.c_str(), str.length());
  }

  
  if(LoraSerial.available()){
    int8_t dmy;

    // do{
    //   dmy = LoraSerial.read();
    // } while((dmy != START_CHAR) && (LoraSerial.available()));

    // while(dmy != START_CHAR){
    //   dmy = LoraSerial.read();
    // }

    str = LoraSerial.readStringUntil('\n');

    // Serial.println(str);
    // Serial.println(str.indexOf("END"));

    if(str.indexOf("START") != -1){
      strHttpCnt = 0;
      Serial.print("Start Test: "); Serial.println(packetCnt);
    } 
    else if(str.indexOf("END") != -1){
      flagPktCplt = 1;
      Serial.print("End Test: "); Serial.println(packetCnt++);

    }
    else {
      dmy = str.indexOf('{');

      if(dmy != -1){
        str = str.substring(dmy); 

      }

      if(str.length() > 5){

        strHTTP[strHttpCnt++] = "{\"TimeRev: \"" + WF_NTP_GetDateTime() + ", \"Pkt: \"" + str + "\"}";
        // {"downlinks": [{"f_port": 15,"frm_payload": "vu8=","priority": "NORMAL"}]}
        // WF_MQTT_Publish(MQTT_TOPIC_UPLINK, pubMsg, sprintf(pubMsg, "{\"downlinks\": [{\"f_port\": 15,\"frm_payload\": \"%s\",\"priority\": \"NORMAL\"}]}", BASE64_Encode(str).c_str()));

        // Serial.print(WF_NTP_GetDateTime());

        Serial.print("Received: ");     Serial.println(str);

      }

      
    }


    leds[0] = CRGB ( 0xFF, 0, 0);
    FastLED.show();
  }
  else {
    leds[0] = CRGB (0, 0xFF, 0);
    FastLED.show();
  }

  if(flagPktCplt == 1){
    flagPktCplt = 0;

    Serial.println("Start!");
    Serial.print("Number of Packets: "); Serial.println(strHttpCnt);

    // Serial.print("HTTP Code: "); Serial.println((int)WF_HttpPost(HTTP_URL, strHTTP));     
    for(i = 0; i < strHttpCnt; i++){
       
      // Serial.print("HTTP Code: "); Serial.println((int)WF_HttpPost(HTTP_URL, strHTTP[i]));
      Serial.println(strHTTP[i]);

      // delay(10);
    }

    
    strHttpCnt = 0;
    
    Serial.println("End!");

  }

  if(WF_IsConnected() == 0){
    Serial.println("Connect Wifi!");
    WF_Init((char*)staName, (char*)staPass, (char*)apName, (char*)apPass, 1);
    
    while(WF_IsConnected() == 0){
      Serial.print(".");
      delay(500);
    }
  }

    
  // if(WF_MQTT_IsConnected() == 0){
  //   Serial.println("Connect Server!");
   
  //   WF_MQTT_Connect(MQTT_SERVER, MQTT_PORT, CLIENT_ID, MQTT_USER, MQTT_PASS);
    
  //   while(WF_MQTT_IsConnected() == 0){
  //     Serial.print(".");
  //     delay(500);
  //   }
  //   // WF_MQTT_Subscribe(MQTT_TOPIC_DOWNLINK);
  //   // WF_MQTT_Subscribe("#");
    
  // }
  
  // WF_MQTT_ClientLoop();
 


  // delay(20);
}


void NodeLoop(void){
  if(digitalRead(PIN_BUTTON) == 0){
    uint32_t cntMs;

    cntMs = millis();
    while(digitalRead(PIN_BUTTON) == 0);

    if(millis() - cntMs > 2000){
      /* Hold 2s */
      packetCnt = 0;
      strHttpCnt = 0;

    }
    else {
      /* Pressed */
      SendPacket(NUM_PACKET_SENT_PERPRSSED);
    }


  }


  if(Serial.available()){
    str = Serial.readString();

    Serial.print("Send: ");     Serial.println(str);
    E220_SendFrame(&config, (uint8_t*)str.c_str(), str.length());
  }
  

  
  if(LoraSerial.available()){
    str = LoraSerial.readString();
    Serial.print("Received: ");     Serial.println(str);

    leds[0] = CRGB ( 0xFF, 0, 0);
    FastLED.show();

  }
  else {
    leds[0] = CRGB (0, 0xFF, 0);
    FastLED.show();
  }

  if(WF_IsConnected() == 0){
    Serial.println("Connect Wifi!");
    WF_Init((char*)staName, (char*)staPass, (char*)apName, (char*)apPass, 1);
    
    while(WF_IsConnected() == 0){
      Serial.print(".");
      delay(500);
    }
  }

    
  // if(WF_MQTT_IsConnected() == 0){
  //   Serial.println("Connect Server!");
   
  //   WF_MQTT_Connect(MQTT_SERVER, MQTT_PORT, CLIENT_ID, MQTT_USER, MQTT_PASS);
    
  //   while(WF_MQTT_IsConnected() == 0){
  //     Serial.print(".");
  //     delay(500);
  //   }
  // }

  // WF_MQTT_ClientLoop();
 

  // delay(20);
}