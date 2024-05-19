#ifndef _MY_WIFI_H_
#define _MY_WIFI_H_



#include <Arduino.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#define MQTT_SERVER "eu1.cloud.thethings.network"
#define MQTT_PORT 1883
#define MQTT_USER "test-atom-eu1@ttn"
#define MQTT_PASS "NNSXS.Z6E4BKNL64CJ6KG5UWQ7SMOLR53Z4HZETJRD5HY.4NONAZSN45EBE4OPSAH7YNKZZAHAG62XSDP62N3DG2FR2LHVFGAQ"
#define CLIENT_ID  "node-e220"
 


#define MQTT_TOPIC_UPLINK       "v3/test-atom-eu1@ttn/devices/e220-p2p/down/push"
#define MQTT_TOPIC_DOWNLINK     "v3/test-atom-eu1@ttn/devices/e220-p2p/down/push"


#define HTTP_URL        "https://script.google.com/macros/s/AKfycby-vYf-4EsObUAblyAfEmhhQUALWTJmUThKzSSA1rNwG8C7o6Gv1fjNyEjGarA6lJBw/exec?action=package"


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

void WF_MQTT_Connect(const char* serverUrl, uint16_t serverPort, const char* clientID, const char* userName, const char* userPass);
void WF_MQTT_Subscribe(const char* topic);
void WF_MQTT_Publish(const char* topic, const char* payload, uint16_t length);
uint8_t WF_MQTT_IsConnected(void);
uint8_t WF_MQTT_IsReceived(void);
uint8_t WF_MQTT_GetMsg(String* buff);
void WF_MQTT_ClientLoop(void);



uint16_t WF_HttpPost(String url, char* jsonString, uint16_t jsonLength);
uint16_t WF_HttpPost(String url, String jsonString);
uint16_t WF_HttpPost(String url, String* jsonString);

void WF_NTP_Init(void);
String WF_NTP_GetDateTime(void);


#endif // !_MY_WIFI_H_
