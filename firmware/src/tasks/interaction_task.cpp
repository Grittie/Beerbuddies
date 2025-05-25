#include <Arduino.h>
#include "components/pn532.h"
#include "controllers/pn532_controller.h"
#include "modules/character_module.h"
#include "controllers/portal_controller.h"
#include "tasks/interaction_task.h"

#define BUTTON_PIN 18

static PN532Component nfc(18, 19, 23, 5);
static PN532Controller controller(nfc);

enum NFCMode { READ, WRITE };
static NFCMode nfcMode = READ;

static void interactionTask(void *pvParameters) {
    bool lastButtonState = HIGH;
    while (true) {
        int buttonState = digitalRead(BUTTON_PIN);

        // Toggle mode on button press
        if (buttonState == LOW && lastButtonState == HIGH) {
            nfcMode = (nfcMode == READ) ? WRITE : READ;
            Serial.printf("NFC mode changed to: %s\n", nfcMode == READ ? "READ" : "WRITE");
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        uint8_t uid[7];
        uint8_t uidLength;
        if (nfc.detectCard(uid, &uidLength)) {
            if (nfcMode == WRITE) {
                Serial.println("Writing Beer Bear to NFC tag...");
                writeCharacterToNFC(controller, "Beer Bear", 420, "red");
                Serial.println("Write done.");
            } else {
                String name, color;
                int level;
                if (readCharacterFromNFC(controller, name, level, color)) {
                    Serial.printf("Character: %s, Level: %d, Color: %s\n", name.c_str(), level, color.c_str());
                    portalReactToCharacter(name, level, color);
                }
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        lastButtonState = buttonState;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void initializeInteractionTask() {
    nfc.begin();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    setupPortal();

    xTaskCreatePinnedToCore(interactionTask, "InteractionTask", 4096, NULL, 1, NULL, 1);
}