#ifndef INPUT_H
#define INPUT_H

#include<Arduino.h>
#include<Wire.h>

#define BTN(inputBtn) (1 << inputBtn)

#define DPAD_VERTICAL_AXIS BTN(Input::Up) | BTN(Input::Down)
#define DPAD_HORIZONTAL_AXIS BTN(Input::Left) | BTN(Input::Right)

#define INPUTS_ADDRESS 0x20

#define INPUTS_I2C_IODIR 0x00
#define INPUTS_I2C_IPOL 0x01
#define INPUTS_I2C_GPINTEN 0x02
#define INPUTS_I2C_GPPU 0x06
#define INPUTS_I2C_GPIO 0x09

#define INPUTS_BUTTONS_EN 0x3F

class Input {
    public:
        enum Button {
            Up,
            Down,
            Left,
            Right,
            Snooze,
            Record
        };

        byte getButtons();
        bool checkButtonsHold(byte btns, unsigned long time);
        bool checkButtonJustPressed(Button btn);
        bool checkButtonsExclusive(byte btns);

        void setup();
        void update();

    private:
        byte buttons_;
        byte oldButtons_;
        unsigned long buttonsHoldStart_[8];

        const unsigned long DEBOUNCE_THRESHOLD = 50;
        unsigned long lastDebounce_;

        bool holdComplete_;
        byte currentHold_;

        bool getButton(Button btn);
        bool getOldButton(Button btn);
        bool checkButtonHold(Button btn, unsigned long time);

};

#endif