#include "State/SetAlarmState.h"
#include "Circadia.h"

SetAlarmState::SetAlarmState(Hardware& hardware, Circadia& circadia) : State(hardware, circadia) {

}

void SetAlarmState::update() {

    if(hardware_.getInput().getDPad()) {
        lastInteraction_ = millis();
    }

    if(hardware_.getInput().checkDPadBtnJustPressed(Input::Button::Left)) {
        int currComponent = selectedComponent_;
        currComponent--;
        if(currComponent < 0) {
            currComponent = sizeof alarmComponents - 1;
        }

        selectedComponent_ = static_cast<TimeComponent>(currComponent);
    } else if(hardware_.getInput().checkDPadBtnJustPressed(Input::Button::Right)) {
        int currComponent = selectedComponent_;
        currComponent++;
        if(currComponent > sizeof alarmComponents - 1) {
            currComponent = 0;
        }
        selectedComponent_ = static_cast<TimeComponent>(currComponent);
    } else if(hardware_.getInput().checkDPadBtnJustPressed(Input::Button::Up)) {
        increaseSelectedComponent();
    } else if(hardware_.getInput().checkDPadBtnJustPressed(Input::Button::Down)) {
        decreaseSelectedComponent();
    }

    if((millis() - lastInteraction_) > TIMEOUT) {
        circadia_.setCurrentState(&circadia_.getClockState());
    }
}

void SetAlarmState::draw() {
    char alarmStr[8];
    byte startingUnderlineChar;
    byte endingUnderlineChar;

    generateAlarmStr(alarmStr, 8, startingUnderlineChar, endingUnderlineChar);
    hardware_.getDisplay().printMsgCenterUnderline(alarmStr, startingUnderlineChar, endingUnderlineChar);
}

void SetAlarmState::onEnter() {
    lastInteraction_ = millis();

    byte sec;
    byte day;
    byte alarmControlWord;
    bool useDoW;
    bool is12h;
    bool isPM;

    DS3231& rtc = hardware_.getClock().getRTC();
    rtc.getA1Time(day, alarmComponents[TimeComponent::Hour], alarmComponents[TimeComponent::Minute], sec, alarmControlWord, useDoW, is12h, isPM);
    alarmComponents[TimeComponent::AmPm] = isPM;
}

void SetAlarmState::onExit() {
    hardware_.getClock().setAlarm(alarmComponents[TimeComponent::Hour], alarmComponents[TimeComponent::Minute], alarmComponents[TimeComponent::AmPm]);
}

void SetAlarmState::increaseSelectedComponent() {
    if(selectedComponent_ == TimeComponent::AmPm) {
        alarmComponents[TimeComponent::AmPm] = ~alarmComponents[selectedComponent_] & 1;
    } else {
        byte max = TIME_COMPONENT_MAX[selectedComponent_];
        if(alarmComponents[selectedComponent_] >= max)
            alarmComponents[selectedComponent_] = TIME_COMPONENT_MIN[selectedComponent_];
        else
            alarmComponents[selectedComponent_]++;
    }
}

void SetAlarmState::decreaseSelectedComponent() {
    if(selectedComponent_ == TimeComponent::AmPm) {
        alarmComponents[TimeComponent::AmPm] = ~alarmComponents[selectedComponent_] & 1;
    } else {
        byte max = TIME_COMPONENT_MIN[selectedComponent_];
        if(alarmComponents[selectedComponent_] <= max)
            alarmComponents[selectedComponent_] = TIME_COMPONENT_MAX[selectedComponent_];
        else
            alarmComponents[selectedComponent_]--;
    }
}

void SetAlarmState::generateAlarmStr(char* alarmStr, byte size, byte& startingUnderlineChar, byte& endingUnderlineChar) {
    char str[8];

    byte hr = alarmComponents[TimeComponent::Hour];
    byte min = alarmComponents[TimeComponent::Minute];
    bool pm = alarmComponents[TimeComponent::AmPm];

    char amOrPm[3];
    memcpy(amOrPm, pm ? "PM" : "AM", sizeof(amOrPm));

    if(min < 10) {
        sprintf(str, "%u:%u%u%s", hr, 0, min, amOrPm);
    }
    else {
        sprintf(str, "%u:%u%s", hr, min, amOrPm);
    }

    if(strlen(str) + 1 > size) {
        Serial.println("SetClockState::generateTimeStr - passed string buffer too small");
        return;
    }

    strcpy(alarmStr, str);

    int offset = 0;
    if(hr >= 10)
        offset = 1;

    switch(selectedComponent_) {
        case TimeComponent::Hour:
            startingUnderlineChar = 0;
            endingUnderlineChar = offset;
            break;
        case TimeComponent::Minute:
            startingUnderlineChar = 2 + offset;
            endingUnderlineChar = 3 + offset;
            break;
        case TimeComponent::AmPm:
            startingUnderlineChar = 4 + offset;
            endingUnderlineChar = 5 + offset;
            break;
        default:
            break;
    }
}