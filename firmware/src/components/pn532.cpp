#include "components/pn532.h"

PN532Component::PN532Component(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t ss)
    : nfc(sck, miso, mosi, ss) {}

void PN532Component::begin() {
    nfc.begin();
    nfc.SAMConfig();
}

bool PN532Component::detectCard(uint8_t *uid, uint8_t *uidLength) {
    return nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, uidLength);
}

bool PN532Component::readBlock(uint8_t blockNumber, uint8_t *data) {
    return nfc.mifareclassic_ReadDataBlock(blockNumber, data);
}

bool PN532Component::writeBlock(uint8_t blockNumber, const uint8_t *data) {
    return nfc.mifareclassic_WriteDataBlock(blockNumber, (uint8_t *)data);
}