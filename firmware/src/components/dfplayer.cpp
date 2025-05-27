#include "components/dfplayer.h"

DFPlayerComponent::DFPlayerComponent(uint8_t rxPin, uint8_t txPin)
    : serial(rxPin, txPin) {}

void DFPlayerComponent::begin() {
    serial.begin(9600);
    if (!player.begin(serial)) {
        Serial.println(F("DFPlayer Mini initialization failed!"));
        Serial.println(F("1. Check the DFPlayer Mini connections"));
        Serial.println(F("2. Insert an SD card"));
        while (true); // Halt execution
    }
    Serial.println(F("DFPlayer Mini initialized successfully."));
    player.setTimeOut(500);  // Set timeout to 500ms
    player.volume(25);       // Set volume (0-30)
    player.EQ(DFPLAYER_EQ_NORMAL); // Set equalization to normal
}

void DFPlayerComponent::testFileCount(int folder) {
    int fileCount = player.readFileCountsInFolder(folder);
    Serial.print("Test Folder: ");
    Serial.println(folder);
    Serial.print("File count: ");
    Serial.println(fileCount);
}

bool DFPlayerComponent::playRandomFile(const String &characterId) {
    int folder = characterId.toInt();
    if (folder <= 0) {
        Serial.println("Invalid character ID.");
        return false;
    }

    int fileCount = player.readFileCountsInFolder(folder);
    if (fileCount <= 0) {
        Serial.println("No files found in folder.");
        return false;
    }



    int randomFile = random(1, fileCount + 1);
    Serial.print("Playing file ");
    Serial.print(randomFile);
    Serial.print(" in folder ");
    Serial.println(folder);

    player.playFolder(folder, randomFile);
    Serial.println("File playback started.");

    return true;
}