#ifndef CLOCK_H
#define CLOCK_H

#include<DS3231.h>

class Clock {
    public:
        void setDateTime(byte year, byte month, byte day, byte dow, byte hour, byte minute, byte second = 0, bool h12Mode = true);
        void getTimeStr(char* timeStr, int size);
        void getDateStr(char* dateStr, int size);

        void setAlarm(byte hour, byte minute, bool pm);

        void toggleAlarm(bool enabled);
        bool checkAlarmFired(byte alarm);

        bool isAlarmEnabled();

        DS3231& getRTC();
    private:
        DS3231 rtc_;
        bool pm_;
        bool h12Mode_;
};

#endif