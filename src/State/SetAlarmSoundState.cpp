#include "State/SetAlarmSoundState.h"
#include "State/StateMachine.h"

SetAlarmSoundState::SetAlarmSoundState(Hardware& hardware, StateMachine& stateMachine) : State(hardware, stateMachine) {
}

void SetAlarmSoundState::update() {
    if(hardware_.getInput().getDPad()) {
        lastInteraction_ = millis();
    }

    if((millis() - lastInteraction_) > TIMEOUT) {
        stateMachine_.setCurrentState(&stateMachine_.getClockState());
    }
}

void SetAlarmSoundState::draw() {
    hardware_.getDisplay().printMsgCenter("SetAS");
}

void SetAlarmSoundState::onEnter() {
    lastInteraction_ = millis();
}

void SetAlarmSoundState::onExit() {

}