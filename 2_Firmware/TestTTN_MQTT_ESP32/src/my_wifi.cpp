#include "my_wifi.h"


const uint8_t peerMac[] = {0x94, 0xE6, 0x86, 0x2C, 0xBD, 0x4C};


/* This ATOM MAC Address 4C:75:25:CF:3F:30 */
uint8_t myMac[6];
esp_now_peer_info_t peerInfor;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

/************************************************************
*                   LOW LEVEL FUNCTION                      *
*************************************************************/
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
    Serial.println("Sent successfully!");
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len){
    Serial.printf("EspNow Received: %s\n", data);
}



/************************************************************
*                   HIGH LEVEL FUNCTION                     *
*************************************************************/
void WF_Init(char* staName, char* staPass, char* apName, char* apPass, uint8_t isEspNow){
    WiFi.mode(WIFI_AP_STA);

    ESP_ERROR_CHECK(esp_efuse_mac_get_default(myMac));
    Serial.print("MAC Address: ");
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n", myMac[0], myMac[1], myMac[2], myMac[3], myMac[4], myMac[5]);
    

    if(staName != NULL){
        Serial.printf("Connecting to: %s %s\n", staName, staPass);
        while(WiFi.status() != WL_CONNECTED){
            WiFi.begin(staName, staPass);
            Serial.print(".");
            delay(3000);
        }
    }

    if(apName != NULL){        
        Serial.printf("Begin AP to: %s %s\n", apName, apPass);
        WiFi.softAP(apName, apPass);

    }

    if(isEspNow){
        Serial.println("Begin espNow!");

        if (esp_now_init() != 0) {
            Serial.println("Error initializing ESP-NOW");
            esp_restart();
        }

        /* Set role */

        /* Register peer */
        memcpy(peerInfor.peer_addr, peerMac, 6);
        peerInfor.channel = 0;
        peerInfor.encrypt = false;
        

        esp_now_add_peer(&peerInfor);

        /* Assign callback function */
        esp_now_register_send_cb(OnDataSent);
        esp_now_register_recv_cb(OnDataRecv);

    }


}


void WF_Connect(char* staName, char* staPass){
    uint16_t i;
    while(WiFi.status() != WL_CONNECTED){
	if(i % 60 == 0){
       		WiFi.begin(staName, staPass);
	}
        // Serial.print(".");
        i++;
        delay(1000);
    }
}
void WF_Disconnect(void){
    WiFi.disconnect();
}

uint8_t WF_IsConnected(void){
    return WiFi.status() == WL_CONNECTED;
}


void WF_SendESPNOW(uint8_t* data, uint8_t length){
    esp_now_send(peerMac, data, length);
}







/************************************************************
*                   MQTT FUNCTION                           *
*************************************************************/

uint8_t flagReceived;

void callback(char* topic, byte *payload, unsigned int length) {
    flagReceived = 1;
}
 


void WF_MQTT_Connect(const char* serverUrl, uint16_t serverPort){
    client.setServer(serverUrl, serverPort);
    client.setCallback(callback);

}

void WF_MQTT_Subscribe(const char* topic){
    client.subscribe(topic);
}

void WF_MQTT_Publish(const char* topic, char* payload, uint16_t length){
    client.publish(topic, payload, length);
}

uint8_t WF_MQTT_IsReceived(void){
    if(flagReceived){
        flagReceived = 0;
        return 1;
    }

    return 0;
}

uint8_t WF_MQTT_GetMsg(char* buff){
    
}

void WF_MQTT_ClientLoop(void){
    client.loop();
}