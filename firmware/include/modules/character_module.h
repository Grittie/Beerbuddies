#pragma once
#include "controllers/pn532_controller.h"

void writeCharacterToNFC(PN532Controller &controller, const char *name, int level, const char *color);
bool readCharacterFromNFC(PN532Controller &controller, String &name, int &level, String &color);