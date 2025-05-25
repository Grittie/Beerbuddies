#include "controllers/portal_controller.h"

#define RED_LED_PIN 6
#define BLUE_LED_PIN 7

void setupPortal() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
}

void portalReactToCharacter(const String &name, int level, const String &color) {

    // TODO: split into smaller functions for better readability and testability
    // Make this stuff cleaner but easy for testing
    if (color == "red") {
        Serial.println("Activating red LED for character: " + name);
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(BLUE_LED_PIN, LOW);
    } else if (color == "blue") {
        Serial.println("Activating blue LED for character: " + name);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, HIGH);
    } else {
        Serial.println("No specific color for character: " + name + ". Turning off LEDs.");
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, LOW);
    }

    // audio stuff
    if (name == "BeerBear") {
        Serial.println("Beer Bear detected! Playing sound...");
    } else if (name == "GrittieBear") {
        Serial.println("Grittie Bear detected! Playing different sound...");
    } else {
        Serial.println("Unknown character detected. No special effects.");
    }
}