#include "State/SetAlarmState.h"
#include "State/StateMachine.h"

SetAlarmState::SetAlarmState(Hardware& hardware, StateMachine& stateMachine) : State(hardware, stateMachine) {
    lastTime_ = millis();
}

void SetAlarmState::update() {


    if((millis() - lastTime_) > TIMEOUT) {
        stateMachine_.setCurrentState(&stateMachine_.getClockState());
    }
}

void SetAlarmState::draw() {
    hardware_.getDisplay().printMsgCenter("SetA");
}

void SetAlarmState::onEnter() {
    lastTime_ = millis();
}

void SetAlarmState::onExit() {

}