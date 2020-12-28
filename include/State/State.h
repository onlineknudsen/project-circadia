#ifndef STATE_H
#define STATE_H

#include "Hardware/Hardware.h"

class Circadia;

class State {
    public:
        State(Hardware& hardware, Circadia& circadia);
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void onEnter() = 0;
        virtual void onExit() = 0;

    protected:
        Hardware& hardware_;
        Circadia& circadia_;
};

#endif