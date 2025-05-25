#include <Arduino.h>
#include "tasks/interaction_task.h"

void setup() {
    Serial.begin(115200);
    initializeInteractionTask();
}

void loop() {
}