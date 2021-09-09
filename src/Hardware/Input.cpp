#include "Hardware/Input.h"

void Input::setup() {
    // set as inputs (pull up resistors)
    Wire.beginTransmission(INPUTS_ADDRESS);
    Wire.write(INPUTS_I2C_GPPU);
    Wire.write(INPUTS_BUTTONS_EN);
    Wire.endTransmission();

    // reverse the polarity
    Wire.beginTransmission(INPUTS_ADDRESS);
    Wire.write(INPUTS_I2C_IPOL);
    Wire.write(INPUTS_BUTTONS_EN);
    Wire.endTransmission();
}

void Input::update() {
    oldButtons_ = buttons_;
    if((millis() - lastDebounce_) < DEBOUNCE_THRESHOLD)
        return;

    lastDebounce_ = millis();

    Wire.beginTransmission(INPUTS_ADDRESS);
    Wire.write(INPUTS_I2C_GPIO);
    Wire.endTransmission(false);
    Wire.requestFrom(INPUTS_ADDRESS, 1);
    buttons_ = Wire.read();

    for(int i = 0; i < 8; i++) {
        byte buttonMask = 1 << i;
        if((buttons_ & buttonMask) && !(oldButtons_ & buttonMask)) {
            buttonsHoldStart_[i] = millis();
        } else if (!(buttons_ & buttonMask) && (oldButtons_ & buttonMask)) {
            buttonsHoldStart_[i] = 0;

            if(buttonMask & currentHold_) {
                holdComplete_ = false;
                currentHold_ = 0;
            }
        }
    }
}

byte Input::getButtons() {
    return buttons_;
}

bool Input::checkButtonJustPressed(Button btn) {
    byte btns = (1 << btn);
    if(!checkButtonsExclusive(btns))
        return false;
    return !getOldButton(btn) && getButton(btn);
}

bool Input::checkButtonsExclusive(byte btns) {
    return !(btns ^ buttons_);
}

bool Input::getButton(Button btn) {
    return (buttons_ >> btn) & 1;
}

bool Input::getOldButton(Button btn) {
    return (oldButtons_ >> btn) & 1;
}

bool Input::checkButtonHold(Button btn, unsigned long time) {
    if(buttonsHoldStart_[btn] == 0) {
        return false;
    }

    unsigned long deltaTime = millis() - buttonsHoldStart_[btn];
    return deltaTime >= time;
}

bool Input::checkButtonsHold(byte btns, unsigned long time) {

    if(!checkButtonsExclusive(btns))
        return false;

    if(holdComplete_)
        return false;


    currentHold_ = btns;
    for(byte i = 0; i < 4; i++) {
        bool bit = (btns >> i) & 1;
        if(bit) {
            if(!checkButtonHold(static_cast<Button>(i), time))
                return false;
        }
    }

    holdComplete_ = true;
    return true;
}