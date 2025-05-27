#include "controllers/portal_controller.h"

PortalController::PortalController(uint8_t redPin, uint8_t bluePin)
    : redPin(redPin), bluePin(bluePin) {}

void PortalController::begin() {
    pinMode(redPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    turnOffLEDs();
}

void PortalController::reactToCharacter(const String &name, int level, const String &color) {
    handleLEDs(color);
    handleAudio(name);
}

void PortalController::handleLEDs(const String &color) {
    if (color == "red") {
        Serial.println("Activating red LED.");
        digitalWrite(redPin, HIGH);
        digitalWrite(bluePin, LOW);
    } else if (color == "blue") {
        Serial.println("Activating blue LED.");
        digitalWrite(redPin, LOW);
        digitalWrite(bluePin, HIGH);
    } else {
        Serial.println("No specific color. Turning off LEDs.");
        turnOffLEDs();
    }
}

void PortalController::handleAudio(const String &name) {
    if (name == "BeerBear") {
        Serial.println("Beer Bear detected! Playing sound...");
    } else if (name == "GrittieBear") {
        Serial.println("Grittie Bear detected! Playing different sound...");
    } else {
        Serial.println("Unknown character detected. No special effects.");
    }
}

void PortalController::turnOffLEDs() {
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
}