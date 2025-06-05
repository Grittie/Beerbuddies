#include "components/sk6812.h"
#include <Adafruit_NeoPixel.h>

SK6812Component::SK6812Component(uint8_t pin, uint16_t numLeds)
    : pin(pin), numLeds(numLeds), strip(numLeds, pin, NEO_GRB + NEO_KHZ800) {}

void SK6812Component::begin() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void SK6812Component::setAllPixels(uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < numLeds; i++) {
        strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
}

uint16_t SK6812Component::getNumLeds() const {
    return numLeds; // Return the number of LEDs
}

void SK6812Component::setPixelColor(uint16_t n, uint8_t red, uint8_t green, uint8_t blue) {
    strip.setPixelColor(n, strip.Color(red, green, blue));
}

void SK6812Component::show() {
    strip.show();
}