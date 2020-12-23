#include "Hardware/Remote.h"

Remote::Remote() {
    rfDriver_.init();
}

bool Remote::receivedTurnOff() {
    byte buf[10];
    byte bufLen = sizeof(buf);
    if(rfDriver_.recv(buf, &bufLen)) {
        char* msgReceived = (char*) buf;
        return strcmp(msgReceived, "alarm_off") == 0;
    }
    return false;
}