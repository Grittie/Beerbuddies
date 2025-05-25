#pragma once
#include "components/pn532.h"
#include <ArduinoJson.h>

class PN532Controller {
public:
    PN532Controller(PN532Component &nfc);
    bool readCharacter(JsonDocument &doc);
    bool writeCharacter(const JsonDocument &doc);
private:
    PN532Component &nfc;
};