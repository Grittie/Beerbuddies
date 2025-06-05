#ifndef SK6812_CONTROLLER_H
#define SK6812_CONTROLLER_H

#include "components/sk6812.h"

class SK6812Controller {
public:
    SK6812Controller(SK6812Component &component);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void turnOff();
    void setStaticWhite();
    void cycleWhite();

private:
    SK6812Component &ledRing;
};

#endif // SK6812_CONTROLLER_H