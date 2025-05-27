#ifndef DFPLAYER_CONTROLLER_H
#define DFPLAYER_CONTROLLER_H

#include "components/dfplayer.h"

class DFPlayerController {
public:
    DFPlayerController(DFPlayerComponent &dfplayer);
    void playCharacterAudio(const String &characterId);

private:
    DFPlayerComponent &dfplayer;
};

#endif