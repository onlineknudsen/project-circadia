#ifndef INPUT_H
#define INPUT_H

#include<Arduino.h>

#define DPAD_UP (1 << Input::DPad::Up)
#define DPAD_DOWN (1 << Input::DPad::Down)
#define DPAD_LEFT (1 << Input::DPad::Left)
#define DPAD_RIGHT (1 << Input::DPad::Right)

#define DPAD_VERTICAL_AXIS DPAD_UP | DPAD_DOWN
#define DPAD_HORIZONTAL_AXIS DPAD_LEFT | DPAD_RIGHT



class Input {
    public:
        enum DPad {
            Up,
            Down,
            Left,
            Right
        };

        Input(int dUpPin, int dDownPin, int dLeftPin, int dRightPin);

        bool getDPadBtn(DPad btn);

        bool checkDPadBtnsHold(byte btns, unsigned long time);
        bool checkDPadBtnsExclusive(byte btns);
        bool checkDPadBtnJustPressed(DPad btn);


        byte getDPad();
        void update();

    private:
        byte dPad_;
        byte oldDPad_;
        unsigned long dPadHoldStart_[4];
        int dPadPins_[4];
        const unsigned long DEBOUNCE_THRESHOLD = 50;
        unsigned long lastDebounce_;

        bool holdComplete_;
        byte currentHold_;

        bool checkDPadBtnHold(DPad btn, unsigned long time);
        bool getOldDPadBtn(DPad btn);




};

#endif