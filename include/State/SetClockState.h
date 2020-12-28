#ifndef SET_CLOCK_STATE_H
#define SET_CLOCK_STATE_H

#include "State.h"
#include "TimeComponent.h"

class SetClockState : public State {
    public:
        void update();
        void draw();
        void onEnter();
        void onExit();
        SetClockState(Hardware& hardware, Circadia& circadia);

    private:
        unsigned long lastInteraction_;
        byte timeComponents[6];

        TimeComponent selectedComponent_;

        void increaseSelectedComponent();
        void decreaseSelectedComponent();

        void generateTimeStr(char* timeStr, byte size, byte& startingUnderlineChar, byte& endingUnderlineChar);
        void generateDateStr(char* dateStr, byte size, byte& startingUnderlineChar, byte& endingUnderlineChar);

};

#endif