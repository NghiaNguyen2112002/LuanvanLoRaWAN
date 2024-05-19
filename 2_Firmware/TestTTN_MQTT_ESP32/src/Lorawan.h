#ifndef _LORAWAN_H_
#define _LORAWAN_H_

#include <Arduino.h>

#if 1
// #define CLASS_C
// #define CLASS_B
// #define CLASS_A
#define CLASS_C_OTTA

/* Class Test */
// #define CLASS_TEST        "2" //class C

// #define CLASS_TEST        "0" //cLass A

#ifdef CLASS_C
  #define CLASS_TEST        "2" //class C
  #define LRW_DEVEUI        "70B3D57ED0062F64"
  #define LRW_APPEUI        "0000000000000001"
  #define LRW_APPKEY        "9BE5A8DE9D989EF8F5365A24232BFE75"
  #define LRW_DEVADDR       "260B9F09"
  #define LRW_APP_S_KEY     "48D02FEEE218F59CE87C1AB84DF846F9"
  #define LRW_NWK_S_KEY     "D66B386C67730562496642FA5CDCA409"
  #define LRW_MANUFACID     "ASR"
  #define LRW_MODELVERSION  "6501"
  #define LRW_VERSIONID     "v4.3"
#endif  //CLASS_C

#ifdef CLASS_B
  #define CLASS_TEST        "1" //class B
  #define LRW_DEVEUI        "70B3D57ED00623F0"
  #define LRW_APPEUI        "0000000000000001"
  #define LRW_APPKEY        "9BE5A8DE9D989EF8F5365A24232BFE75"
  #define LRW_DEVADDR       "260B2FC0"
  #define LRW_APP_S_KEY     "CBB55AAF27A060D13EC8163219BE2F8F"
  #define LRW_NWK_S_KEY     "5A0348ED3E85E2650A3D8201459FB880"
  #define LRW_MANUFACID     "ASR"
  #define LRW_MODELVERSION  "6501"
  #define LRW_VERSIONID     "v4.3"
#endif // CLASS_B

#ifdef CLASS_A
  #define CLASS_TEST        "0" //class A
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
  #define LRW_DEVEUI        "70B3D57ED006308D"  
  #define LRW_APPEUI        "0000000000000003"
  #define LRW_APPKEY        "EF41386956DDB02B5307375B464159AB"
  #define LRW_DEVADDR       "260B59EA"
  #define LRW_APP_S_KEY     "3EAEC4479047F1BAD3A29FF1794BF920"
  #define LRW_NWK_S_KEY     "D4303425CF98904DB55357B8A06F881D"
  #define LRW_MANUFACID     "ASR"
  #define LRW_MODELVERSION  "6501"
  #define LRW_VERSIONID     "v4.3"
#endif  //CLASS_C_OTTA

#endif

void SendCMD(String cmd);
String WaitMsg(uint16_t timeOut);


void LRW_Init_ABP(uint8_t RX, uint8_t TX, uint32_t baudrate);
void LRW_Init_OTAA(uint8_t RX, uint8_t TX, uint32_t baudrate);

uint8_t LRW_IsDeviceConnected(void);
uint8_t LRW_IsJoined(void);

void LRW_ConfigOTTA(String deviceEUI, String appEUI, String appKey, String uldlMode);
void LRW_ConfigABP(String deviceAddr, String appSKey, String netSKey, String uldlMode);

void LRW_SetClass(String className);
void LRW_SetRxWindow(String freq);
void LRW_SetFreqMask(String mask);

void LRW_StartJoin(void);
void LRW_StopJoin(void);

String LRW_SendMsg(uint8_t confirm, uint8_t nbTrials, String msg);
String LRW_ReceiveMsg(void);


/* String to Hex String */
String LRW_EncodeMsg(String msg);

/* Hex String to String */
String LRW_DecodeMsg(String msg);







#endif