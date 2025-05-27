#pragma once
#include <Arduino.h>

class PortalController {
public:
    PortalController(uint8_t redPin, uint8_t bluePin);
    void begin();
    void reactToCharacter(const String &name, int level, const String &color);
    void turnOffLEDs();
    
private:
    uint8_t redPin;
    uint8_t bluePin;

    void handleLEDs(const String &color);
    void handleAudio(const String &name);
};