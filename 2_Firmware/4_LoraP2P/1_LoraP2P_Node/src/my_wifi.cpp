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
*                   SMART CONFIG FUNCTION                   *
*************************************************************/
#ifdef SMART_CONFIG

static WebServer webserver(80);
static String wfName, wfPass;
static String minDO, maxDO;
static String minTemp, maxTemp;
static String minpH, maxpH;
static String minEC, maxEC;
static uint8_t flagWeb;


const char webPage[] =
{
    "<style>"
    "body {"
        "background: #f6f5f7;"
        "display: flex;"
        "justify-content: center;"
        "align-items: center;"
        "flex-direction: column;"
        "font-family: 'Montserrat', sans-serif;"
        "height: 100%;"
        "width: 100%;"
        "margin: 0 0 0;"
    "}"

    "h1 {"
        "font-weight: bold;"
        "margin: 0;"
    "}"

    "h2 {"
        "text-align: center;"
    "}"

    "button {"
        "border-radius: 20px;"
        "border: 1px solid #007ef5;"
        "background-color: #007ef5;"
        "color: #FFFFFF;"
        "font-size: 12px;"
        "font-weight: bold;"
        "padding: 12px 45px;"
        "letter-spacing: 1px;"
        "text-transform: uppercase;"
    "}"

    "button:active {"
        "transform: scale(0.95);"
    "}"

    "button:focus {"
        "outline: none;"
    "}"

    "button.ghost {"
        "background-color: transparent;"
        "border-color: #FFFFFF;"
    "}"

    "form {"
        "background-color: #FFFFFF;"
        "display: flex;"
        "align-items: center;"
        "justify-content: center;"
        "flex-direction: column;"
        "padding: 0 50px;"
        "height: 100%;"
        "text-align: center;"
    "}"

    "input {"
        "background-color: #eee;"
        "border: none;"
        "padding: 15px 20px;"
        "margin: 5px 0;"
        "width: 100%;"
    "}"

    ".container {"

        "top: 0;"
        "bottom: 0;"
        "background-color: #fff;"
        "border-radius: 10px;"
        "box-shadow: 0 14px 28px rgba(0, 0, 0, 0.25),"
            "0 10px 10px rgba(0, 0, 0, 0.22);"
        "position: relative;"
        "width: 40%;"
        "height: 100%;"
    "}"

    ".form-container {"
        "position: relative;"
        "top: 0;"
        "height: 100%;"
    "}"

    ".sign-in-container {"
        "left: 0;"
        "width: 100%;"
        "height: 100%;"
        "z-index: 2;"
    "}"


    ".bottom-left-content {"
            "position: fixed;"
            "bottom: 0px;"
            "right: 0px;"
    "}"

    "</style>"
    "</head>"
    "<body>"
            "<div class='container'>"
            "<div class='form-container sign-in-container'>"
                
                "<form method='get' action='setap' style='transform: scale(0.95); transform-origin: center;'>"
                    "<h3 style='text-align:left;color:#000000'>Enter Wifi Information</h3>"
                    "<input type='text' placeholder='Enter Name' name='wifiName' id='wifiName' required>"
                    "<input type='text' placeholder='Enter Password' name='wifiPass' id='wifiPass' required>"
                    "<h3 style='text-align:left;color:#000000'>Enter Temperature Warning Level</h3>"
                    "<input type='text' placeholder='Enter Min' name='minTemp' id='minTemp' required>"
                    "<input type='text' placeholder='Enter Max' name='maxTemp' id='maxTemp' required>"
                    "<h3 style='text-align:left;color:#000000'>Enter Dissolved Oxygen Warning Level</h3>"
                    "<input type='text' placeholder='Enter Min DO' name='minDO' id='minDO' required>"
                    "<input type='text' placeholder='Enter Max DO' name='maxDO' id='maxDO' required>"
                    "<h3 style='text-align:left;color:#000000'>Enter pH Warning Level</h3>"
                    "<input type='text' placeholder='Enter Min pH' name='minpH' id='minpH' required>"
                    "<input type='text' placeholder='Enter Max pH' name='maxpH' id='maxpH' required>"
                    "<h3 style='text-align:left;color:#000000'>Enter Electrical Conductivity Warning Level</h3>"
                    "<input type='text' placeholder='Enter Min EC' name='minEC' id='minEC' required>"
                    "<input type='text' placeholder='Enter Max EC' name='maxEC' id='maxEC' required>"
                    "<button type='submit;font-size:40px;width:100%;margin-top:20px;margin-bottom:20px'>Save</button>"
                "</form>"
            "</div>"     
        "</div>"
        "<div class='bottom-left-content'>"
        "<p>Made by Nguyen Trung Nghia</p>"
            "<p>Contact 0906631096</p>"
        "</div>"
    "</body>"
};

String MakePage(String title, String contents) {
  String s;

  s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head>";
  s += contents;
  s += "</html>";

  return s;
}


void WF_WebStart(void){
    flagWeb = 0;
    webserver.on("/", WF_WebHomePage);

    webserver.on("/setap", [](){

        wfName = webserver.arg("wifiName");
        wfPass = webserver.arg("wifiPass");
        maxDO = webserver.arg("maxDO");
        minDO = webserver.arg("minDO");
        maxpH = webserver.arg("maxpH");
        minpH = webserver.arg("minpH");
        maxTemp = webserver.arg("maxTemp");
        minTemp = webserver.arg("minTemp");
        maxEC = webserver.arg("maxEC");
        minEC = webserver.arg("minEC");


        // Serial.println("Config Done");

        // Serial.print("wifiName: "); Serial.println(wfName);
        // Serial.print("wifiPass: "); Serial.println(wfPass);
        // Serial.print("minDO: "); Serial.println(minDO);
        // Serial.print("maxDO: "); Serial.println(maxDO);
        // Serial.print("minEC: "); Serial.println(minEC);
        // Serial.print("maxEC: "); Serial.println(maxEC);
        // Serial.print("minpH: "); Serial.println(minpH);
        // Serial.print("maxpH: "); Serial.println(maxpH);
        // Serial.print("minTemp: "); Serial.println(minTemp);
        // Serial.print("maxTemp: "); Serial.println(maxTemp);


        String s = "<body><h1>Wifi configuration's successful !!</h1></body>";
        webserver.send(200, "text/html", MakePage("", s));
        
        flagWeb = 1;

    });

    webserver.begin();
}

void WF_WebHomePage(void){
    webserver.send(200, "text/html", webPage);
}

void WF_WebHandleClient(void){
    webserver.handleClient();
}

uint8_t WF_WebIsDone(void){
    if(flagWeb){
        flagWeb = 0;

        return 1;
    }

    return 0;
}

// uint8_t WF_WebReceive(uint8_t* buff, uint8_t size){
    
// }

String WF_WebGetWFName(void){
    return wfName;
}
String WF_WebGetWFPass(void){
    return wfPass;
}

float WF_WebGetMinDO(void){
    return atof(minDO.c_str());
}
float WF_WebGetMaxDO(void){
    return atof(maxDO.c_str());
}

float WF_WebGetMinPH(void){
    return atof(minpH.c_str());
}

float WF_WebGetMaxPH(void){
    return atof(maxpH.c_str());

}

float WF_WebGetMinEC(void){
    return atof(minEC.c_str());

}

float WF_WebGetMaxEC(void){
    return atof(maxEC.c_str());
}

float WF_WebGetMinTemp(void){
    return atof(minTemp.c_str());
}

float WF_WebGetMaxTemp(void){
    return atof(maxTemp.c_str());
}




#endif // SMART_CONFIG



/************************************************************
*                   HTTP FUNCTION                           *
*************************************************************/
#if 1 
uint16_t WF_HttpPost(const char* url, char* jsonString){
    uint16_t httpCode;
    String str;
    // String testJson = "{\"node_name\":1,\"do\":12.0,\"ph\":12.3,\"temp\":12.3,\"sal\":12.3}";

    // Create WiFiClient object
    WiFiClient client;

    // Create HTTPClient object and pass WiFiClient and URL
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, url);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    while(*jsonString){
        str += *jsonString++;
    }
    Serial.print(str);
    httpCode = http.POST(str);

    // Serial.print("Code": ");
    Serial.println((int)httpCode);
    // Check the result of the request
    // if (httpCode > 0) {
    //     String payload = http.getString();
    //     Serial.println("HTTP Response: " + payload);
    // } else {
    //     Serial.println("HTTP POST request failed");
    // }

    // Close the connection
    http.end();

    return httpCode;
}

#endif  // END HTTP FUNCTION


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
