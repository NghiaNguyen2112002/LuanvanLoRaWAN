#ifndef _MY_WIFI_H_
#define _MY_WIFI_H_

#include <Arduino.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "esp_now.h"


#define MQTT_SERVER "eu1.cloud.thethings.network"
#define MQTT_PORT 1883
#define MQTT_USER "test-m5-atom@ttn"
#define MQTT_PASS "NNSXS.YTZGPXPXIUJBH6NHJREVZYQBAA5RR66HJDRRKCY.IFIWP6HGRXOYWRUOWXUVFNIGYRU3JHZSVPSSUGVXFPVFVZUMDFWQ"

#define APP_ID  "test-lorawan-12"


void WF_Init(char* staName, char* staPass, char* apName, char* apPass, uint8_t isEspNow);
void WF_Connect(void);
void WF_Disconnect(void);
uint8_t WF_IsConnected(void);


void WF_SendESPNOW(uint8_t* data, uint8_t length);









#endif // !_MY_WIFI_H_
