#ifndef CLOCK_STATE_H
#define CLOCK_STATE_H

#include "State.h"

class ClockState : public State {
    public:
        void update();
        void draw();
        void onEnter();
        void onExit();
        ClockState(Hardware& hardware, Circadia& circadia);

    private:
        char dateTimeStr_[9];
        bool showDate = false;
        unsigned long HOLD_TIME = 2000;
};

#endif