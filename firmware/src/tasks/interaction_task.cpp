#include <Arduino.h>
#include "components/pn532.h"
#include "controllers/pn532_controller.h"
#include "modules/character_module.h"
#include "controllers/portal_controller.h"
#include "tasks/interaction_task.h"

#define BUTTON_PIN 18
#define NFC_UID_MAX_LENGTH 7
#define NFC_BLOCK_SIZE 4
#define NFC_START_PAGE 4
#define NFC_END_PAGE 129
#define JSON_DOC_SIZE 512

static PN532Component nfc(2, 3);
static PN532Controller controller(nfc);

enum NFCMode
{
    READ,
    WRITE
};

static NFCMode nfcMode = READ;

// Helper function to print the UID
void printCardUID(const uint8_t *uid, uint8_t uidLength)
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
}

// Helper function to read card data
String readCardData()
{
    String cardData = "";
    uint8_t blockData[NFC_BLOCK_SIZE];

    for (uint8_t page = NFC_START_PAGE; page <= NFC_END_PAGE; page++)
    {
        if (nfc.readBlock(page, blockData))
        {
            for (uint8_t i = 0; i < NFC_BLOCK_SIZE; i++)
            {
                char c = (char)blockData[i];
                if (c == '\0')
                    break;
                if (c >= 32 && c <= 126) // Printable ASCII range
                {
                    cardData += c;
                }
            }
        }
        else
        {
            Serial.print("Failed to read page ");
            Serial.println(page);
            break;
        }
    }

    return cardData;
}

// Helper function to parse and react to card data
void processCardData(const String &cardData)
{
    Serial.println("Raw Card Data:");
    Serial.println(cardData);

    StaticJsonDocument<JSON_DOC_SIZE> doc;
    DeserializationError error = deserializeJson(doc, cardData);

    if (!error)
    {
        Serial.println("Parsed JSON Data:");
        serializeJsonPretty(doc, Serial);
        Serial.println();

        // Extract fields from the JSON
        String name = doc["name"] | "";
        int level = doc["level"] | 0;
        String color = doc["color"] | "";

        // React to the character using portal logic
        portalReactToCharacter(name, level, color);
    }
    else
    {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
    }
}

static void interactionTask()
{
    uint8_t uid[NFC_UID_MAX_LENGTH];
    uint8_t uidLength;

    static uint8_t lastUid[NFC_UID_MAX_LENGTH];
    static uint8_t lastUidLength = 0;
    static unsigned long lastTagTime = 0;

    while (true)
    {
        if (nfc.detectCard(uid, &uidLength))
        {
            if (uidLength == lastUidLength && memcmp(uid, lastUid, uidLength) == 0)
            {
                unsigned long currentTime = millis();
                if (currentTime - lastTagTime >= 5000)
                {
                    Serial.println("Same tag detected but took more than 5 seconds, processing again.");
                    lastTagTime = currentTime;
                    printCardUID(uid, uidLength);

                    memcpy(lastUid, uid, uidLength);
                    lastUidLength = uidLength;

                    String cardData = readCardData();
                    processCardData(cardData);

                    delay(1000);
                }
                else
                {
                    Serial.println("Same tag detected, skipping processing.");
                    lastTagTime = currentTime;
                }
                delay(500);
                continue;
            }

            Serial.println("New tag detected!");
            printCardUID(uid, uidLength);

            memcpy(lastUid, uid, uidLength);
            lastUidLength = uidLength;

            lastTagTime = millis();

            delay(500);

            String cardData = readCardData();
            processCardData(cardData);

            delay(1000);
        }
        delay(500);
    }
}

void initializeInteractionTask()
{
    nfc.begin();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    setupPortal();

    interactionTask();
}