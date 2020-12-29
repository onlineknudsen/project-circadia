#ifndef SET_ALARM_STATE_H
#define SET_ALARM_STATE_H

#include "State.h"
#include "TimeComponent.h"

class SetAlarmState : public State {
    public:
        void update();
        void draw();
        void onEnter();
        void onExit();
        SetAlarmState(Hardware& hardware, Circadia& circadia);

    private:
        unsigned long lastInteraction_;
        TimeComponent selectedComponent_;
        byte alarmComponents[3];

        void increaseSelectedComponent();
        void decreaseSelectedComponent();

        void generateAlarmStr(char* timeStr, byte size, byte& startingUnderlineChar, byte& endingUnderlineChar);
};

#endif