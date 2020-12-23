#ifndef CLOCK_H
#define CLOCK_H

#include<Wire.h>
#include<DS3231.h>

#define PRIMARY_ALARM_CONFIG_DEFAULT 0b1000
#define SECONDARY_ALARM_CONFIG_DEFAULT 0b0100

class Clock {
    public:
        void setDateTime(byte year, byte month, byte day, byte dow, byte hour, byte minute, byte second, bool h12Mode);
        void getTimeStr(char* timeStr, int size);

        void setPrimaryAlarm(byte hour, byte minute, byte second, byte day, bool useDoW = false, byte alarmConfigWord = PRIMARY_ALARM_CONFIG_DEFAULT, bool h12 = true, bool pm = true);
        void setPrimaryAlarm(byte hour, byte minute, byte second, bool useDoW = false, byte alarmConfigWord = PRIMARY_ALARM_CONFIG_DEFAULT, bool h12 = true, bool pm = true);

        void setSecondaryAlarm(byte hour, byte minute, byte day, bool useDoW = false, byte alarmConfigWord = SECONDARY_ALARM_CONFIG_DEFAULT, bool h12 = true, bool pm = true);
        void setSecondaryAlarm(byte hour, byte minute, bool useDoW = false, byte alarmConfigWord = SECONDARY_ALARM_CONFIG_DEFAULT, bool h12 = true, bool pm = true);
    private:
        DS3231 rtc_;
        bool pm_;
        bool h12Mode_;
};

#endif