#include "State/SetAlarmSoundState.h"
#include "Circadia.h"

SetAlarmSoundState::SetAlarmSoundState(Hardware& hardware, Circadia& circadia) : State(hardware, circadia) {
}

void SetAlarmSoundState::update() {
    if(hardware_.getInput().getDPad()) {
        lastInteraction_ = millis();
    }

    if((millis() - lastInteraction_) > TIMEOUT) {
        circadia_.setCurrentState(&circadia_.getClockState());
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