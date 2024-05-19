#include "sensor_read.h"

static const uint8_t cmd_temp_humi[] = {0x14, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC6, 0xCE};
// static const uint8_t cmd_temp_humi[] = {1, 3, 0, 0, 0, 2, 196, 11};


static const uint8_t cmd_temp[] = {0x03, 0x03, 0x00, 0x00, 0x00, 0x01, 0x85, 0xE8};
static const uint8_t cmd_humi[] = {0x03, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD4, 0x28};
static const uint8_t cmd_co[] = {0x0E, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0x35};
static const uint8_t cmd_co2[] = {0x02, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC6, 0xF8};
static const uint8_t cmd_so2[] = {0x0D, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0x06};
static const uint8_t cmd_no2[] = {0x0C, 0x03, 0x00, 0x02, 0x00, 0x01, 0x24, 0xD7};
static const uint8_t cmd_pm2_5[] = {0x04, 0x03, 0x00, 0x0C, 0x00, 0x01, 0x44, 0x5C};
static const uint8_t cmd_o3[] = {0x0B, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0x60};
static const uint8_t cmd_pm10[] = {0x04, 0x03, 0x00, 0x0D, 0x00, 0x01, 0x15, 0x9C};

static const uint8_t cmd_soil_PH[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
static const uint8_t cmd_soil_temp_humi[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E};
static const uint8_t cmd_soil_npk[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x03, 0x65, 0xCD};
static const uint8_t cmd_soil_conductivity[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};


static float testTemp = 28.9, testHumi = 53.4;
static uint16_t testCO2 = 410, testPm25 = 22, testPM10 = 22, testCO = 1, testO3 = 0, testNO2 = 1, testSO2 = 0;




void SR_Init(uint8_t RX, uint8_t TX, uint32_t baudrate){

    Serial2.begin(baudrate, SERIAL_8N1, RX, TX);
    delay(1000);




}

float SR_GetTemp(void){
    float result;
    uint16_t timeOut;

    Serial.print("Getting temperature: ");

    Serial2.write(cmd_temp, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));
  
        result = ((int)receivedData[3] * 256 + (int)receivedData[4]) / 10.0;
    }

#ifdef MODE_TEST
    result  = testTemp;
#endif


    Serial.println(result);
    return result;
}


float SR_GetHumi(void){
    float result;
    uint16_t timeOut;

    Serial.print("Getting humidity: ");

    Serial2.write(cmd_humi, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

       if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        result = ((int)receivedData[3] * 256 + (int)receivedData[4]) / 10.0;
    }

#ifdef MODE_TEST
    result  = testHumi;
#endif


    Serial.println(result);
    return result;
}


uint16_t SR_GetCO(void){
    int result;
    uint16_t timeOut;

    Serial.print("Getting CO: ");

    Serial2.write(cmd_co, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        result = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];


    }

#ifdef MODE_TEST
    result  = testCO;
#endif
    Serial.println(result);

    return result;
}


uint16_t SR_GetCO2(void){
    int result;
    uint16_t timeOut;

    Serial.print("Getting CO2: ");

    Serial2.write(cmd_co2, 8);

    timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        result = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];


    }

#ifdef MODE_TEST
    result  = testCO2;
#endif
    Serial.println(result);

    return result;
}

uint16_t SR_GetSO2(void){
    int result;
    uint16_t timeOut;

    Serial.print("Getting SO2: ");

    Serial2.write(cmd_so2, 8);

    timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        result = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];


    }

#ifdef MODE_TEST
    result  = testSO2;
#endif
    Serial.println(result);

    return result;
}
uint16_t SR_GetNO2(void){
    int result;
    uint16_t timeOut;

    Serial.print("Getting NO2: ");

    Serial2.write(cmd_no2, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        result = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];


    }

#ifdef MODE_TEST
    result  = testNO2;
#endif
    Serial.println(result);

    return result;
}


uint16_t SR_GetPM2_5(void){
    int result; 
    uint16_t timeOut;

    Serial.print("Getting PM2.5: ");

    Serial2.write(cmd_pm2_5, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        result = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];


    }

#ifdef MODE_TEST
    result  = testPm25;
#endif
    Serial.println(result);

    return result;
}


uint16_t SR_GetO3(void){
    int result;
    uint16_t timeOut;

    Serial.print("Getting O3: ");

    Serial2.write(cmd_o3, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        result = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];


    }

#ifdef MODE_TEST
    result  = testO3;
#endif
    Serial.println(result);

    return result;
}

uint16_t SR_GetPM10(void){
    int result;
    uint16_t timeOut;

    Serial.print("Getting PM10: ");

    Serial2.write(cmd_pm10, 8);

    timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        result = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];


    }

#ifdef MODE_TEST
    result  = testPM10;
#endif
    Serial.println(result);

    return result;
}


uint8_t SR_GetTempHumi(float* temp, float* humi){
    float result;
    uint16_t timeOut;

    Serial.print("Getting Air Temp, Humi: ");
    if(Serial2.available()){
        Serial2.readString();   
    }

    Serial2.write(cmd_temp_humi, 8);

    timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[9];

        Serial2.readBytes(receivedData, sizeof(receivedData));

        *humi = ((int)receivedData[3]*256 + (int)receivedData[4])/10.0;
        *temp = ((int)receivedData[5]*256 + (int)receivedData[6])/10.0;

        
        // result = ((int)receivedData[3]*256 + (int)receivedData[4])/10.0;
        // Serial.printf("Result: %f", result);

        return 1;
    }

    return 0;
}


uint8_t SR_GetSoil_PH(float* pH){
 int result;
    uint16_t timeOut;

    Serial.print("Getting Soil pH: ");

    Serial2.write(cmd_soil_PH, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        *pH = ((uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4]) * 0.01;

        return 1;
    }

    return 0;

}

uint8_t SR_GetSoil_TempHumi(float* sTemp, float* sHumi){
    float result;
    uint16_t timeOut;

    Serial.print("Getting Soil Temp, Humi: ");
    if(Serial2.available()){
        Serial2.readString();   
    }

    Serial2.write(cmd_soil_temp_humi, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[9];

        Serial2.readBytes(receivedData, sizeof(receivedData));

        *sHumi = ((int)receivedData[3]*256 + (int)receivedData[4])/10.0;
        *sTemp = ((int)receivedData[5]*256 + (int)receivedData[6])/10.0;

        // result = ((int)receivedData[3]*256 + (int)receivedData[4])/10.0;
        // Serial.printf("Result: %f", result);
        
        return 1;
    }

    return 0;
}


uint8_t SR_GetSoil_NPK(uint16_t* nito, uint16_t* phoptpho, uint16_t* kali){
    int result;
    uint16_t timeOut;

    Serial.print("Getting Soil NPK: ");

    Serial2.write(cmd_soil_npk, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[11];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        *nito = ((uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4]);
        *phoptpho = (uint16_t)(receivedData[5] << 8) | (uint16_t)receivedData[6];
        *kali = (uint16_t)(receivedData[7] << 8) | (uint16_t)receivedData[8];

        return 1;
    }

    return 0;
}

uint8_t SR_GetSoil_Conductivity(uint16_t* conductivity){
    int result;
    uint16_t timeOut;

    Serial.print("Getting Soil Conductivity: ");

    Serial2.write(cmd_soil_conductivity, 8);

   timeOut = 2000 / 50;
    while( (timeOut--) && (!Serial2.available()) ){
        delay(50);
    }

    if(Serial2.available()){
        uint8_t receivedData[7];
        
        Serial2.readBytes(receivedData, sizeof(receivedData));

        *conductivity = (uint16_t)(receivedData[3] << 8) | (uint16_t)receivedData[4];

        return 1;
    }

    return 0;
}