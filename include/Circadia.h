#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "Hardware/Hardware.h"
#include "State/ClockState.h"
#include "State/SetAlarmState.h"
#include "State/SetClockState.h"
#include "State/SetAlarmSoundState.h"
#include "State/AlarmTriggeredState.h"
#include "State/AlarmStatus.h"

#define TIMEOUT 5000

class Circadia {
    public:
        Circadia(Hardware& hardware);
        void setCurrentState(State* newState);
        State& getCurrentState();
        void update();

        Hardware& getHardware();

        State& getClockState();
        State& getSetClockState();
        State& getSetAlarmState();
        State& getSetAlarmSoundState();
        State& getAlarmTriggeredState();
    private:
        State* currentState_;

        Hardware& hardware_;
        // Clock state
        ClockState clockState_;
        SetAlarmState setAlarmState_;
        SetClockState setClockState_;
        SetAlarmSoundState setAlarmSoundState_;
        AlarmTriggeredState alarmTriggeredState_;

        AlarmStatus alarmStatus_ = AlarmStatus::PrimaryReady;

        void listen();
        void debugListen();

        friend void AlarmTriggeredState::onExit();

};

#endif