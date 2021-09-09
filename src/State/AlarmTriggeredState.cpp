#include "State/AlarmTriggeredState.h"
#include "Circadia.h"

void AlarmTriggeredState::update() {
    // listen to input for snooze or remote button
    // if snooze is pressed, just return to clock and wait for 2nd alarm to fire
    // if remote is pressed, just return to clock and ignore 2nd alarm
    // if ignored, just keep going
    // play music
    hardware_.getAudioSource().loop();
    hardware_.getClock().getTimeStr(dateTimeStr_, 8);

    if(isPrimaryAlarm_) {
        // listen for snooze
        // TODO: Change to snooze button
        if(hardware_.getInput().checkButtonJustPressed(Input::Button::Up)) {
            circadia_.setCurrentState(&circadia_.getClockState());
        }
    }

    // listen for remote
    // if(hardware_.getRemote().receivedTurnOff()) {
    if(hardware_.getInput().checkButtonJustPressed(Input::Button::Down)) {
        remotePressed_ = true;
        circadia_.setCurrentState(&circadia_.getClockState());
    }
}

void AlarmTriggeredState::draw() {
    hardware_.getDisplay().printMsgCenter(dateTimeStr_);
    long now = millis();

    if(now - lastInvert_ > FLASH_INTERVAL) {
        invertOn_ = !invertOn_;
        lastInvert_ = now;
    }

    if(invertOn_) {
        hardware_.getDisplay().invert();
    }
}

void AlarmTriggeredState::setPrimary(bool isPrimary) {
    isPrimaryAlarm_ = isPrimary;
}

void AlarmTriggeredState::onEnter() {
    hardware_.getClock().getTimeStr(dateTimeStr_, 8);
    hardware_.getRemote().reset();
    if(isPrimaryAlarm_) {
        hardware_.getAudioSource().loadCurrentSong();
    } else {
        hardware_.getAudioSource().setAudio("alarm");
    }
}

void AlarmTriggeredState::onExit() {
    hardware_.getAudioSource().stop();
    if(remotePressed_) {
        circadia_.alarmStatus_ = AlarmStatus::PrimaryReady;
        remotePressed_ = false;
    }

}

AlarmTriggeredState::AlarmTriggeredState(Hardware& hardware, Circadia& circadia) : State(hardware, circadia) {

}