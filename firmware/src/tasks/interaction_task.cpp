#include <Arduino.h>
#include "components/pn532.h"
#include "controllers/pn532_controller.h"
#include "modules/character_module.h"
#include "controllers/portal_controller.h"
#include "tasks/interaction_task.h"

#define BUTTON_PIN 18

static PN532Component nfc(2, 3);
static PN532Controller controller(nfc);

enum NFCMode { READ, WRITE };
static NFCMode nfcMode = READ;

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
            Serial.println();

            String cardData = "";
            uint8_t data[4];
            for (uint8_t page = 4; page <= 129; page++) {
                if (nfc.readBlock(page, data)) {
                    for (uint8_t i = 0; i < 4; i++) {
                        char c = (char)data[i];
                        if (c == '\0') break; // Optional: stop early if null terminator
                        if (c >= 32 && c <= 126) {
                            cardData += c;
                        }
                    }
                } else {
                    Serial.print("Failed to read page ");
                    Serial.println(page);
                }
            }

            Serial.println("Raw Card Data:");
            Serial.println(cardData);

            StaticJsonDocument<512> doc; // Consider increasing for longer JSON
            DeserializationError error = deserializeJson(doc, cardData);
            if (!error) {
                Serial.println("Parsed JSON Data:");
                serializeJsonPretty(doc, Serial);
                Serial.println();
            } else {
                Serial.print("Failed to parse JSON: ");
                Serial.println(error.c_str());
            }
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

    interactionTask();
}