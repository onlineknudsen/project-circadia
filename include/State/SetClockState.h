#ifndef SET_CLOCK_STATE_H
#define SET_CLOCK_STATE_H

#include "State.h"

class SetClockState : public State {
    public:
        void update();
        void draw();
        void onEnter();
        void onExit();
        SetClockState(Hardware& hardware, StateMachine& stateMachine);

    private:
        unsigned long lastTime_;
};

#endif