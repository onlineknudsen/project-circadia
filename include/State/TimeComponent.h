#ifndef TIME_COMPONENT_H
#define TIME_COMPONENT_H

#include<Arduino.h>

enum TimeComponent {
    Hour,
    Minute,
    AmPm,
    Month,
    Day,
    Year
};

const byte TIME_COMPONENT_MIN[] = { 1, 0, 0, 1, 1, 20 };

const byte TIME_COMPONENT_MAX[] = { 12, 59, 0, 12, 0, 0 };

const byte DAYS_IN_MONTH [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

#endif