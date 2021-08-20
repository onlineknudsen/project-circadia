#include "Circadia.h"


Circadia::Circadia(Hardware& hardware) : hardware_(hardware), clockState_(hardware, *this), setAlarmState_(hardware, *this), setClockState_(hardware, *this), setAlarmSoundState_(hardware, *this), alarmTriggeredState_(hardware, *this) {
    setCurrentState(&clockState_);
}

void Circadia::update() {
    hardware_.getInput().update();

    listen();
    debugListen();

    currentState_->update();
    currentState_->draw();

    hardware_.getDisplay().update();

}

void Circadia::setCurrentState(State* newState) {
    if(currentState_ != nullptr)
        currentState_->onExit();
    currentState_ = newState;
    currentState_->onEnter();
}

void Circadia::listen() {
    if(alarmStatus_ == AlarmStatus::PrimaryReady) {
        if(hardware_.getClock().checkAlarmFired(1)) {
            alarmStatus_ = AlarmStatus::EmergencyReady;
            Serial.println("Primary Alarm Fired");
            hardware_.getClock().clearAlarms();
            alarmTriggeredState_.setPrimary(true);
            setCurrentState(&alarmTriggeredState_);
        }
    } else if(alarmStatus_ == AlarmStatus::EmergencyReady) {
        if(hardware_.getClock().checkAlarmFired(2)) {
            alarmStatus_ = AlarmStatus::EmergencyFired;
            Serial.println("Secondary Alarm Fired");
            hardware_.getClock().clearAlarms();
            alarmTriggeredState_.setPrimary(false);
            setCurrentState(&alarmTriggeredState_);
        }
    }
}

void Circadia::debugListen() {
    if(Serial.available()) {
        char input = Serial.read();
        if(input == 's') {
            switch(alarmStatus_) {
            case AlarmStatus::PrimaryReady:
                Serial.println("AlarmStatus = PrimaryReady");
                break;
            case AlarmStatus::EmergencyReady:
                Serial.println("AlarmStatus = EmergencyReady");
                break;
            case AlarmStatus::EmergencyFired:
                Serial.println("AlarmStatus = EmergencyFired");
                break;
            default:
                break;
            }
        } else if (input == 'r') {
            Wire.beginTransmission(0x68);
            Wire.write(0x0f);
            Wire.endTransmission();
            Wire.requestFrom(0x68, 1);
            byte controlWord = Wire.read();
            Serial.print("A1F = ");
            Serial.println(controlWord & 1);
            Serial.print("A2F = ");
            Serial.println((controlWord & 2) >> 1);
        } else if (input == 'c') {
            Wire.beginTransmission(0x68);
            Wire.write(0x0f);
            Wire.endTransmission();
            Wire.requestFrom(0x68, 1);
            byte controlWord = Wire.read();

            Wire.beginTransmission(0x68);
            Wire.write(0x0f);
            Wire.write(controlWord & 0b11111100);
            Wire.endTransmission();

            alarmStatus_ = PrimaryReady;
        } else if (input == 'a') {
            byte day;
            byte hour;
            byte minute;
            byte controlByte;
            bool temp;
            bool pm;

            hardware_.getClock().getRTC().getA2Time(day, hour, minute, controlByte, temp, temp, pm);

            char str[9];

            char amOrPm[3];
            memcpy(amOrPm, pm ? "PM" : "AM", sizeof(amOrPm));

            if(minute < 10) {
                sprintf(str, "%u:%u%u%s", hour, 0, minute, amOrPm);
            }
            else {
                sprintf(str, "%u:%u%s", hour, minute, amOrPm);
            }

            Serial.print("A2 = ");
            Serial.println(str);
            Serial.print("ControlByte = ");
            Serial.println(controlByte, BIN);
        } else if (input == 't') {
            byte day;
            byte hour;
            byte minute;
            byte second;
            byte controlByte;
            bool temp;
            bool pm;

            hardware_.getClock().getRTC().getA1Time(day, hour, minute, second, controlByte, temp, temp, pm);

            second = 50;

            if(minute == 0) {
                minute = 59;
                if(hour == 1) {
                    hour = 12;
                    if(!pm) {
                        hour = 0;
                    }
                } else {
                    hour += 11;
                }
            } else {
                minute--;
                if(pm) {
                    hour += 12;
                }
            }

            hardware_.getClock().getRTC().setHour(hour);
            hardware_.getClock().getRTC().setMinute(minute);
            hardware_.getClock().getRTC().setSecond(second);
        } else if(input == 'y') {
            byte day;
            byte hour;
            byte minute;
            byte controlByte;
            byte second;
            bool temp;
            bool pm;

            hardware_.getClock().getRTC().getA2Time(day, hour, minute, controlByte, temp, temp, pm);

            second = 50;

            if(minute == 0) {
                minute = 59;
                if(hour == 1) {
                    hour = 12;
                    if(!pm) {
                        hour = 0;
                    }
                } else {
                    hour += 11;
                }
            } else {
                minute--;
                hour += 12;
            }

            hardware_.getClock().getRTC().setHour(hour);
            hardware_.getClock().getRTC().setMinute(minute);
            hardware_.getClock().getRTC().setSecond(second);
        }
    }

}

State& Circadia::getClockState() {
    return clockState_;
}

State& Circadia::getSetAlarmState() {
    return setAlarmState_;
}

State& Circadia::getSetClockState() {
    return setClockState_;
}

State& Circadia::getSetAlarmSoundState() {
    return setAlarmSoundState_;
}

State& Circadia::getAlarmTriggeredState() {
    return alarmTriggeredState_;
}

Hardware& Circadia::getHardware() {
    return hardware_;
}