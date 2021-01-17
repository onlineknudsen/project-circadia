#include "State/AlarmTriggeredState.h"
#include "Circadia.h"

void AlarmTriggeredState::update() {
    // listen to input for snooze or remote button

    // play music
    hardware_.getAudioSource().loop();
    hardware_.getClock().getTimeStr(dateTimeStr_, 9);
    if(millis() - timeout_ > 15000) {
        circadia_.setCurrentState(&circadia_.getClockState());
    }
}

void AlarmTriggeredState::draw() {
    hardware_.getDisplay().printMsgCenter(dateTimeStr_);

    if(millis() - lastInvert_ > FLASH_INTERVAL) {
        invertOn_ = !invertOn_;
        lastInvert_ = millis();
    }

    if(invertOn_) {
        hardware_.getDisplay().invert();
    }
}

void AlarmTriggeredState::onEnter() {
    hardware_.getAudioSource().setAudio("alarm");
    hardware_.getClock().getTimeStr(dateTimeStr_, 9);
    timeout_ = millis();
}

void AlarmTriggeredState::onExit() {
    hardware_.getAudioSource().stop();
}

AlarmTriggeredState::AlarmTriggeredState(Hardware& hardware, Circadia& circadia) : State(hardware, circadia) {

}

AlarmTriggeredState::~AlarmTriggeredState() {
    delete dateTimeStr_;
}