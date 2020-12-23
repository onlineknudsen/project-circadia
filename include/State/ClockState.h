#ifndef CLOCK_STATE_H
#define CLOCK_STATE_H

#include "State.h"

class ClockState : public State {
    public:
        void update();
        void draw();
        void onEnter();
        void onExit();
        ClockState(Hardware& hardware, StateMachine& stateMachine);
        ~ClockState();

    private:
        char* timeStr_ = new char[8];
        unsigned long HOLD_TIME = 2000;
};

#endif