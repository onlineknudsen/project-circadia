#include "State/ClockState.h"

#include "State/StateMachine.h"


void ClockState::update() {

    if(hardware_.getInput().checkDPadBtnsHold(DPAD_VERTICAL_AXIS, 2000)) {
        stateMachine_.setCurrentState(&stateMachine_.getSetAlarmState());
    } else if (hardware_.getInput().checkDPadBtnsHold(DPAD_HORIZONTAL_AXIS, 2000)) {
        stateMachine_.setCurrentState(&stateMachine_.getSetClockState());
    } else if (hardware_.getInput().checkDPadBtnsHold(DPAD_LEFT, 2000)) {
        stateMachine_.setCurrentState(&stateMachine_.getSetAlarmSoundState());
    }

    hardware_.getClock().getTimeStr(timeStr_, 8);


}

void ClockState::draw() {
    hardware_.getDisplay().printMsgCenter("CLOCK");
}

void ClockState::onEnter() {

}

void ClockState::onExit() {

}

ClockState::ClockState(Hardware& hardware, StateMachine& stateMachine) : State(hardware, stateMachine)  {

}

ClockState::~ClockState() {
    delete timeStr_;
}