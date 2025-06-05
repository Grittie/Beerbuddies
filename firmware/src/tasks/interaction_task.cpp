#include <Arduino.h>
#include "components/pn532.h"
#include "controllers/pn532_controller.h"
#include "modules/character_module.h"
#include "controllers/portal_controller.h"
#include "tasks/interaction_task.h"
#include "components/dfplayer.h"
#include "controllers/dfplayer_controller.h"
#include "components/sk6812.h"
#include "controllers/sk6812_controller.h"

static SK6812Component sk6812(4, 24); // Pin 4, 24 LEDs
static SK6812Controller sk6812Controller(sk6812);

#define BUTTON_PIN 18
#define NFC_UID_MAX_LENGTH 7
#define NFC_BLOCK_SIZE 4
#define NFC_START_PAGE 4
#define NFC_END_PAGE 129
#define JSON_DOC_SIZE 64
#define CARD_COOLDOWN_TIME 5000

static PN532Component nfc(2, 3);
static PN532Controller controller(nfc);
static PortalController portalController(6, 8);
static DFPlayerComponent dfplayer(10, 11);
static DFPlayerController dfplayerController(dfplayer);

enum NFCMode
{
    READ,
    WRITE
};

extern char *__brkval;
extern char *__heap_start;

int freeMemory() {
    char top;
    return &top - (__brkval == 0 ? __heap_start : __brkval);
}

static NFCMode nfcMode = READ;

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

String readCardData()
{
    String cardData = "";
    uint8_t blockData[NFC_BLOCK_SIZE];
    bool endFound = false;

    for (uint8_t page = NFC_START_PAGE; page <= NFC_END_PAGE && !endFound; page++)
    {
        if (nfc.readBlock(page, blockData))
        {
            for (uint8_t i = 0; i < NFC_BLOCK_SIZE; i++)
            {
                char c = (char)blockData[i];

                if (c == '\0') {
                    endFound = true;
                    break;
                }

                if (c >= 32 && c <= 126)
                {
                    cardData += c;

                    if (c == '}')
                    {
                        endFound = true;
                        break;
                    }
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


void processCardData(const String &cardData)
{
    Serial.println("Raw Card Data:");
    Serial.println(cardData);

    Serial.print("Free memory before manual parsing: ");
    Serial.println(freeMemory());

    String name = "";
    int id = 0;
    String color = "";

    int nameStart = cardData.indexOf("\"name\":\"") + 8;
    int nameEnd = cardData.indexOf("\"", nameStart);
    if (nameStart > 0 && nameEnd > nameStart)
        name = cardData.substring(nameStart, nameEnd);

    int idStart = cardData.indexOf("\"id\":\"") + 6;
    int idEnd = cardData.indexOf("\"", idStart);
    if (idStart > 0 && idEnd > idStart)
        id = cardData.substring(idStart, idEnd).toInt();

    int colorStart = cardData.indexOf("\"color\":\"") + 9;
    int colorEnd = cardData.indexOf("\"", colorStart);
    if (colorStart > 0 && colorEnd > colorStart)
        color = cardData.substring(colorStart, colorEnd);

    Serial.println("Parsed Data:");
    Serial.print("Name: ");
    Serial.println(name);
    Serial.print("ID: ");
    Serial.println(id);
    Serial.print("Color: ");
    Serial.println(color);

    portalController.reactToCharacter(name, id, color);
    dfplayerController.playCharacterAudio(String(id));
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
            // Rapid circles while reading the card
            sk6812Controller.cycleWhite(); // Rapid cycling effect
            delay(500);

            if (uidLength == lastUidLength && memcmp(uid, lastUid, uidLength) == 0)
            {
                unsigned long currentTime = millis();
                if (currentTime - lastTagTime >= CARD_COOLDOWN_TIME)
                {
                    Serial.println("Same tag detected but took more than 5 seconds, processing again.");
                    lastTagTime = currentTime;
                    printCardUID(uid, uidLength);

                    memcpy(lastUid, uid, uidLength);
                    lastUidLength = uidLength;

                    delay(1000);

                    String cardData = readCardData();
                    processCardData(cardData);

                    // Parse color and spin rapidly 5 times
                    String color = ""; // Extracted from cardData
                    int colorStart = cardData.indexOf("\"color\":\"") + 9;
                    int colorEnd = cardData.indexOf("\"", colorStart);
                    if (colorStart > 0 && colorEnd > colorStart)
                        color = cardData.substring(colorStart, colorEnd);

                    Serial.print("Color: ");
                    Serial.println(color);

                    uint8_t red = 0, green = 0, blue = 0;
                    if (color == "red") { red = 255; }
                    else if (color == "green") { green = 255; }
                    else if (color == "blue") { blue = 255; }
                    // Add more colors as needed

                    sk6812Controller.setColor(red, green, blue);
                    for (int i = 0; i < 5; i++) {
                        sk6812Controller.cycleWhite(); // Spin rapidly
                    }
                    sk6812Controller.turnOff(); // Turn off LEDs

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

            delay(1000);

            String cardData = readCardData();
            processCardData(cardData);

            // Parse color and spin rapidly 5 times
            String color = ""; // Extracted from cardData
            int colorStart = cardData.indexOf("\"color\":\"") + 9;
            int colorEnd = cardData.indexOf("\"", colorStart);
            if (colorStart > 0 && colorEnd > colorStart)
                color = cardData.substring(colorStart, colorEnd);

            Serial.print("Color: ");
            Serial.println(color);

            uint8_t red = 0, green = 0, blue = 0;
            if (color == "red") { red = 255; }
            else if (color == "green") { green = 255; }
            else if (color == "blue") { blue = 255; }
            // Add more colors as needed

            sk6812Controller.setColor(red, green, blue);
            for (int i = 0; i < 5; i++) {
                sk6812Controller.cycleWhite(); // Spin rapidly
            }
            sk6812Controller.turnOff(); // Turn off LEDs

            delay(1000);
        }
        else
        {
            // Breathing effect when no card is detected
            for (int brightness = 0; brightness <= 255; brightness += 5) {
                sk6812Controller.setColor(brightness, brightness, brightness); // Gradually increase brightness
                delay(30);
            }
            for (int brightness = 255; brightness >= 0; brightness -= 5) {
                sk6812Controller.setColor(brightness, brightness, brightness); // Gradually decrease brightness
                delay(30);
            }
        }
        delay(500);
    }
}

void initializeInteractionTask()
{
    nfc.begin();
    portalController.begin();
    dfplayer.begin();
    dfplayer.testFileCount(1);
    interactionTask();
}