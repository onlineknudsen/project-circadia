#include "Hardware/Hardware.h"

Hardware::Hardware(int displayCS, int sdCS) : display_(displayCS), audioSource_(sdCS) {

}

void Hardware::setup() {
    alarmClock_.getRTC().setClockMode(true);
    input_.setup();
}

Display& Hardware::getDisplay() {
    return display_;
}

Remote& Hardware::getRemote() {
    return remote_;
}

SDAudioSource& Hardware::getAudioSource() {
    return audioSource_;
}

Clock& Hardware::getClock() {
    return alarmClock_;
}

Input& Hardware::getInput() {
    return input_;
}