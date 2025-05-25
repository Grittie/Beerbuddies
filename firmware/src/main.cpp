#include <Arduino.h>
#include "tasks/interaction_task.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Starting NFC Interaction Task...");
    initializeInteractionTask();
}

void loop() {
}