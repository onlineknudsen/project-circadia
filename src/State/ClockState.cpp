#include "State/ClockState.h"

#include "Circadia.h"


void ClockState::update() {

    showDate = hardware_.getInput().checkButtonsExclusive(BTN(Input::Down));

    if(showDate) {
        hardware_.getClock().getDateStr(dateTimeStr_, 9);
    } else {
        hardware_.getClock().getTimeStr(dateTimeStr_, 9);
    }

    if(hardware_.getInput().checkButtonsHold(DPAD_VERTICAL_AXIS, HOLD_TIME)) {
        circadia_.setCurrentState(&circadia_.getSetAlarmState());
    } else if (hardware_.getInput().checkButtonsHold(DPAD_HORIZONTAL_AXIS, HOLD_TIME)) {
        circadia_.setCurrentState(&circadia_.getSetClockState());
    } else if (hardware_.getInput().checkButtonsHold(BTN(Input::Left), HOLD_TIME)) {
        circadia_.setCurrentState(&circadia_.getSetAlarmSoundState());
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

ClockState::ClockState(Hardware& hardware, Circadia& circadia) : State(hardware, circadia)  {

}