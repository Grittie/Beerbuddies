#include "controllers/pn532_controller.h"
#include <ArduinoJson.h>

void writeCharacterToNFC(PN532Controller &controller, const char *name, int level, const char *color) {
    StaticJsonDocument<128> doc;
    doc["name"] = name;
    doc["id"] = level;
    doc["color"] = color;
    controller.writeCharacter(doc);
}

bool readCharacterFromNFC(PN532Controller &controller, String &name, int &level, String &color) {
    StaticJsonDocument<128> doc;
    if (!controller.readCharacter(doc)) return false;
    name = doc["name"] | "";
    level = doc["id"] | 0;
    color = doc["color"] | "";
    return true;
}