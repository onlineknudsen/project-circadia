#ifndef SET_ALARM_SOUND_STATE_H
#define SET_ALARM_SOUND_STATE_H

#include "State.h"

class SetAlarmSoundState : public State {
    public:
        void update();
        void draw();
        void onEnter();
        void onExit();
        SetAlarmSoundState(Hardware& hardware, Circadia& circadia);

    private:
        unsigned long lastInteraction_;

        char currentSongTitle_[75];
        int currentSongIndex_ = 0;
        byte songCount_;

        void updateSongTitle();
};

#endif