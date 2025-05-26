#include "components/pn532.h"

PN532Component::PN532Component(uint8_t irq, uint8_t reset)
    : nfc(irq, reset), cardPreviouslyDetected(false) {}

void PN532Component::begin() {
    nfc.begin();
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN532 board");
        while (1);
    }
    Serial.println("Found chip PN5"); 
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    nfc.SAMConfig();
}

bool PN532Component::detectCard(uint8_t *uid, uint8_t *uidLength) {
    bool cardDetected = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, uidLength);

    if (cardDetected) {
        cardPreviouslyDetected = true;
        return true; // Card is detected
    } else if (cardPreviouslyDetected) {
        cardPreviouslyDetected = false;
        Serial.println("Card removed."); // Log card removal
        return false; // Card was removed
    }

    return false; // No card detected
}

bool PN532Component::readBlock(uint8_t pageNumber, uint8_t *data) {
    return nfc.ntag2xx_ReadPage(pageNumber, data);
}

bool PN532Component::writeBlock(uint8_t pageNumber, const uint8_t *data) {
    return nfc.ntag2xx_WritePage(pageNumber, const_cast<uint8_t *>(data));
}
