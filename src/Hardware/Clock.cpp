#include "Hardware/Clock.h"
#include<Wire.h>

#define PRIMARY_ALARM_CONFIG_DEFAULT 0b1000
#define SECONDARY_ALARM_CONFIG_DEFAULT 0b0100

void Clock::setDateTime(byte year, byte month, byte day, byte dow, byte hour, byte minute, byte second, bool h12Mode) {
    rtc_.setYear(year);
    rtc_.setMonth(month);
    rtc_.setDate(day);
    rtc_.setDoW(dow);
    rtc_.setHour(hour);
    rtc_.setMinute(minute);
    rtc_.setSecond(second);
    rtc_.setClockMode(h12Mode);
    h12Mode_ = h12Mode;
}

void Clock::getTimeStr(char* timeStr, int size) {
    char str[9];

    byte hr = rtc_.getHour(h12Mode_, pm_);
    byte min = rtc_.getMinute();

    char amOrPm[3];
    memcpy(amOrPm, pm_ ? "PM" : "AM", sizeof(amOrPm));

    if(min < 10) {
        sprintf(str, "%u:%u%u%s", hr, 0, min, amOrPm);
    }
    else {
        sprintf(str, "%u:%u%s", hr, min, amOrPm);
    }

    if(strlen(str) + 1 > size) {
        Serial.println("Clock::getTimeStr - passed string buffer too small");
        return;
    }

    strcpy(timeStr, str);
}

void Clock::getDateStr(char* dateStr, int size) {
    char str[9];
    bool century;

    byte month = rtc_.getMonth(century);
    byte date = rtc_.getDate();
    byte year = rtc_.getYear();

    sprintf(str, "%u.%u.%u", month, date, year);

    if(strlen(str) + 1 > size) {
        Serial.println("Clock::getDateStr - passed string buffer too small");
        return;
    }

    strcpy(dateStr, str);
}

void Clock::setAlarm(byte hour, byte minute, bool pm) {
    rtc_.setA1Time(1, hour, minute, 0, PRIMARY_ALARM_CONFIG_DEFAULT, false, true, pm);

    byte emergencyHour = hour;
    byte emergencyMin = minute;
    bool emergencyPM = pm;
    if(minute + 5 > 59) {
        emergencyHour++;
        emergencyMin = 60 - (minute + 5);
        if(emergencyHour == 12) {
            emergencyPM = !emergencyPM;
        }
    }

    rtc_.setA2Time(1, emergencyHour, emergencyMin, SECONDARY_ALARM_CONFIG_DEFAULT, false, true, emergencyPM);
}

bool Clock::isAlarmEnabled() {
    return rtc_.checkAlarmEnabled(1);
}

DS3231& Clock::getRTC() {
    return rtc_;
}