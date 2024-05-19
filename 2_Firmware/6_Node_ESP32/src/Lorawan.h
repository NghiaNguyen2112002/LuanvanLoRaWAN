#ifndef _LORAWAN_H_
#define _LORAWAN_H_

#include <Arduino.h>

#include "pin.h"


// #define CLASS_B
// #define CLASS_A
// #define CLASS_C_ABP
#define CLASS_C_OTTA

// #define LORAWAN_ABP
#define LORAWAN_OTAA

#ifdef CLASS_A
  #define CLASS_TEST        "2" //class A
  #define LRW_DEVEUI        "70B3D57ED00623F2"
  #define LRW_APPEUI        "0000000000000001"
  #define LRW_APPKEY        "9BE5A8DE9D989EF8F5365A24232BFE75"
  #define LRW_DEVADDR       "260BC5AC"
  #define LRW_APP_S_KEY     "316605D56612C11ABCEFB06A93F9BBFC"
  #define LRW_NWK_S_KEY     "BEC280302560429A1B0E8F4B1068F48A"
  #define LRW_MANUFACID     "ASR"
  #define LRW_MODELVERSION  "6501"
  #define LRW_VERSIONID     "v4.3"
#endif // CLASS_A
 
#ifdef CLASS_C_OTTA
  #define CLASS_TEST        "2" //class C
  #define LRW_DEVEUI        "70B3D57ED0066749"
  #define LRW_APPEUI        "0000000000000010"
  #define LRW_APPKEY        "5C07CC59A804372CEC9758BF68234AD8"
  #define LRW_DEVADDR       "260BBF22"
  #define LRW_APP_S_KEY     "34512CE5D404A156134E32BABFD65137"
  #define LRW_NWK_S_KEY     "BAB48F264B12B63C8F2F8662565B14F5"
  #define LRW_MANUFACID     "ASR"
  #define LRW_MODELVERSION  "6501"
  #define LRW_VERSIONID     "v4.3"
#endif  //CLASS_C_OTTA

#ifdef CLASS_C_ABP
  #define CLASS_TEST        "2" //class C
  #define LRW_DEVEUI        "70B3D57ED0066E68"
  #define LRW_APPEUI        "0000000000000001"
  #define LRW_APPKEY        "5031A973CD4FBC565261C68F556729F6"
  #define LRW_DEVADDR       "260BC92F"
  #define LRW_APP_S_KEY     "52E39F9DFDB08B155920E98553059162"
  #define LRW_NWK_S_KEY     "B35CCE3B3EC9B0A04366DD0249B7E053"
  #define LRW_MANUFACID     "ASR"
  #define LRW_MODELVERSION  "6501"
  #define LRW_VERSIONID     "v4.3"
#endif  //CLASS_C_ABP


void SendCMD(String cmd);
String WaitMsg(uint16_t timeOut);


void LRW_Init(uint8_t RX, uint8_t TX, uint32_t baudrate);

uint8_t LRW_IsDeviceConnected(void);
uint8_t LRW_IsJoined(void);

void LRW_ConfigOTTA(String deviceEUI, String appEUI, String appKey, String uldlMode);
void LRW_ConfigABP(String deviceAddr, String appSKey, String netSKey, String uldlMode);

void LRW_SetClass(String className);
void LRW_SetRxWindow(String freq);
void LRW_SetFreqMask(String mask);
void LRW_SetDataRate(uint8_t indexRange);

void LRW_StartJoin(void);
void LRW_StopJoin(void);

String LRW_SendMsg(uint8_t confirm, uint8_t nbTrials, String msg);
String LRW_ReceiveMsg(void);


/* String to Hex String */
String LRW_EncodeMsg(String msg);

/* Hex String to String */
String LRW_DecodeMsg(String msg);







#endif