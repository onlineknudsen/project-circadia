#ifndef ALARM_TRIGGERED_STATE_H
#define ALARM_TRIGGERED_STATE_H

#include "State.h"

class AlarmTriggeredState : public State {
    public:
        void update();
        void draw();
        void onEnter();
        void onExit();
        void setPrimary(bool isPrimary);
        AlarmTriggeredState(Hardware& hardware, Circadia& circadia);

    private:
        char dateTimeStr_[8];
        unsigned long lastInvert_;
        unsigned long timeout_ = 0;
        bool invertOn_;
        const unsigned long FLASH_INTERVAL = 500;
        bool isPrimaryAlarm_;
        bool remotePressed_;
};

#endif