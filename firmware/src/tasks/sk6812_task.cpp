#include "components/sk6812.h"
#include "controllers/sk6812_controller.h"

static SK6812Component sk6812(4, 24); // Pin 4, 24 LEDs
static SK6812Controller sk6812Controller(sk6812);

void sk6812_glow_white_task() {
    static bool initialized = false;
    if (!initialized) {
        Serial.println("Initializing SK6812 LED ring...");
        sk6812.begin();
        initialized = true;
    }

    Serial.println("Setting LED ring to white...");
    sk6812Controller.setColor(255, 255, 255); // Set to white color
    delay(1000); // Keep the LEDs white for 1 second
    Serial.println("Turning off LED ring...");
    sk6812Controller.setColor(0, 0, 0); // Turn off the LEDs
    delay(1000); // Wait for 1 second before the next cycle
    // brown leds
    sk6812Controller.setColor(165, 42, 42); // Set to brown color
}