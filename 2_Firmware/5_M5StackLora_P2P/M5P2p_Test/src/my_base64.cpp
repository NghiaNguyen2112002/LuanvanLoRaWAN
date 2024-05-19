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


String BASE64_Encode(String str){
    String result;
    uint16_t i, j, index;
    uint8_t buff4[4];
    uint8_t buff3[3];
    
    result = "";

    i = 0;
    index = 0;
    while(index < str.length()){
        buff3[i++] = str[index++];
        
        if(i == 3){
            buff4[0] = (buff3[0] & 0xFC) >> 2;
            buff4[1] = ((buff3[0] & 0x03) << 4) | ((buff3[1] & 0xF0) >> 4);
            buff4[2] = ((buff3[1] & 0x0F) << 2) | ((buff3[2] & 0xC0) >> 6);
            buff4[3] = (buff3[2] & 0x3F);

            for(i = 0; i < 4; i++){ 
                // Serial.print("Buff4: "); Serial.print(buff4[i]); Serial.print(" "); Serial.println((char)base64Char[buff4[i]]);

                result  += String((char)base64Char[buff4[i]]);
            }

            i = 0;
        }
    }

    /* Remaining char */
    if(i){
        for(j = i; j < 3; j++){
            buff3[j] = 0;
        }

        buff4[0] = (buff3[0] & 0xFC) >> 2;
        buff4[1] = ((buff3[0] & 0x03) << 4) | ((buff3[1] & 0xF0) >> 4);
        buff4[2] = ((buff3[1] & 0x0F) << 2) | ((buff3[2] & 0xC0) >> 6);
        buff4[3] = (buff3[2] & 0x3F);

        for(j = 0; j < i+1; j++){   
            // Serial.print("Buff4: "); Serial.print(buff4[j]); Serial.print(" "); Serial.println((char)base64Char[buff4[j]]);

            result  += String((char)base64Char[buff4[j]]);
        }

        while(i++ < 3){
            result += '=';
        }
    }

    // Serial.print("Base 64 Encoded: "); Serial.println(result);
    return result;

}


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