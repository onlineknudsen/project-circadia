#include "State/SetAlarmSoundState.h"
#include "Circadia.h"

SetAlarmSoundState::SetAlarmSoundState(Hardware& hardware, Circadia& circadia) : State(hardware, circadia) {
}

void SetAlarmSoundState::update() {
    if(hardware_.getInput().getDPad()) {
        lastInteraction_ = millis();
    }

    if(hardware_.getInput().checkDPadBtnJustPressed(Input::DPad::Left)) {
        currentSongIndex_--;
        if(currentSongIndex_ < 0) {
            currentSongIndex_ = songCount_;
        }
        updateSongTitle();
    } else if(hardware_.getInput().checkDPadBtnJustPressed(Input::DPad::Right)) {
        currentSongIndex_++;
        if(currentSongIndex_ > songCount_) {
            currentSongIndex_ = 0;
        }
        updateSongTitle();
    }

    if((millis() - lastInteraction_) > TIMEOUT) {
        circadia_.setCurrentState(&circadia_.getClockState());
    }
}

void SetAlarmSoundState::draw() {
    hardware_.getDisplay().displayScrollLeft();
}

void SetAlarmSoundState::onEnter() {
    lastInteraction_ = millis();
    currentSongIndex_ = hardware_.getAudioSource().getCurrentSongIndex();
    songCount_ = hardware_.getAudioSource().getSongCount();
    hardware_.getAudioSource().getSongTitle(currentSongIndex_, currentSongTitle_, 75); // FIXME: MAGIC NUMBERS
    hardware_.getDisplay().setScrollMsg(currentSongTitle_);
}

void SetAlarmSoundState::onExit() {
    hardware_.getAudioSource().setCurrentSong(currentSongIndex_);
}

void SetAlarmSoundState::updateSongTitle() {
    if(currentSongIndex_ == songCount_) {
        strcpy(currentSongTitle_, "Random");
    } else {
        hardware_.getAudioSource().getSongTitle(currentSongIndex_, currentSongTitle_, 75); // FIXME: MAGIC NUMBERS
    }
    hardware_.getDisplay().setScrollMsg(currentSongTitle_);
}