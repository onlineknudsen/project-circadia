#include "State/StateMachine.h"


StateMachine::StateMachine(Hardware& hardware) : hardware_(hardware), clockState_(hardware, *this), setAlarmState_(hardware, *this), setClockState_(hardware, *this), setAlarmSoundState_(hardware, *this) {
    setCurrentState(&clockState_);
}

void StateMachine::update() {
    hardware_.getInput().update();

    currentState_->update();
    currentState_->draw();

    hardware_.getDisplay().update();

}

void StateMachine::setCurrentState(State* newState) {
    currentState_->onExit();
    currentState_ = newState;
    currentState_->onEnter();
}

State& StateMachine::getClockState() {
    return clockState_;
}

State& StateMachine::getSetAlarmState() {
    return setAlarmState_;
}

State& StateMachine::getSetClockState() {
    return setClockState_;
}

State& StateMachine::getSetAlarmSoundState() {
    return setAlarmSoundState_;
}

Hardware& StateMachine::getHardware() {
    return hardware_;
}