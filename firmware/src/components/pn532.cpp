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

// Use NTAG2xx-compatible methods
bool PN532Component::readBlock(uint8_t pageNumber, uint8_t *data) {
    return nfc.ntag2xx_ReadPage(pageNumber, data);
}

bool PN532Component::writeBlock(uint8_t pageNumber, const uint8_t *data) {
    return nfc.ntag2xx_WritePage(pageNumber, const_cast<uint8_t *>(data));
}
