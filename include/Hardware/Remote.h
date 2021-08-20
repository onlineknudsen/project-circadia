#ifndef REMOTE_H
#define REMOTE_H

#include<RH_ASK.h>

class Remote {
    public:
        bool receivedTurnOff();
        void reset();
        Remote();
    private:
        RH_ASK rfDriver_;
};

#endif