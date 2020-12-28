#ifndef SET_ALARM_STATE_H
#define SET_ALARM_STATE_H

#include "State.h"

class SetAlarmState : public State {
    public:
        void update();
        void draw();
        void onEnter();
        void onExit();
        SetAlarmState(Hardware& hardware, Circadia& circadia);

    private:
        unsigned long lastInteraction_;
};

#endif