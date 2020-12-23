#include "State/SetClockState.h"
#include "State/StateMachine.h"

SetClockState::SetClockState(Hardware& hardware, StateMachine& stateMachine) : State(hardware, stateMachine) {
}

void SetClockState::update() {


    if((millis() - lastTime_) > TIMEOUT) {
        stateMachine_.setCurrentState(&stateMachine_.getClockState());
    }
}

void SetClockState::draw() {
    hardware_.getDisplay().printMsgCenter("SetC");
}

void SetClockState::onEnter() {
    lastTime_ = millis();
}

void SetClockState::onExit() {

}