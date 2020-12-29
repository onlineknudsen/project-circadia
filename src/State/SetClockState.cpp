#include "State/SetClockState.h"
#include "Circadia.h"

SetClockState::SetClockState(Hardware& hardware, Circadia& circadia) : State(hardware, circadia) {
}

void SetClockState::update() {

    if(hardware_.getInput().getDPad()) {
        lastInteraction_ = millis();
    }

    if(hardware_.getInput().checkDPadBtnJustPressed(Input::DPad::Left)) {
        byte currComponent = selectedComponent_;
        currComponent--;
        if(currComponent < 0) {
            currComponent = sizeof timeComponents - 1;
        }

        selectedComponent_ = static_cast<TimeComponent>(currComponent);
    } else if(hardware_.getInput().checkDPadBtnJustPressed(Input::DPad::Right)) {
        byte currComponent = selectedComponent_;
        currComponent++;
        if(currComponent > sizeof timeComponents - 1) {
            currComponent = 0;
        }
        selectedComponent_ = static_cast<TimeComponent>(currComponent);
    } else if(hardware_.getInput().checkDPadBtnJustPressed(Input::DPad::Up)) {
        increaseSelectedComponent();
    } else if(hardware_.getInput().checkDPadBtnJustPressed(Input::DPad::Down)) {
        decreaseSelectedComponent();
    }

    if((millis() - lastInteraction_) > TIMEOUT) {
        circadia_.setCurrentState(&circadia_.getClockState());
    }
}

void SetClockState::draw() {
    char dateTimeStr[9];
    byte startingUnderlineChar;
    byte endingUnderlineChar;
    if(selectedComponent_ > TimeComponent::AmPm)
        generateDateStr(dateTimeStr, 9, startingUnderlineChar, endingUnderlineChar);
    else
        generateTimeStr(dateTimeStr, 9, startingUnderlineChar, endingUnderlineChar);
    hardware_.getDisplay().printMsgCenterUnderline(dateTimeStr, startingUnderlineChar, endingUnderlineChar);
}

void SetClockState::onEnter() {
    lastInteraction_ = millis();

    bool h12;
    bool century;
    bool isPM;

    DS3231& rtc = hardware_.getClock().getRTC();

    timeComponents[TimeComponent::Hour] = rtc.getHour(h12, isPM);
    timeComponents[TimeComponent::Minute] = rtc.getMinute();
    timeComponents[TimeComponent::AmPm] = isPM;
    timeComponents[TimeComponent::Month] = rtc.getMonth(century);
    timeComponents[TimeComponent::Day] = rtc.getDate();
    timeComponents[TimeComponent::Year] = rtc.getYear();

}

void SetClockState::onExit() {
    DS3231& rtc = hardware_.getClock().getRTC();

    if(timeComponents[TimeComponent::AmPm]) {
        rtc.setHour(timeComponents[TimeComponent::Hour] + 12);
    } else {
        if(timeComponents[TimeComponent::Hour] == 12)
            rtc.setHour(0);
        else
            rtc.setHour(timeComponents[TimeComponent::Hour]);
    }
    rtc.setMinute(timeComponents[TimeComponent::Minute]);
    rtc.setSecond(0);
    rtc.setMonth(timeComponents[TimeComponent::Month]);
    rtc.setDate(timeComponents[TimeComponent::Day]);
    rtc.setYear(timeComponents[TimeComponent::Year]);
}

void SetClockState::increaseSelectedComponent() {
    byte max;
    if(selectedComponent_ == TimeComponent::AmPm) {
        timeComponents[TimeComponent::AmPm] = ~timeComponents[selectedComponent_] & 1;
        return;
    } else if(selectedComponent_ == TimeComponent::Day) {
        max = DAYS_IN_MONTH[timeComponents[TimeComponent::Month] - 1];
        if(max == 28 && isleapYear(timeComponents[TimeComponent::Year]))
            max = 29;
        if(timeComponents[TimeComponent::Day] >= max) {
            timeComponents[TimeComponent::Day] = 1;
            if(timeComponents[TimeComponent::Month] == 12) {
                timeComponents[TimeComponent::Month] = 1;
                timeComponents[TimeComponent::Year]++;
            } else {
                timeComponents[TimeComponent::Month]++;
            }
            return;
        }
    } else {
        max = TIME_COMPONENT_MAX[selectedComponent_];
    }

    if(timeComponents[selectedComponent_] >= max && selectedComponent_ != TimeComponent::Year)
        timeComponents[selectedComponent_] = TIME_COMPONENT_MIN[selectedComponent_];
    else
        timeComponents[selectedComponent_]++;

    if(timeComponents[TimeComponent::Day] > DAYS_IN_MONTH[timeComponents[TimeComponent::Month] - 1])
        timeComponents[TimeComponent::Day] = DAYS_IN_MONTH[timeComponents[TimeComponent::Month] - 1];
}

void SetClockState::decreaseSelectedComponent() {
    byte min;
    if(selectedComponent_ == TimeComponent::AmPm) {
        timeComponents[TimeComponent::AmPm] = ~timeComponents[selectedComponent_] & 1;
        return;
    } else if(selectedComponent_ == TimeComponent::Day) {
        if(timeComponents[TimeComponent::Day] == 1) {
            if(timeComponents[TimeComponent::Month] == 1)
                timeComponents[TimeComponent::Month] = 12;
            byte max = DAYS_IN_MONTH[timeComponents[TimeComponent::Month] - 1];
            timeComponents[TimeComponent::Day] = max;
            return;
        }
        min = TIME_COMPONENT_MIN[TimeComponent::Day];
    } else {
        min = TIME_COMPONENT_MIN[selectedComponent_];
    }

    if(timeComponents[selectedComponent_] <= min) {
        if(selectedComponent_ == TimeComponent::Year)
            timeComponents[selectedComponent_] = TIME_COMPONENT_MIN[TimeComponent::Year];
        else
            timeComponents[selectedComponent_] = TIME_COMPONENT_MAX[selectedComponent_];
    } else {
        timeComponents[selectedComponent_]--;
    }

    if(timeComponents[TimeComponent::Day] > DAYS_IN_MONTH[timeComponents[TimeComponent::Month] - 1])
        timeComponents[TimeComponent::Day] = DAYS_IN_MONTH[timeComponents[TimeComponent::Month] - 1];
}

void SetClockState::generateTimeStr(char* timeStr, byte size, byte& startingUnderlineChar, byte& endingUnderlineChar) {
    char str[8];

    byte hr = timeComponents[TimeComponent::Hour];
    byte min = timeComponents[TimeComponent::Minute];
    bool pm = timeComponents[TimeComponent::AmPm];

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

    strcpy(timeStr, str);

    if(selectedComponent_ > TimeComponent::AmPm)
        return;

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

void SetClockState::generateDateStr(char* dateStr, byte size, byte& startingUnderlineChar, byte& endingUnderlineChar) {
    char str[9];

    byte month = timeComponents[TimeComponent::Month];
    byte date = timeComponents[TimeComponent::Day];
    byte year = timeComponents[TimeComponent::Year];

    sprintf(str, "%u.%u.%u", month, date, year);

    if(strlen(str) + 1 > size) {
        Serial.println("Clock::getDateStr - passed string buffer too small");
        return;
    }

    strcpy(dateStr, str);

    byte monthOffset = 0;
    byte dateOffset = 0;
    if(month >= 10)
        monthOffset = 1;

    if(date >= 10)
        dateOffset = 1;

    switch(selectedComponent_) {
        case TimeComponent::Month:
            startingUnderlineChar = 0;
            endingUnderlineChar = monthOffset;
            break;
        case TimeComponent::Day:
            startingUnderlineChar = 2 + monthOffset;
            endingUnderlineChar = 2 + monthOffset + dateOffset;
            break;
        case TimeComponent::Year:
            startingUnderlineChar = 4 + monthOffset + dateOffset;
            endingUnderlineChar = 5 + monthOffset + dateOffset;
            break;
        default:
            break;
    }
}