#include "Hardware/Hardware.h"

Hardware::Hardware(int displayCS, int sdCS, int dUpPin, int dDownPin, int dLeftPin, int dRightPin) : display_(displayCS), audioSource_(sdCS), input_(dUpPin, dDownPin, dLeftPin, dRightPin) {

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