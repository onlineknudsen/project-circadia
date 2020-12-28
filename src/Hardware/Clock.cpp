#include "Hardware/Clock.h"

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

void Clock::setPrimaryAlarm(byte hour, byte minute, byte second, byte day, bool useDoW, byte alarmConfigWord, bool h12, bool pm) {
    rtc_.setA1Time(day, hour, minute, second, alarmConfigWord, useDoW, h12, pm);
}

void Clock::setPrimaryAlarm(byte hour, byte minute, byte second, bool useDoW, byte alarmConfigWord, bool h12, bool pm) {
    byte day;
    if(useDoW) {
        day = rtc_.getDoW();
    } else {
        day = rtc_.getDate();
    }

    setPrimaryAlarm(hour, minute, second, day, useDoW, alarmConfigWord, h12, pm);
}

void Clock::setSecondaryAlarm(byte hour, byte minute, byte day, bool useDoW, byte alarmConfigWord, bool h12, bool pm) {
    rtc_.setA2Time(day, hour, minute, alarmConfigWord, useDoW, h12, pm);
}

void Clock::setSecondaryAlarm(byte hour, byte minute, bool useDoW, byte alarmConfigWord, bool h12, bool pm) {
    byte day;
    if(useDoW) {
        day = rtc_.getDoW();
    } else {
        day = rtc_.getDate();
    }

    setSecondaryAlarm(hour, minute, day, useDoW, alarmConfigWord, h12, pm);
}

DS3231& Clock::getRTC() {
    return rtc_;
}