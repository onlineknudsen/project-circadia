#include "Circadia.h"


Circadia::Circadia(Hardware& hardware) : hardware_(hardware), clockState_(hardware, *this), setAlarmState_(hardware, *this), setClockState_(hardware, *this), setAlarmSoundState_(hardware, *this) {
    setCurrentState(&clockState_);
}

void Circadia::update() {
    hardware_.getInput().update();

    currentState_->update();
    currentState_->draw();

    hardware_.getDisplay().update();

}

void Circadia::setCurrentState(State* newState) {
    if(currentState_ != nullptr)
        currentState_->onExit();
    currentState_ = newState;
    currentState_->onEnter();
}

State& Circadia::getClockState() {
    return clockState_;
}

State& Circadia::getSetAlarmState() {
    return setAlarmState_;
}

State& Circadia::getSetClockState() {
    return setClockState_;
}

State& Circadia::getSetAlarmSoundState() {
    return setAlarmSoundState_;
}

Hardware& Circadia::getHardware() {
    return hardware_;
}