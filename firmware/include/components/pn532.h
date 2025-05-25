#pragma once
#include <Adafruit_PN532.h>

class PN532Component {
public:
    PN532Component(uint8_t irq, uint8_t reset);    void begin();
    bool detectCard(uint8_t *uid, uint8_t *uidLength);
    bool readBlock(uint8_t blockNumber, uint8_t *data);
    bool writeBlock(uint8_t blockNumber, const uint8_t *data);
private:
    Adafruit_PN532 nfc;
};