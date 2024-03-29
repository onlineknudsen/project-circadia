#include "Hardware/Remote.h"

Remote::Remote() : rfDriver_(2000, 34, 12) {
    if(!rfDriver_.init()) {
        Serial.println("Remote::Remote() - RF Driver failed to initialize");
    }
}

bool Remote::receivedTurnOff() {
    byte buf[10];
    byte bufLen = sizeof(buf);
    if(rfDriver_.recv(buf, &bufLen)) {
        Serial.println((char *) buf);
        return strcmp((char *) buf, "alarm_off") == 0;
    }
    return false;
}

void Remote::reset() {
    byte buf[10];
    byte bufLen = sizeof(buf);
    rfDriver_.recv(buf, &bufLen);
}