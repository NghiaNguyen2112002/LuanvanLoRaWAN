#include "sensor_read.h"

#define SerialSensor            Serial2


static const uint8_t cmd_temp[] = {0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x60, 0x0A};
static const uint8_t cmd_humi[] = {0x01, 0x04, 0x00, 0x02, 0x00, 0x01, 0x90, 0x0A};

static const uint8_t cmd_temp_humi[] = {0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x20, 0x0B};

static const uint8_t cmd_baudrate[] = {0x01, 0x04, 0x01, 0x01, 0x00, 0x01, 0xD4, 0x36};

static float testTemp = 28.9, testHumi = 53.4;



void SR_Init(uint8_t RX, uint8_t TX, uint32_t baudrate){
    
    SerialSensor.begin(baudrate, SERIAL_8N1, RX, TX);
    delay(1000);

}


float SR_GetTemp(void){
    float result;
    uint16_t timeOut;
    uint8_t receivedData[7];

    // Serial.print("Getting temperature: ");

    SerialSensor.flush();
    SerialSensor.write(cmd_temp, 8);

    timeOut = 2000 / 50;
    while( (timeOut--) && (!SerialSensor.available()) ){
        delay(50);
    }

    if(SerialSensor.available()){
        SerialSensor.readBytes(receivedData, sizeof(receivedData));
        // for(int i = 0; i < sizeof(receivedData); i++){
        //     Serial.print(receivedData[i], HEX); Serial.print(" ");
        // }
        result = ((receivedData[3] << 8) | receivedData[4]) / 100.0;
    }

#ifdef MODE_TEST
    result  = testTemp;
#endif


    // Serial.println(result);
    return result;
}


float SR_GetHumi(void){
    float result;
    uint16_t timeOut;
    uint8_t receivedData[7];

    // Serial.print("Getting humidity: ");

    SerialSensor.flush();
    SerialSensor.write(cmd_humi, sizeof(cmd_humi));

    timeOut = 2000 / 50;
    while( (timeOut--) && (!SerialSensor.available()) ){
        delay(50);
    }

    if(SerialSensor.available()){
        
        SerialSensor.readBytes(receivedData, sizeof(receivedData));

        // for(int i = 0; i < sizeof(receivedData); i++){
        //     Serial.print(receivedData[i], HEX); Serial.print(" ");
        // }

        result = ((receivedData[3] << 8) | receivedData[4]) / 100.0;
    }

#ifdef MODE_TEST
    result  = testHumi;
#endif


    // Serial.println(result);
    return result;
}




uint8_t SR_GetHumiTemp(float* temp, float* humi){
    uint16_t timeOut;
    uint8_t receivedData[9];
    float dmy;
    Serial.println("Getting temperature and humidity");

    while(SerialSensor.available()){
        SerialSensor.read();
    }
    SerialSensor.write(cmd_temp_humi, sizeof(cmd_temp_humi));

    timeOut = 2000 / 50;
    while( (timeOut--) && (!SerialSensor.available()) ){
        delay(50);
    }

    if(SerialSensor.available()){
        
        SerialSensor.readBytes(receivedData, sizeof(receivedData));

        for(int i = 0; i < sizeof(receivedData); i++){
            Serial.print(receivedData[i], HEX); Serial.print(" ");
        }

        dmy = ((receivedData[5] << 8) | receivedData[6]) / 100.0;
        Serial.println(dmy);
        
        if( (dmy > 30) & (dmy < 100) ){
            *temp = ((receivedData[3] << 8) | receivedData[4]) / 100.0;
            *humi = ((receivedData[5] << 8) | receivedData[6]) / 100.0;

            return 1;
        }

        // Serial.print("Temp: "); Serial.println(*temp);
        // Serial.print("Humi: "); Serial.println(*humi);

    }

    return 0;
}

