#include "controllers/sk6812_controller.h"

SK6812Controller::SK6812Controller(SK6812Component &component) : ledRing(component) {}

void SK6812Controller::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    ledRing.setAllPixels(red, green, blue);
}

void SK6812Controller::turnOff() {
    ledRing.setAllPixels(0, 0, 0); // Turn off all LEDs
}

void SK6812Controller::setStaticWhite() {
    ledRing.setAllPixels(255, 255, 255); // Set all LEDs to white
}

void SK6812Controller::cycleWhite() {
    for (int i = 0; i < ledRing.getNumLeds(); i++) {
        ledRing.setAllPixels(0, 0, 0); // Turn off all LEDs
        ledRing.setPixelColor(i, 255, 255, 255); // Turn on one LED to white
        ledRing.show();
        delay(100); // Adjust delay for speed of cycling
    }
}