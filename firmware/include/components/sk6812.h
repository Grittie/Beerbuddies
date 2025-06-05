#ifndef SK6812_H
#define SK6812_H

#include <Adafruit_NeoPixel.h>

class SK6812Component {
public:
    SK6812Component(uint8_t pin, uint16_t numLeds);
    void begin();
    void setAllPixels(uint8_t red, uint8_t green, uint8_t blue);
    uint16_t getNumLeds() const; // Add this getter method
    void setPixelColor(uint16_t n, uint8_t red, uint8_t green, uint8_t blue);
void show();

private:
    uint8_t pin;
    uint16_t numLeds;
    Adafruit_NeoPixel strip;
};

#endif // SK6812_H