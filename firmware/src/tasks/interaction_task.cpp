#include <Arduino.h>
#include "components/pn532.h"
#include "controllers/pn532_controller.h"
#include "modules/character_module.h"
#include "controllers/portal_controller.h"
#include "tasks/interaction_task.h"

#define BUTTON_PIN 18

static PN532Component nfc(2, 3);
static PN532Controller controller(nfc);

enum NFCMode
{
    READ,
    WRITE
};
static NFCMode nfcMode = READ;

static void interactionTask()
{
    uint8_t uid[7];
    uint8_t uidLength;

    while (true)
    {
        if (nfc.detectCard(uid, &uidLength))
        {
            Serial.print("Found a card! UID Length: ");
            Serial.print(uidLength, DEC);
            Serial.print(" bytes. UID Value: ");
            for (uint8_t i = 0; i < uidLength; i++)
            {
                Serial.print(" 0x");
                Serial.print(uid[i], HEX);
            }
            Serial.println();

            delay(1000); // Delay otherwise the first read is gonna be all types of fucked up

            String cardData = "";
            uint8_t data[4];
            for (uint8_t page = 4; page <= 129; page++)
            {
                if (nfc.readBlock(page, data))
                {
                    for (uint8_t i = 0; i < 4; i++)
                    {
                        char c = (char)data[i];
                        if (c == '\0')
                            break; // This break doesn't reall do nuttin, but it should break the loop if we hit a null character
                        if (c >= 32 && c <= 126)
                        {
                            cardData += c;
                        }
                    }
                }
                else
                {
                    Serial.print("Failed to read page ");
                    Serial.println(page);
                }
            }

            Serial.println("Raw Card Data:");
            Serial.println(cardData);

            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, cardData);
            if (!error)
            {
                Serial.println("Parsed JSON Data:");
                serializeJsonPretty(doc, Serial);
                Serial.println();

                // Extract fields from the JSON
                // TODO: get this from some character module
                String name = doc["name"] | "";
                int level = doc["level"] | 0;
                String color = doc["color"] | "";

                // React to the character using portal logic
                // Bleh I'm starting to write spaghetti code
                portalReactToCharacter(name, level, color);
            }
            else
            {
                Serial.print("Failed to parse JSON: ");
                Serial.println(error.c_str());
            }

            delay(1000);
        }
    }
}

void initializeInteractionTask()
{
    nfc.begin();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    setupPortal();

    interactionTask();
}