#include <Arduino.h>
#include "components/pn532.h"
#include "controllers/pn532_controller.h"
#include "modules/character_module.h"
#include "controllers/portal_controller.h"
#include "tasks/interaction_task.h"

#define BUTTON_PIN 18

// Updated to use I2C-based constructor
static PN532Component nfc(2, 3); // IRQ and RESET pins
static PN532Controller controller(nfc);

enum NFCMode { READ, WRITE };
static NFCMode nfcMode = READ;

// static void interactionTask() { // Removed FreeRTOS-specific parameters
//     bool lastButtonState = HIGH;
//     while (true) {
//         int buttonState = digitalRead(BUTTON_PIN);

//         // Toggle mode on button press
//         if (buttonState == LOW && lastButtonState == HIGH) {
//             nfcMode = (nfcMode == READ) ? WRITE : READ;
//             Serial.print("NFC mode changed to: ");
//             Serial.println(nfcMode == READ ? "READ" : "WRITE");
//             delay(300); // Replaced vTaskDelay with delay
//         }

//         uint8_t uid[7];
//         uint8_t uidLength;
//         if (nfc.detectCard(uid, &uidLength)) {
//             if (nfcMode == WRITE) {
//                 Serial.println("Writing Beer Bear to NFC tag...");
//                 writeCharacterToNFC(controller, "Beer Bear", 420, "red");
//                 Serial.println("Write done.");
//             } else {
//                 String name, color;
//                 int level;
//                 if (readCharacterFromNFC(controller, name, level, color)) {
//                     Serial.print("Character: ");
//                     Serial.print(name);
//                     Serial.print(", Level: ");
//                     Serial.print(level);
//                     Serial.print(", Color: ");
//                     Serial.println(color);
//                     portalReactToCharacter(name, level, color);
//                 }
//             }
//             delay(1000); // Replaced vTaskDelay with delay
//         }

//         lastButtonState = buttonState;
//         delay(100); // Replaced vTaskDelay with delay
//     }
// }

static void interactionTask() {
    uint8_t uid[7];
    uint8_t uidLength;

    while (true) {
        if (nfc.detectCard(uid, &uidLength)) {
            Serial.print("Found a card! UID Length: ");
            Serial.print(uidLength, DEC);
            Serial.print(" bytes. UID Value: ");
            for (uint8_t i = 0; i < uidLength; i++) {
                Serial.print(" 0x");
                Serial.print(uid[i], HEX);
            }
            Serial.println("");
        } else {
            Serial.println("No card detected.");
        }
        delay(1000);
    }
}

void initializeInteractionTask() {
    nfc.begin();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    setupPortal();

    // Removed xTaskCreatePinnedToCore, as it's not supported on AVR
    interactionTask(); // Directly call the task function
}