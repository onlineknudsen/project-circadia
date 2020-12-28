#include "State/SetAlarmState.h"
#include "Circadia.h"

SetAlarmState::SetAlarmState(Hardware& hardware, Circadia& circadia) : State(hardware, circadia) {

}

void SetAlarmState::update() {

    if(hardware_.getInput().getDPad()) {
        lastInteraction_ = millis();
    }

    if((millis() - lastInteraction_) > TIMEOUT) {
        circadia_.setCurrentState(&circadia_.getClockState());
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