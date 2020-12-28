#include "Hardware/Input.h"

Input::Input(byte dUpPin, byte dDownPin, byte dLeftPin, byte dRightPin)  {
    dPadPins_[DPad::Up] = dUpPin;
    dPadPins_[DPad::Down] = dDownPin;
    dPadPins_[DPad::Left] = dLeftPin;
    dPadPins_[DPad::Right] = dRightPin;

    for(byte i = 0; i < 4; i++) {
        pinMode(dPadPins_[i], INPUT_PULLUP);
    }

}

void Input::update() {
    oldDPad_ = dPad_;
    if((millis() - lastDebounce_) < DEBOUNCE_THRESHOLD)
        return;

    lastDebounce_ = millis();
    for(byte i = 0; i < 4; i++) {
        bool btnBit = !digitalRead(dPadPins_[i]);
        byte posMask = 1 << i;
        dPad_ = (dPad_ & ~posMask) | (btnBit << i);
        if(btnBit && !((oldDPad_ & posMask) >> i)) {
            dPadHoldStart_[i] = millis();
        } else if(!btnBit && ((oldDPad_ & posMask) >> i)) {
            dPadHoldStart_[i] = 0;
            // if this button was part of the hold pattern and it was just released
            // then we want to say that the hold is no longer complete
            // posMask    currentHold_
            // 0b0010  &  0b0011 = 0b0010

            if(posMask & currentHold_) {
                holdComplete_ =  false;
                currentHold_ = 0;
            }
        }
    }
}

bool Input::checkDPadBtnHold(DPad btn, unsigned long time) {
    if(dPadHoldStart_[btn] == 0) {
        return false;
    }

    unsigned long deltaTime = millis() - dPadHoldStart_[btn];
    return deltaTime >= time;
}

bool Input::checkDPadBtnsHold(byte btns, unsigned long time) {

    if(!checkDPadBtnsExclusive(btns))
        return false;

    if(holdComplete_)
        return false;


    currentHold_ = btns;
    for(byte i = 0; i < 4; i++) {
        bool bit = (btns >> i) & 1;
        if(bit) {
            if(!checkDPadBtnHold(static_cast<DPad>(i), time))
                return false;
        }
    }

    holdComplete_ = true;
    return true;
}

bool Input::checkDPadBtnJustPressed(DPad btn) {
    byte btns = (1 << btn);
    if(!checkDPadBtnsExclusive(btns))
        return false;
    return !getOldDPadBtn(btn) && getDPadBtn(btn);
}

bool Input::checkDPadBtnsExclusive(byte btns) {
    return !(btns ^ dPad_);
}

byte Input::getDPad() {
    return dPad_;
}

bool Input::getDPadBtn(DPad btn) {
    return (dPad_ >> btn) & 1;
}

bool Input::getOldDPadBtn(DPad btn) {
    return (oldDPad_ >> btn) & 1;
}
