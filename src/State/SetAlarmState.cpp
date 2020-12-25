#include "State/SetAlarmState.h"
#include "State/StateMachine.h"

SetAlarmState::SetAlarmState(Hardware& hardware, StateMachine& stateMachine) : State(hardware, stateMachine) {

}

void SetAlarmState::update() {

    if(hardware_.getInput().getDPad()) {
        lastInteraction_ = millis();
    }

    if((millis() - lastInteraction_) > TIMEOUT) {
        stateMachine_.setCurrentState(&stateMachine_.getClockState());
    }
}

void SetAlarmState::draw() {
    hardware_.getDisplay().printMsgCenter("SetA");
}

void SetAlarmState::onEnter() {
    lastInteraction_ = millis();
}

void SetAlarmState::onExit() {

}