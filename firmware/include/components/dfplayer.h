#ifndef DFPLAYER_H
#define DFPLAYER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

class DFPlayerComponent {
public:
    DFPlayerComponent(uint8_t rxPin, uint8_t txPin);
    void begin();
    bool playRandomFile(const String &characterId);
    void testFileCount(int folder);

private:
    SoftwareSerial serial;
    DFRobotDFPlayerMini player;
};

#endif