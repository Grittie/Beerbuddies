#include "controllers/dfplayer_controller.h"

DFPlayerController::DFPlayerController(DFPlayerComponent &dfplayer)
    : dfplayer(dfplayer) {}

void DFPlayerController::playCharacterAudio(const String &characterId) {
    if (!dfplayer.playRandomFile(characterId)) {
        Serial.println("Failed to play audio for character.");
    }
}