#ifndef STATE_H
#define STATE_H

#include "Hardware/Hardware.h"

class StateMachine;

class State {
    public:
        State(Hardware& hardware, StateMachine& stateMachine);
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void onEnter() = 0;
        virtual void onExit() = 0;

    protected:
        unsigned long lastTime_;
        Hardware& hardware_;
        StateMachine& stateMachine_;
};

#endif