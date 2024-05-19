#include "E220_lora_p2p.h"



template <typename T>
uint8_t IsInRange(T target, T min, T max);



void E220_Init(uint8_t pinTx, uint8_t pinRx, uint32_t baudrate){
    LoraSerial.begin(baudrate, SERIAL_8N1, pinRx, pinTx);

}


int E220_Setting(LoRaConfigItem_t *config){
    int ret = 0;
    uint8_t reg0, reg1, reg2, reg3, cryptH, cryptL;

    uint8_t cmd[20], response[20];
    uint8_t cnt;
    // if (!IsInRange((int)config.own_channel, 0, 30)) {
    //     return 1;
    // }

    cmd[cnt++] = 0xC0;
    cmd[cnt++] = 0x00;
    cmd[cnt++] = 0x08;

    /* Register Address 00H, 01H */
    cmd[cnt++] = config->own_address >> 8;
    cmd[cnt++] = config->own_address & 0xff;


    /* Register Address 02H */
    reg0 = (config->baud_rate << 5);
    reg0 |= (config->air_data_rate);
    cmd[cnt++] = reg0;

    /* Register Address 03H */
    reg1 = (config->subpacket_size << 6);
    reg1 |= (config->rssi_ambient_noise_flag << 5);
    reg1 |= (config->transmitting_power);
    cmd[cnt++] = reg1;

    /* Register Address 04H */
    reg2 = config->own_channel;
    cmd[cnt++] = reg2;

    /* Register Address 05H */
    reg3 = (config->rssi_byte_flag << 7);
    reg3 |= (config->transmission_method_type << 6);
    reg3 |= (config->lbt_flag << 4);
    reg3 |= (config->wor_cycle);
    cmd[cnt++] = reg3;

    /* Register Address 06H, 07H */
    cryptH = config->encryption_key >> 8;
    cryptL = config->encryption_key & 0xff;
    cmd[cnt++] = cryptH;
    cmd[cnt++] = cryptL;

    LoraSerial.write(cmd, cnt);

    delay(100);

    cnt = 0;
    Serial.println("Response: ");
    while (LoraSerial.available()){
        response[cnt++] = LoraSerial.read();
        Serial.println(response[cnt-1], HEX);
    }

    return cnt;
}


int E220_ReceiveFrame(RecvFrame_t *recv_frame);

int E220_SendFrame(LoRaConfigItem_t *config, uint8_t *send_data, int size){
    uint8_t subpacket_size = 0;
    uint8_t frame[3 + size];

    switch (config->subpacket_size) {
        case SUBPACKET_200_BYTE:
            subpacket_size = 200;
            break;
        case SUBPACKET_128_BYTE:
            subpacket_size = 128;
            break;
        case SUBPACKET_64_BYTE:
            subpacket_size = 64;
            break;
        case SUBPACKET_32_BYTE:
            subpacket_size = 32;
            break;
        default:
            subpacket_size = 200;
            break;
    }


    if (size > subpacket_size) {
        Serial.printf("Send data length too long\n");
        return 0;
    }

    frame[0] = config->target_address >> 8;
    frame[1] = config->target_address & 0xff;
    frame[2] = config->target_channel;

    memmove(frame+3, send_data, size);

    LoraSerial.write(frame, size+3);

    delay(100);

    while(LoraSerial.available()){
        Serial.write(LoraSerial.read());
    }

    return 1;
}

void E220_SetDefaultConfigValue(LoRaConfigItem_t *config);






template <typename T>
uint8_t IsInRange(T target, T min, T max) {
    if (target >= min && target <= max) {
        return 1;
    } else {
        return 0;
    }
}
