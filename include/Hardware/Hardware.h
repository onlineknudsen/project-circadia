#ifndef HARDWARE_H
#define HARDWARE_H

#include "Display.h"
#include "SDAudioSource.h"
#include "Remote.h"
#include "Clock.h"
#include "Input.h"

class Hardware {
    public:
        Hardware(int displayCS, int sdCS, int dUpPin, int dDownPin, int dLeftPin, int dRightPin);
        Display& getDisplay();
        SDAudioSource& getAudioSource();
        Remote& getRemote();
        Clock& getClock();
        Input& getInput();
    private:
        Display display_;
        SDAudioSource audioSource_;
        Remote remote_;
        Clock alarmClock_;
        Input input_;
};

#endif