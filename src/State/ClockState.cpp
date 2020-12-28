#include "State/ClockState.h"

#include "State/StateMachine.h"


void ClockState::update() {

    if(hardware_.getInput().checkDPadBtnsHold(DPAD_VERTICAL_AXIS, HOLD_TIME)) {
        stateMachine_.setCurrentState(&stateMachine_.getSetAlarmState());
    } else if (hardware_.getInput().checkDPadBtnsHold(DPAD_HORIZONTAL_AXIS, HOLD_TIME)) {
        stateMachine_.setCurrentState(&stateMachine_.getSetClockState());
    } else if (hardware_.getInput().checkDPadBtnsHold(DPAD_LEFT, HOLD_TIME)) {
        stateMachine_.setCurrentState(&stateMachine_.getSetAlarmSoundState());
    }

    showDate = hardware_.getInput().checkDPadBtnsExclusive(DPAD_DOWN);

    if(showDate) {
        hardware_.getClock().getDateStr(dateTimeStr_, 9);
    } else {
        hardware_.getClock().getTimeStr(dateTimeStr_, 9);
    }
}

void ClockState::draw() {
    hardware_.getDisplay().printMsgCenter(dateTimeStr_);
}

void ClockState::onEnter() {
    hardware_.getClock().getTimeStr(dateTimeStr_, 9);
}

void ClockState::onExit() {

}

ClockState::ClockState(Hardware& hardware, StateMachine& stateMachine) : State(hardware, stateMachine)  {

}

ClockState::~ClockState() {
    delete dateTimeStr_;
}