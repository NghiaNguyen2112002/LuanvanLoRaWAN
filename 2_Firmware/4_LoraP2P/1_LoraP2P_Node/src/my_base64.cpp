#include "my_base64.h"


static const uint8_t base64Char[] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/"
};


uint8_t IsBase64(char c){
    if( (c >= 'A') && (c <= 'Z') ){
        return 1;
    }
    
    if( (c >= 'a') && (c <= 'z') ){
        return 1;
    }

    if( (c >= '0') && (c <= '9') ){
        return 1;
    }

    if( (c == '+') || (c == '/') ){
        return 1;
    }


    return 0;
}

int8_t FindIndex64(char c){
    uint8_t i;

    for(i = 0; i < 64; i++){
        if(base64Char[i] == c){
            return i;
        }
    }

    return -1;
}


void BASE64_Encode(void);

String BASE64_Decode(String str){
    String result;
    uint16_t i, j, size, index;
    uint8_t buff4[4];
    uint8_t buff3[3];

    size = str.length();
    i = 0;
    j = 0;
    index = 0;
    result = "";

    while( (size--) ){
    // while( (size--) && (str[index] != '=') && IsBase64(str[index]) ){

        if((str[index] == '=')){
            buff4[i++] = 0;
            index++;
            
        }
        else { 
            
            buff4[i++] = str[index++];
            
            Serial.print(buff4[i-1], HEX);
            /* Convert base64 char to uint8 */
            buff4[i-1] = FindIndex64(buff4[i-1]);
        }

        
        // buff4[i++] = str[index++];
        if(i == 4){

            buff3[0] = (buff4[0] << 2) | ( (buff4[1] >> 4) & 0x03);
            buff3[1] = (buff4[1] << 4) | ( (buff4[2] >> 2) & 0x0F);
            buff3[2] = (buff4[2] << 6) | ( (buff4[3]) & 0x3F);
            
            // Serial.println(buff3[0], HEX);
            // Serial.println(buff3[1], HEX);
            // Serial.println(buff3[2], HEX);

            result += (char)buff3[0];
            result += (char)buff3[1];
            result += (char)buff3[2];

            i = 0;
        }
    }

    // Serial.print("Result: ");

    // for(i = 0; i < result.length(); i++){
    //   Serial.print("0x"); Serial.print(result[i], HEX); Serial.print(" ");
    // } 


    return result;
}