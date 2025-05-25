#include "controllers/pn532_controller.h"

PN532Controller::PN532Controller(PN532Component &nfc) : nfc(nfc) {}

bool PN532Controller::readCharacter(JsonDocument &doc) {
    uint8_t uid[7], uidLength;
    if (!nfc.detectCard(uid, &uidLength)) return false;

    uint8_t data[16];
    if (!nfc.readBlock(4, data)) return false;
    DeserializationError err = deserializeJson(doc, data, 16);
    return !err;
}

bool PN532Controller::writeCharacter(const JsonDocument &doc) {
    uint8_t data[16] = {0};
    serializeJson(doc, (char*)data, 16);
    return nfc.writeBlock(4, data);
}