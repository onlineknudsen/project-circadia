#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "Hardware/Hardware.h"
#include "ClockState.h"
#include "SetAlarmState.h"
#include "SetClockState.h"
#include "SetAlarmSoundState.h"

class State;

#define TIMEOUT 5000

class StateMachine {
    public:
        StateMachine(Hardware& hardware);
        void setCurrentState(State* newState);
        State& getCurrentState();
        void update();

        Hardware& getHardware();

        State& getClockState();
        State& getSetClockState();
        State& getSetAlarmState();
        State& getSetAlarmSoundState();
    private:
        State* currentState_;

        Hardware& hardware_;
        // Clock state
        ClockState clockState_;
        SetAlarmState setAlarmState_;
        SetClockState setClockState_;
        SetAlarmSoundState setAlarmSoundState_;


};

#endif