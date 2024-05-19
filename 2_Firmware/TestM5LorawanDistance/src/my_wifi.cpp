#include "my_wifi.h"


static ESPNow_Data_Pack dataPackESPNow;

WiFiClient wifiClient;
PubSubClient client(wifiClient);


/************************************************************
*                   LOW LEVEL FUNCTION                      *
*************************************************************/
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
    Serial.println("Sent successfully!");
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len){
    Serial.print("L: "); Serial.println(len);
    if(len > MAX_ESPNOW_RXBUFFLEN) return;

    // Serial.printf("EspNow Received: %s\n", incomingData);

    // uint32_t dmy = millis();

    dataPackESPNow.rxLength = len;
    if(dataPackESPNow.pRxBuffer != NULL){
        memcpy(dataPackESPNow.pRxBuffer, incomingData, dataPackESPNow.rxLength);
    }
    // Serial.print("T: "); Serial.println(millis() - dmy);
}



/************************************************************
*                   HIGH LEVEL FUNCTION                     *
*************************************************************/
void WF_Init(char* staName, char* staPass, char* apName, char* apPass, uint8_t isEspNow){
    if((staName != NULL) && (apName != NULL)) {
        WiFi.mode(WIFI_AP_STA);
    }
    else if((staName != NULL) && (apName == NULL)){
        WiFi.mode(WIFI_STA);

    }
    else if((staName == NULL) && (apName != NULL)){
        WiFi.mode(WIFI_AP);
    }


    if(staName != NULL){
        Serial.printf("Connecting to: %s %s\n", staName, staPass);
        WiFi.begin(staName, staPass);
    }

    if(apName != NULL){        
        Serial.printf("Begin AP: %s %s\n", apName, apPass);
        WiFi.softAP(apName, apPass);

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





/************************************************************
*                   MQTT FUNCTION                           *
*************************************************************/
uint8_t flagReceived;
String subMsg;


void callback(char* topic, byte *payload, unsigned int length) {
    flagReceived = 1;
    Serial.println("MQTT Received: ");
    Serial.println(topic);
    Serial.write(payload, length);
    Serial.println();

    subMsg = String( (const char *)payload);
}
 

void WF_MQTT_Connect(const char* serverUrl, uint16_t serverPort, const char* clientID, const char* userName, const char* userPass){
    client.setServer(serverUrl, serverPort);
    client.setCallback(callback);
    client.setBufferSize(2048);
    client.connect(clientID, userName, userPass);

}

void WF_MQTT_Subscribe(const char* topic){
    client.subscribe(topic);
}

void WF_MQTT_Publish(const char* topic, const char* payload, uint16_t length){
    if(topic == NULL || payload == NULL){
        return;
    }
    Serial.print("Publish to Topic: "); Serial.println(topic);
    Serial.print("Payload: "); Serial.println(payload);
    if(length){
        client.publish(topic, payload, length);
    }
    else {
        client.publish(topic, payload);
    }
}

uint8_t WF_MQTT_IsReceived(void){
    if(flagReceived){
        flagReceived = 0;
        return 1;
    }

    return 0;
}

uint8_t WF_MQTT_IsConnected(void){
    if(client.connected()){
        return 1;
    }

    return 0;
}


uint8_t WF_MQTT_GetMsg(String* buff){
    if(flagReceived){
        flagReceived = 0;

        *buff = subMsg;

        return 1;
    }

    return 0;
}

void WF_MQTT_ClientLoop(void){
    client.loop();
}





uint16_t WF_HttpPost(String url, char* jsonString, uint16_t jsonLength){
    uint16_t httpCode;
    String str;
    // String testJson = "{\"node_name\":1,\"do\":12.0,\"ph\":12.3,\"temp\":12.3,\"sal\":12.3}";


    // Create HTTPClient object and pass WiFiClient and URL
    HTTPClient http;   
      
    http.begin(url);  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
      
    httpCode = http.POST((uint8_t*)jsonString, jsonLength);   //Send the actual POST request
      
    if(httpCode>0){
      
        String response = http.getString();                       //Get the response to the request
      
        Serial.println(httpCode);   //Print return code
        Serial.println(response);           //Print request answer
      
    }else{
      
        Serial.print("Error on sending POST: ");
        Serial.println(httpCode);
      
    }
      
    http.end();  //Free resources

    return httpCode;
}

uint16_t WF_HttpPost(String url, String jsonString){
    uint16_t httpCode;
    String str;
    // String testJson = "{\"node_name\":1,\"do\":12.0,\"ph\":12.3,\"temp\":12.3,\"sal\":12.3}";


    // Create HTTPClient object and pass WiFiClient and URL
    HTTPClient http;   
      
    http.begin(url);  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
      
    httpCode = http.POST(jsonString);   //Send the actual POST request
      
    // if(httpCode > 0){
      
    //     String response = http.getString();                       //Get the response to the request
      
    //     Serial.println(httpCode);           //Print return code
    //     Serial.println(response);           //Print request answer
      
    // }else{
      
    //     Serial.print("Error on sending POST: ");
    //     Serial.println(httpCode);
      
    // }
      
    http.end();  //Free resources

    return httpCode;
}


uint16_t WF_HttpPost(String url, String* jsonString){
    uint16_t httpCode;
    String str;
    // String testJson = "{\"node_name\":1,\"do\":12.0,\"ph\":12.3,\"temp\":12.3,\"sal\":12.3}";


    // Create HTTPClient object and pass WiFiClient and URL
    HTTPClient http;   
      
    http.begin(url);  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
    
    for(int i = 0; i < 1000; i++){
        httpCode = http.POST(jsonString[i]);   //Send the actual POST request
        Serial.println((int)httpCode);
    }
      
    // if(httpCode > 0){
      
    //     String response = http.getString();                       //Get the response to the request
      
    //     Serial.println(httpCode);           //Print return code
    //     Serial.println(response);           //Print request answer
      
    // }else{
      
    //     Serial.print("Error on sending POST: ");
    //     Serial.println(httpCode);
      
    // }
      
    http.end();  //Free resources

    return httpCode;
}




WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

void WF_NTP_Init(void){
    timeClient.begin();
}

String WF_NTP_GetDateTime(void){
    timeClient.update();
    // Serial.println(timeClient.getFormattedTime());
    return (timeClient.getFormattedTime());

}