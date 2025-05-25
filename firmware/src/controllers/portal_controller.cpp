#include "controllers/portal_controller.h"

#define RED_LED_PIN 21
#define BLUE_LED_PIN 22

void setupPortal() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
}

void portalReactToCharacter(const String &name, int level, const String &color) {
    // LED logic
    if (color == "red") {
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(BLUE_LED_PIN, LOW);
    } else if (color == "blue") {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, HIGH);
    } else {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, LOW);
    }

    // Sound or other effects
    if (name == "Beer Bear") {
        Serial.println("Beer Bear detected! Playing sound...");
    } else if (name == "Blue Bear") {
        Serial.println("Blue Bear detected! Playing different sound...");
    } else {
        Serial.println("Unknown character detected. No special effects.");
    }
}