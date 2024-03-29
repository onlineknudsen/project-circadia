#include "Hardware/Clock.h"
#include<Wire.h>

#define PRIMARY_ALARM_CONFIG_DEFAULT 0b1000
#define SECONDARY_ALARM_CONFIG_DEFAULT 0b01000000

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
    byte emergencyMin = minute + 5;
    bool emergencyPM = pm;
    if(minute + 5 > 59) {
        emergencyHour++;
        emergencyMin = (minute + 5) - 60;
        if(emergencyHour == 12) {
            emergencyPM = !emergencyPM;
        }
    }

    char str[9];

    char amOrPm[3];
    memcpy(amOrPm, emergencyPM ? "PM" : "AM", sizeof(amOrPm));

    if(emergencyMin < 10) {
        sprintf(str, "%u:%u%u%s", emergencyHour, 0, emergencyMin, amOrPm);
    }
    else {
        sprintf(str, "%u:%u%s", emergencyHour, emergencyMin, amOrPm);
    }

    Serial.println(str);

    rtc_.setA2Time(1, emergencyHour, emergencyMin, SECONDARY_ALARM_CONFIG_DEFAULT, false, true, emergencyPM);
}

bool Clock::checkAlarmFired(byte alarm) {
    Wire.beginTransmission(0x68);
    Wire.write(0x0f);
    Wire.endTransmission(false);

    Wire.requestFrom(0x68, 1);
    byte controlWord = Wire.read();

    if(alarm == 1) {
        return controlWord & 0b00000001;
    } else {
        return controlWord & 0b00000010;
    }
}

void Clock::clearAlarms() {
    Wire.beginTransmission(0x68);
    Wire.write(0x0f);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 1);
    byte controlWord = Wire.read();

    Wire.beginTransmission(0x68);
    Wire.write(0x0f);
    Wire.write(controlWord & 0b11111100);
    Wire.endTransmission();
}

DS3231& Clock::getRTC() {
    return rtc_;
}