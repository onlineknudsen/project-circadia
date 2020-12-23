#include "State/SetAlarmSoundState.h"
#include "State/StateMachine.h"

SetAlarmSoundState::SetAlarmSoundState(Hardware& hardware, StateMachine& stateMachine) : State(hardware, stateMachine) {
}

void SetAlarmSoundState::update() {

    if((millis() - lastTime_) > TIMEOUT) {
        stateMachine_.setCurrentState(&stateMachine_.getClockState());
    }
}

void SetAlarmSoundState::draw() {
    hardware_.getDisplay().printMsgCenter("SetAS");
}

void SetAlarmSoundState::onEnter() {
    lastTime_ = millis();
}

void SetAlarmSoundState::onExit() {

}