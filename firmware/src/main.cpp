#include <Arduino.h>
#include "tasks/interaction_task.h"
#include "tasks/sk6812_task.h"
#include <Arduino_FreeRTOS.h>

void interactionTaskWrapper(void *pvParameters) {
    while (true) {
        Serial.println("Running Interaction Task...");
        interactionTask(); // Ensure this function is non-blocking
        vTaskDelay(500 / portTICK_PERIOD_MS); // Adjust interval as needed
    }
}

void sk6812TaskWrapper(void *pvParameters) {
    while (true) {
        Serial.println("Running SK6812 Task...");
        sk6812_glow_white_task(); // Ensure this function is non-blocking
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Adjust interval as needed
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting tasks...");

    sk6812_glow_white_task();
    initializeInteractionTask();
}

void loop() {
    interactionTask();
    delay(500); // Adjust interval as needed
}