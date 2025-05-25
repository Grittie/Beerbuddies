#include "components/pn532.h"

PN532Component::PN532Component(uint8_t irq, uint8_t reset)
    : nfc(irq, reset) {}

void PN532Component::begin() {
    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN532 board");
        while (1); // Halt
    }
    Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
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