#ifndef _MY_WIFI_H_
#define _MY_WIFI_H_


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>




#define SMART_CONFIG
#define MAX_ESPNOW_RXBUFFLEN            300



typedef struct ESPNow_Data_Pack{
    const uint8_t* peerMac;
    uint8_t rxLength;
    uint8_t pRxBuffer[MAX_ESPNOW_RXBUFFLEN];
} ESPNow_Data_Pack;


void WF_Init(char* staName, char* staPass, char* apName, char* apPass, uint8_t isEspNow);
void WF_Connect(char* staName, char* staPass);
void WF_Disconnect(void);
uint8_t WF_IsConnected(void);

void WF_ESPNOWAddPeer(const uint8_t* peerMac);
void WF_ESPNOWSetBuffer(uint8_t* pBuffer);
void WF_ESPNOWSend(uint8_t* data, uint8_t length);
uint8_t WF_ESPNOWRead(uint8_t *pBuff);

void WF_WebStart(void);
void WF_WebHomePage(void);
void WF_WebHandleClient(void);
uint8_t WF_WebIsDone(void);
uint8_t WF_WebReceive(uint8_t* buff, uint8_t size);

String WF_WebGetWFName(void);
String WF_WebGetWFPass(void);

float WF_WebGetMinDO(void);
float WF_WebGetMaxDO(void);

float WF_WebGetMinPH(void);
float WF_WebGetMaxPH(void);

float WF_WebGetMinEC(void);
float WF_WebGetMaxEC(void);

float WF_WebGetMinTemp(void);
float WF_WebGetMaxTemp(void);


uint16_t WF_HttpPost(const char* url, char* jsonString);

void WF_MQTT_Connect(const char* serverUrl, uint16_t serverPort, const char* clientID, const char* userName, const char* userPass);
void WF_MQTT_Subscribe(const char* topic);
void WF_MQTT_Publish(const char* topic, const char* payload, uint16_t length);
uint8_t WF_MQTT_IsConnected(void);
uint8_t WF_MQTT_IsReceived(void);
uint8_t WF_MQTT_GetMsg(String* buff);
void WF_MQTT_ClientLoop(void);


#endif // !_MY_WIFI_H_
