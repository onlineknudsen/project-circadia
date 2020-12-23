#ifndef REMOTE_H
#define REMOTE_H

#include<RH_ASK.h>

class Remote {
    public:
        bool receivedTurnOff();
        Remote();
    private:
        RH_ASK rfDriver_;
};

#endif