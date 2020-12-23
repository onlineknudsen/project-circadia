#ifndef SD_AUDIO_SOURCE_H
#define SD_AUDIO_SOURCE_H

#include<Arduino.h>
#include<SD.h>

#include "I2S.h"

#define SD_BUFFER_SIZE 2400

class SDAudioSource {
    public:
        SDAudioSource(int sdCS, int sdBufferSize = SD_BUFFER_SIZE, int i2sBClk = I2S_BCLK, int i2sLRC = I2S_LRC, int i2sDataOut = I2S_DOUT);
        ~SDAudioSource();
        // void setAudio(int index);
        void setAudio(const char* name);
        void loop();
        bool play();
        void stop();
        void restart();
    private:
        void initSD(int sdCS);
        void initI2S(int i2sBClk, int i2sLRC, int i2sDataOut);

        const char* audioDir = "/music/";
        File audioFile_;
        byte* sdBuffer_;
        int sdBufferSize_;
        bool audioEnabled_ = true;

};
#endif