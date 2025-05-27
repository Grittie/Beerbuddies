#include <Arduino.h>
#include "tasks/interaction_task.h"

// this is not my finest work :)

void setup() {
    Serial.begin(115200);
    
    Serial.println("Starting NFC Interaction Task...");
    initializeInteractionTask();
}

void loop() {
}